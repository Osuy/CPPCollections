#pragma once
#include <mutex>
#include <queue>

/*
	һ���̵߳ȴ���һ���̼߳���������ż���
	�Ⲣ��join��ϡ�
	��һ���̼߳�����������������Ҫ����������Ȼ�������У��������ϵؼ���
	�����߳�Ҳ�������У�ֻ����ĳЩ����ͣ�٣��ȴ���һ�̵߳Ľ��
	�����Ĺ��̳����������൱�������߳��ں�������

	�������̺߳�����������ơ�����������Ҫ�������ݣ����Կ���������Ϊȫ�ֱ�����
	������һ��������������Ƿ����
	��Ȼ���ڶ��̷߳��ʣ�������Ҫ���Ᵽ��
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
	some_data;// ʹ��some_data
}

/*
	�߳�1�������ϵط��� usable
	���߳�2���ú���some_data���߳�1��������ס����ʹ��some_data

	���������ȱ�����ڣ��߳�1��usable�Ĳ�ͣ�ؽ���������ķ��ʣ�����ʹ���������½����ﲻ�����ö��߳�������ܵ�Ŀ��
	��������sleep���߳�1�ķ���usable����ôƵ������������sleep��ã���ԶԶ����cpuʱ������
	�ǳ��򡰱����������ܵ��¡�
	�䱾��ԭ�����ڣ�һ���̵߳ȴ���һ���߳�ͬ����Ҫôѭ���ȴ���Ҫô���ڵȴ��������ٶ�����������˷�
	Ψ���������߳�֪ͨ��һ���߳�ͬ�������С��޷��νӡ���Ч��

	C++��׼���ṩ������������ʵ���޷�ͬ��
	
	4.1��������
	wait������һ��������Ͷ��ԡ��������Ϊ�棬��return
		���򣬽�����������������̣߳��ȴ�notify

	notify_one������һ�������������������̡߳��Ǹ��̻߳����¶�������������Ȼ���ٴ��ж϶���
		�������false����������������
		�����Ǹ��߳̿�ʼ����ִ��

	���Ե����ã������߳���������������ݣ�ͨ�����ڹ������ݼ���ó���������������������������̼߳���������������
	�������ã����ڶ��Կ��ܻ���ʹ������ݣ�������Ҫ���������Ҹ���Ҳ�ܹ���wait�ٿ�
	���ڴ����wait����������Ҫ�Ӷ������������ͽ�������������Ҫʹ��unique_lock
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

		// �˴���һ������������ȥ��������Ϊ������������ notify_one ʱ����������ǽ�����
		// �뿪�������lk��������
		{
			std::lock_guard lk{ mtx };
			data_queue.push(data);
		}
		data_cond.notify_one();// ����ֻ�����˵������ݣ�����Ҳֻ����һ���߳�
	}
}

void data_processing_thread()
{
	while (true) // while�ڻ�������������ѭ�������˷�����
	{
		std::unique_lock lk{ mtx }; // ��������һ��ֻ���߳�1����pushʱ������Ŀ���Ǳ�֤���ķ���data_queueʱ�����ݰ�ȫ��������Ϊ��ͬ��
		data_cond.wait(lk, [] {return !data_queue.empty(); });// ���Ϊ�棬�����ִ�У������������ȴ�notify

		// �������ִ�У�˵������ԭ���������ݣ��������������ݲ��ұ��߳�1������
		// ���ǿ��Բ����пգ�ֱ��ȡ��
		auto data = data_queue.front();
		data_queue.pop();
		
		lk.unlock();// ��ʱ�������ٷ��ʶ��У����������߳�1���Լ���push

		process(data);
		if (is_last_data())break;
	}
}

/*
	�������������һ���̰߳�ȫ�Ķ��С��ο���ȫ��ջ
	
*/
// std::queue �Ľӿ�
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
	 mutable std::mutex mtx;//const ����ҲҪ���������Ի�������mutable
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

	// ����pop����������Ҫô�ɹ���Ҫôʧ��
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

	// �ȴ�pop��Ҫô�ɹ���Ҫô����
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
	notify_all ��������Щ����̵߳ȴ�ͬһ���źŵ����
	��������ǳ�ʼ�����״�Ψһ���������ع˵����£�ʹ��call_once
	��Ϊ�״�Ψһ�¼����������������Ͳ����ٱ�ʹ��

	���˶���̣߳�����ǵ����̵߳ȴ�ͬ������notify_one����ʤ��
	���Ƕ��ڵ����̵߳��״�Ψһ���Ȳ��ʺ�ʹ��notify_one��Ҳ���ʺ�call_once
	std::future���ʺϴ˳���
*/

/*
	���ֹ��ߵıȽ�
	��ȴ��໽�ѣ�notify_all
	��ȴ������ѣ�notify_one
	���ȴ������ѣ�future
	���߳��״�Ψһ�¼���call_once
*/