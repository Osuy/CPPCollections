#pragma once
#include <mutex>
#include <queue>

/*
	一个线程等待另一个线程计算出结果后才继续
	这并非join汇合。
	另一个线程计算结果，并不代表它要结束。它依然继续运行，持续不断地计算
	而本线程也持续运行，只是在某些步骤停顿，等待另一线程的结果
	这样的过程持续发生，相当于两个线程在合作运行

	让两个线程合作并不难设计。由于它们需要交互数据，所以可以另数据为全局变量。
	再用另一个变量标记数据是否可用
	当然由于多线程访问，所以需要互斥保护
*/

std::mutex mtx;
int some_data;
bool usable = false;
void set_some_data(int sd)
{
	std::unique_lock lk{ mtx };
	usable = true;
	some_data = sd;
}

void use_some_data()
{
	while (!usable);
	std::unique_lock lk{ mtx };
	usable = false;
	some_data;// 使用some_data
}

/*
	线程1持续不断地访问 usable
	当线程2设置好了some_data后，线程1会立即锁住互斥使用some_data

	如此做法的缺点在于：线程1对usable的不停地进行无意义的访问，反而使整体性能下降，达不到利用多线程提高性能的目的
	可以利用sleep让线程1的访问usable不那么频繁。但是无论sleep多久，都远远大于cpu时钟周期
	是程序“表现起来性能低下”
	其本质原因在于：一个线程等待另一个线程同步，要么循环等待，要么周期等待，或多或少都造成了性能浪费
	唯有做到：线程通知另一个线程同步，才有“无缝衔接”的效果

	C++标准库提供了条件变量来实现无缝同步
	
	4.1条件变量
	wait，传入一个锁对象和断言。如果断言为真，则return
		否则，将锁解除，并阻塞本线程，等待notify

	notify_one，唤醒一个被条件变量阻塞的线程。那个线程会重新对锁对象上锁，然后再次判断断言
		如果断言false，解锁，继续阻塞
		否则，那个线程开始继续执行

	断言的作用：决定线程阻塞或继续的依据，通常基于共享数据计算得出（共享数据如果满足条件，则线程继续，否则阻塞）
	锁的作用：由于断言可能会访问共享数据，所以需要加锁。并且该锁也能够被wait操控
	由于传入给wait的锁对象需要视断言条件加锁和解锁，所以至少要使用unique_lock
*/

std::condition_variable data_cond;
struct data_chunk {};
std::queue<data_chunk> data_queue;
data_chunk prepare_data();
bool more_data_to_prepare();
void process(data_chunk&);
bool is_last_data();
void data_preparation_thread()
{
	while (more_data_to_prepare())
	{
		const auto data = prepare_data();

		// 此处在一个新作用域里去上锁，因为下文条件变量 notify_one 时，互斥必须是解锁的
		// 离开作用域后，lk析构解锁
		{
			std::lock_guard lk{ mtx };
			data_queue.push(data);
		}
		data_cond.notify_one();// 由于只插入了单个数据，所以也只唤醒一个线程
	}
}

void data_processing_thread()
{
	while (true) // while内会阻塞，所以死循环不会浪费性能
	{
		std::unique_lock lk{ mtx }; // 加锁，这一步只会线程1正在push时阻塞。目的是保证下文访问data_queue时的数据安全。并不是为了同步
		data_cond.wait(lk, [] {return !data_queue.empty(); });// 如果为真，则继续执行；否则阻塞，等待notify

		// 如果继续执行，说明队列原本就有数据，或者新增了数据并且被线程1唤醒了
		// 于是可以不比判空，直接取用
		auto data = data_queue.front();
		data_queue.pop();
		
		lk.unlock();// 此时已无需再访问队列，解锁，让线程1可以继续push

		process(data);
		if (is_last_data())break;
	}
}

/*
	用条件变量设计一个线程安全的队列。参考安全的栈
	
*/
// std::queue 的接口
template<typename T, typename Container = std::deque<T>>
class my_queue
{
	explicit my_queue(const Container&);
	explicit my_queue(Container&& = ());
	template<typename Alloc> explicit my_queue(const Alloc&);
	template<typename Alloc> explicit my_queue(const Container&, const Alloc&);
	template<typename Alloc> explicit my_queue(Container&&, const Alloc&);
	template<typename Alloc> explicit my_queue(my_queue&&, const Alloc&);
	void swap(my_queue&&);
	bool empty()const;
	size_type size()const;
	T& front();
	const T& front()const;
	T& back();
	const T& back()const;

	void push(const T&);
	void push(T&&);
	void pop();
	
	template<typename ...Args>
	void emplace(Args&&...args);
};

// threadsafe_queue
template<typename T>
class threadsafe_queue
{
	 mutable std::mutex mtx;//const 函数也要加锁，所以互斥需是mutable
	std::queue<T> q;
	std::condition_variable cond;
public:
	threadsafe_queue() = default;
	threadsafe_queue(const threadsafe_queue& other)
	{
		std::lock_guard lk{ mtx };
		q = other.q;
	}
	threadsafe_queue& opertator = (const threadsafe_queue&) = delete;

	bool empty()const
	{
		std::lock_guard lk{ mtx };
		return q.empty();
	}
	void push(T new_value)
	{
		std::lock_guard lk{ mtx };
		q.push(new_value);
		cond.notify_one();
	}

	// 立即pop，不阻塞，要么成功，要么失败
	bool try_pop(T& value)
	{
		std::lock_guard lk{ mtx };
		if (q.empty())
		{
			return false;
		}

		value = q.front();
		q.pop();
		return true;
	}
	std::shared_ptr<T> try_pop()
	{
		std::lock_guard lk{ mtx };
		if (q.empty())
		{
			return std::shared_ptr<T>{};
		}

		auto res = std::make_shared(q.front());
		q.pop();

		return res;
	}

	// 等待pop，要么成功，要么阻塞
	void wait_and_pop(T& value)
	{
		std::unique_lock lk{ mtx };
		cond.wait(lk, [this] {return !q.empty(); });
		value = q.front();
		q.pop();
	}
	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_lock lk{ mtx };
		cond.wait(lk, [this] {return !q.empty(); });
		auto res = std::make_shared(q.front());
		q.pop();

		return res;
	}
};

/*
	notify_all 适用于那些多个线程等待同一个信号的情况
	但是如果是初始化等首次唯一的情况，请回顾第三章，使用call_once
	因为首次唯一事件发生后，条件变量就不会再被使用

	除了多个线程，如果是单个线程等待同步，则notify_one可以胜任
	但是对于单个线程的首次唯一，既不适合使用notify_one，也不适合call_once
	std::future更适合此场景
*/

/*
	四种工具的比较
	多等待多唤醒：notify_all
	多等待单唤醒：notify_one
	单等待单唤醒：future
	多线程首次唯一事件：call_once
*/