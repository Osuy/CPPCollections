#pragma once
#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>

namespace messaging
{
	// 所有消息的基类，仅一个虚析构。为了所有消息对象的智能指针能向上转型为基类指针
	// 以便存放在容器里，并且能被正常析构
	struct message_base
	{
		virtual ~message_base() {}
	};

	// 泛化的消息子类（包装类。Msg可以是任意类型，而不必继承自message_base）
	template<typename Msg>
	struct wrapped_message : public message_base
	{
		Msg contents;
		explicit wrapped_message(const Msg& contents_)
			: contents{ contents_ }
		{
		}
	};

	class queue
	{
		std::mutex m;
		std::condition_variable c;
		std::queue<std::shared_ptr<message_base>> q;

	public:

		// 传入任意类型的消息并 唤醒等待所有阻塞的线程
		template<typename T>
		void push(const T& msg)
		{
			std::lock_guard<std::mutex>  lk{ m };
			q.push(std::make_shared<wrapped_message<T>>(msg));
			c.notify_all();
		}

		// 等待并出队一个消息
		std::shared_ptr<message_base> wait_and_pop()
		{
			std::unique_lock<std::mutex> lk{ m };
			c.wait(lk, [&] { return !q.empty(); });
			auto res = q.front();
			q.pop();
			return res;
		}
	};

	// sender不持有队列，而只是单纯地向队列里push
	class sender
	{
		queue* q;
	public:
		sender() :q{ nullptr } {}

		explicit sender(queue* q_) :q{ q_ } {}

		template<typename  Message>
		void send(const Message& msg)
		{
			if (q)q->push(msg);
		}

		
	};
	class close_queue {};

	// dispatcher 也不持有队列。它的任务由析构函数完成
	class dispatcher
	{
		queue* q;
		bool chained;
		dispatcher(const dispatcher&) = delete;
		dispatcher& operator=(const dispatcher&) = delete;

		template<typename Dispatcher, typename Msg, typename Func>
		friend class TemplateDispatcher;

		void wait_and_dispatch()
		{
			for (;;)
			{
				auto msg = q->wait_and_pop();
				dispatch(msg);
			}
		}

		bool dispatch(const std::shared_ptr<message_base>& msg)
		{
			if (dynamic_cast<wrapped_message<close_queue>*>(msg.get()))
			{
				throw close_queue{};//dispatch只发生于析构函数中，所以析构函数需定义为可抛异常
			}
			return false;
		}

	public:
		dispatcher(dispatcher&& other) : q{ other.q }, chained{ other.chained }
		{
			other.chained = true;
		}

		explicit dispatcher(queue* q_) : q{ q_ }, chained{ false } {}

		template<typename Message, typename Func>
		TemplateDispatcher<dispatcher, Message, Func> handle(Func&& f)
		{
			return TemplateDispatcher<dispatcher, Message, Func>{ q, this, std::forward<Func>(f) };
		}

		~dispatcher() noexcept(false)
		{
			if (!chained)
			{
				wait_and_dispatch();
			}
		}
	};

	// receiver持有队列，并且可以转型为sender和dispatcher
	class receiver
	{
		queue q;
	public:
		operator sender()
		{
			return sender(&q);
		}

		dispatcher wait()
		{
			return dispatcher(&q);
		}
	};

	template<typename PreviousDispatcher, typename Msg, typename Func>
	class TemplateDispatcher
	{
		queue* q;
		PreviousDispatcher* prev;
		Func f;
		bool chained;
		TemplateDispatcher(const TemplateDispatcher&) = delete;
		TemplateDispatcher& operator=(const TemplateDispatcher&) = delete;
		template<typename Dispatcher, typename OtherMsg, typename OtherFunc>
		friend class TemplateDispatcher;

		void wait_and_dispatch()
		{
			for (;;)
			{
				auto msg = q->wait_and_pop();
				if (dispatch(msg))break;
			}
		}

		void dispatch(const std::shared_ptr<message_base>& msg)
		{
			if (auto wrapped = dynamic_cast<wrapped_message<Msg>*>(msg.get()))
			{
				f(wrapped->contents);
					return true;
			}
			else
			{
				return prev->dispatch(msg);
			}
		}

	public:
		TemplateDispatcher(TemplateDispatcher&& other)
			: q{ other.q }
			, prev{ other.prev }
			, f{ std::move(other.f) }
			, chained{ other.chained }
		{
			other.chained = true;
		}

		TemplateDispatcher(queue* q_, PreviousDispatcher* prev_, Func&& f_)
			: q{ q_ }
			, prev{ prev_ }
			, f{ std::forward<Func>(f_) }
			, chained{ false }
		{
			prev_->chained = true;
		}

		template<typename OtherMsg, typename OtherFunc>
		TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc> handle(OtherFunc&& of)
		{
			return TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>{ q, this, std::forward<OtherFunc>(of) };
		}
		

		~TemplateDispatcher()
		{
			if (!chained) wait_and_dispatch();
		}
	};
}