#pragma once
#include <mutex>
#include <queue>
#include <future>
#include <set>
#include <list>
#include <type_traits>

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
	���߳�һ����ͬ���¼� std::future���ʺϴ˳���

	std::future ��һ���¼��������޷����ã�Ψ�и��¼���ɣ����ſɶ��ý��
	���˷����߳�û�з���ֵ��ȱ��
	������std::future��ģ�����������void�����㲻�÷���ֵ��һ����ͬ��Ҳ��future������
	
	һ���߳�������һ���߳���Ҫͬ�������Ҹ�ͬ����һ��һ�����¼����������߳�2���¼��������߳�1��һ��future
	Ȼ���߳�1����future�еĽ�����������¼���δ��ɲ���������������
	���������������Ҫ��������Գ���futureȥ�������£�֪����Ҫʱ�ٷ���
	std::future ���������߳�ͬ��������������������̷߳��ʣ���Ȼ��Ҫ���Ᵽ��
	���������������һ����ʽ�����һ���¼���������std::shared_future������
	���Ƕ���̶߳����Զ����¼��Ľ����������ɾ���

	����ʹ��std::async������future��������thread���ƣ����������ִ�ж����з���ֵ
*/

int find_the_answer_to_ltuae();

void example2()
{
	std::future<int> answer = std::async(find_the_answer_to_ltuae);
	// do other thing
	answer.get(); // ���δ������ɣ�������
}

/*
	async ��thread���ƣ����ܿ�ִ�ж���
	�����Ǻ���ָ�룬��Ա�������º�����lambda���ʽ�ȵ�
	��������������Ҳͬ�����ڿ������ƶ����ִ�������֧�����ã�����std::ref���
*/

struct A1
{
	void foo(int, std::string const&);
	std::string bar(std::string const&);
};

struct A2
{
	double operator()(double);
};

struct A3
{
	A3() = default;
	A3(A3&&) = default;
	A3(const A3&) = delete;
	A3& operator=(const A3&) = delete;
	A3& operator=(A3&&) = delete;

	double operator()();
};

void example3()
{
	A1 a1;
	auto f1 = std::async(A1::foo, &a1, 42, "hello"); // ��������ַ����p->foo��ʽ����
	auto f2 = std::async(A1::bar, a1, "goodbye");// ����ֵ���ݣ��Զ���Ŀ���tmp.bar��ʽ����

	A2 a2;
	auto f3 = std::async(A2(), 3.141); // ������ʱ���󣬷����ƶ����Զ�����ƶ����� tmp()��ʽ����
	auto f4 = std::async(std::ref(a2), 3.141); // ����a2��ref���� a2()��ʽ����

	auto f4 = std::async(A3{}); // ������ʱ���󣬷����ƶ����Զ�����ƶ����� tmp()��ʽ����
}

/*
	����async��promise��package_taskҲ���Ի�ȡ future
	package_task �� function���ƣ�Ҳ��һ����ִ�ж����������ڣ�
		function����ʱ���̻�÷���ֵ���ͺ���һ��
		package_task ���ú󣬷���ֵ������future�С�future������get_future��ȡ��
		package_task �ĳ����÷��Ǵ����󣬻�ȡ����������future��Ȼ���ٰ����ƶ�����һ���߳�ȥ����
		���߳�����Ҫʱ��ȡfuture���ֵ

	��������ui��ܶ���������ר�ŵ��߳��ϣ�����߳���Ҫ����ui������ui�̷߳�����Ϣ
	package_task����ʵ�ִ����ܣ����ұ���Ϊÿ����Ϣ����ר�е�����
	������Ϣ����ö���ඨ�塣��ui�̷߳���ö��ֵ�Լ�����������֪ͨui���¡������µĲ���
	����Ҫ�����µ�ö��ֵ������չ�Բ�ǿ������ʹ�����ļ�����ö�٣���ģ�����
	��ʹ��package_task�������²�����Ϊһ�����񴫵ݸ�ui�߳�ȥ���
*/

std::deque<std::packaged_task<void()>> tasks;
bool gui_shutdown_message_received();
void get_and_process_gui_message();
void gui_thread()
{
	while (!gui_shutdown_message_received())
	{
		get_and_process_gui_message();
		std::packaged_task<void()> task;
		{
			std::lock_guard lk{ mtx };
			if (tasks.empty())continue;
			task = std::move(tasks.front());
			tasks.pop_front();
		}
		task();
	}
}

template<typename Func>
std::future<void> post_task_for_gui_thread(Func f)
{
	std::packaged_task<void()>task{ f };
	std::future<void> res = task.get_future();
	std::lock_guard lk{ mtx };
	tasks.push_back(std::move(task));
	return res;
}

/*
	�����Ŀ���Ѿ����̳߳������һ���̲߳��ϵش����������ȡ����������
	һ���������������������push���񡣲��ҷ���future
	ʹ�á�package_task��future������ʹ�á�function��û�з���ֵ����Ϊ����
	��Ϊ����ʹ���߶��ԣ��������ڲ���Ҫ����ֵ�������Ƿ����Ҫ����ֵҲ��һ������
	���Ըɴౣ������ֵ����ء�����future<void>֧���޷���ֵ������Ҫֱ�Ӷ�������

	�ٺ�ǰ�ĵ��̳߳رȽϡ�ui��ܵ������ǲ�ͣ����ui�߳�ִ�С���Ϊ��Щ������Ϊ�˸���ui
	��ֻ��ui�߳��ܸ���ui
	�̳߳�������ǰ�����������̣߳�һ���������ж����ˣ���ȡ��������һ���߳�
	�����ǽ��������߳�ȥִ�е�

	package_task ʹһ��future �����˿�ִ�ж���ķ���ֵ
	�� promise ��ʹһ��future ������������
	promise<T> ����һ��T���͵ı����������� get_future ��ȡfuture
	�� promise������set_value�󣬸�future��׼������
*/

struct payload_type {};
struct data_packet { int id; payload_type payload; };
struct outgoing_packet { payload_type payload; std::promise<bool> promise; };
struct connection_t
{
	bool has_incoming_data()const;
	data_packet incoming()const;
	bool has_outgoing_data()const;
	outgoing_packet top_of_outgoing_queue()const;
	std::promise<payload_type>& get_promise(int)const;

	void send(payload_type)const;
};

using connection_set = std::set<connection_t>;
using connection_iterator = connection_set::iterator;
bool done(connection_set&);
void process_connections(connection_set& connections)
{
	while (!done(connections))
	{
		/*
			����ÿ������
			����������н������ݣ��򽫸����ݸ�����id��Ӧ��promise
			promise�����ӹ���ÿ��id��promise��Ӧ��promise��future�����������߳����ȴ�ʹ��
			һ�����ݽ��ܵ���������promise��ʱ��future����ȡ�ɹ��������̼߳���ִ��

			��������������ݷ������򽫸����ݷ������������ݶ�Ӧ��promise��true
			�˹��̿����������̷߳����߳�X��Ҫ�������ݣ����ڡ����ͳɹ��������ִ��
			����������ݽ������ӹ����̣߳����õ�һ��future������������������ʱ��future�õ�true
			�����߳�X����ִ��
		*/
		for (connection_iterator connection = connections.begin(); connection != connections.end(); ++connection)
		{
			if (connection->has_incoming_data())
			{
				data_packet data = connection->incoming();
				std::promise<payload_type>& p = connection->get_promise(data.id);
				p.set_value(data.payload);
			}

			if (connection->has_outgoing_data())
			{
				outgoing_packet data = connection->top_of_outgoing_queue();
				connection->send(data.payload);
				data.promise.set_value(true);
			}
		}
	}
}

/*
	���ֹ��ߵıȽ�
	��ȴ��໽�ѣ�notify_all
	��ȴ������ѣ�notify_one
	���߳��״�Ψһ�¼���call_once
	һ���ԣ�future
	��ȴ�һ���ԣ�shared_future
*/

/*
	4.4����ͬ���򻯴���

	1.����ʽ���
	���ǹܵ����ľ޴��������Ĵ������������Ӵ����Ա���Ϊ�˸�������
	Ȼ�󽻸��ܵ��еĶ������ȥ��ɡ�ÿ���������������һ������������
	ȱ�����ڣ�1.���ܴ�����Щ�������������ݾͿ��Լ���Ķ������ݣ���˿������ö��߳�
	2.�л������൱���л��������ģ����״̬���뱣�������������⣬����ȫ�ֱ��������Ա

	���ö��߳�ͬ�������������񽻸����̣߳�������future��������ֻ����������е�ĳ��ʱ��ȡfuture��ֵ����
	�����߼���ͬһƬ�������У��������ݿ����ɲ������ݶ�������ȫ�ֱ������������͵���
	���ֲ�����ȫ�ֱ����ĵ����������� ������ȫ���������ķ���ֵ��������Ķ��κ��ⲿ״̬�ֳ� ��pure����
	������������ʹ�õĺ�����ֻҪ����Ĳ�����ͬ���õ��Ľ��Ҳ��Ȼ��ͬ���������Ӱ��

	������Ϊ��
*/

template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input)
{
	if (input.empty())
	{
		return input;
	}

	std::list<T> result;

	result.splice(result.begin(), input, input.begin()); // ��input����Ԫ���ƶ��� result
	const T& pivot = *result.begin();
	auto divide_point = std::partition(input.begin(), input.end(), [&](const T& t) {return t < pivot;}); // ��С��pivot��Ԫ����������ǰ�棬���غ�벿�ֵĵ�һ��Ԫ�صĵ�����

	std::list<T> lower_part;
	lower_part.splice(lower_part.end(), input, input.begin(), divide_point);// ��ǰ�벿���ƶ���lp

	// �ݹ�������� 
	// ��lp�ƶ���sequential_quick_sort�Ĳ�����������ֵ����
	// ͬʱ���ܷ���ֵ����һ���µ�������sequential_quick_sort�ķ���ֵҲ��ֵ���ݵģ��Զ��ƶ�
	// ��������û�з����κο���
	auto new_lower(sequential_quick_sort(std::move(lower_part));

	// ����input��ʣ�µĺ�벿����ͬ���Ĳ���
	auto new_higher(sequential_quick_sort(std::move(input));

	// ��ǰ�벿���ƶ���resultǰ�棬��벿���ƶ���result���档��pivot��ǰ�Ѱ�����result�
	result.splice(result.begin(), new_lower);
	result.splice(result.end(), new_higher);

	return result;
}

/*
	ע�⵽��������ǰ�벿��ʱ����������벿�֣������ǿ��������̴߳���ǰ�벿�֣�ͬʱ��һ���̴߳����벿��
	���߳��������ʱ��future.getȡ�ú�벿�ֵĽ������

*/

template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input)
{
	if (input.empty())
	{
		return input;
	}

	std::list<T> result;

	result.splice(result.begin(), input, input.begin());
	const T& pivot = *result.begin();
	auto divide_point = std::partition(input.begin(), input.end(), [&](const T& t) {return t < pivot;});

	std::list<T> lower_part;
	lower_part.splice(lower_part.end(), input, input.begin(), divide_point);

	// �봮�а汾���Ψһ��֮ͬ������ǰ�벿�������߳������򣬲��ҽ����future��
	std::future<std::list<T> new_lower(std::async(&parallel_quick_sort, std::move(lower_part));

	auto new_higher(parallel_quick_sort(std::move(input));

	result.splice(result.begin(), new_lower.get());// ��future����ȡ
	result.splice(result.end(), new_higher);

	return result;
}

/*
	����package_task��future����ʵ��һ����Լ�ĺ���
	�����ִ�ж���Ͳ���������future������Ҫ��ʱ���ٴ�future���ȡ����ֵ
*/

template<typename F, typename A>
std::future<std::invoke_result_t<F(A&&)>> spawn_task(F&& f, A&& a)
{
	using result_t = std::invoke_result_t<F(A&&)>;
	std::packaged_task<result_t(A&&)> task{ std::move(f) };

	auto res = t.get_future();

	std::thread t{ std::move(task), std::move(a) };// packaged_task Ҳ�ǿ�ִ�ж�������Ҳ�����ڹ���thread
	t.detach();
	return;
}