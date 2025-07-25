#pragma once
#include <mutex>
#include <queue>
#include <future>
#include <set>
#include <list>
#include <type_traits>

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
	单线程一次性同步事件 std::future更适合此场景

	std::future 与一个事件关联，无法重置，唯有该事件完成，它才可读得结果
	它克服了线程没有返回值的缺点
	不过，std::future的模板参数可以是void，即便不用返回值，一次性同步也是future更合适
	
	一个线程若与另一个线程需要同步，并且该同步是一个一次性事件。可以让线程2的事件关联到线程1的一个future
	然后线程1访问future中的结果。如果如果事件尚未完成产生结果，则会阻塞
	如果并不是立刻需要结果，可以持有future去做其他事，知道需要时再访问
	std::future 自身用于线程同步，但它本身如果被多线程访问，依然需要互斥保护
	这种情况可以用另一种形式替代。一个事件可以与多个std::shared_future关联。
	于是多个线程都可以读得事件的结果而不会造成竞争

	可以使用std::async来构造future对象，它和thread相似，不过允许可执行对象有返回值
*/

int find_the_answer_to_ltuae();

void example2()
{
	std::future<int> answer = std::async(find_the_answer_to_ltuae);
	// do other thing
	answer.get(); // 如果未计算完成，会阻塞
}

/*
	async 和thread相似，接受可执行对象
	可以是函数指针，成员函数，仿函数，lambda表达式等等
	对于其他参数，也同样存在拷贝和移动两种处理，而不支持引用，需用std::ref替代
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
	auto f1 = std::async(A1::foo, &a1, 42, "hello"); // 传入对象地址，以p->foo形式调用
	auto f2 = std::async(A1::bar, a1, "goodbye");// 对象值传递，以对象的拷贝tmp.bar形式调用

	A2 a2;
	auto f3 = std::async(A2(), 3.141); // 传入临时对象，发生移动，以对象的移动拷贝 tmp()形式调用
	auto f4 = std::async(std::ref(a2), 3.141); // 传入a2的ref，以 a2()形式调用

	auto f4 = std::async(A3{}); // 传入临时对象，发生移动，以对象的移动拷贝 tmp()形式调用
}

/*
	除了async，promise和package_task也可以获取 future
	package_task 和 function类似，也是一个可执行对象。区别在于：
		function调用时立刻获得返回值，和函数一样
		package_task 调用后，返回值保存于future中。future可以用get_future获取。
		package_task 的常规用法是创建后，获取并保存它的future，然后再把它移动给另一个线程去运行
		本线程在需要时获取future里的值

	许多软件的ui框架都会运行在专门的线程上，别的线程若要更新ui，需向ui线程发送消息
	package_task可以实现此类框架，并且避免为每个消息定义专有的类型
	所有消息都由枚举类定义。向ui线程发生枚举值以及其他参数，通知ui更新。但是新的操作
	就需要增加新的枚举值，可扩展性不强，还会使其他文件依赖枚举，是模块耦合
	而使用package_task，将更新操作作为一个任务传递给ui线程去完成
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
	这样的框架已经和线程池相近，一个线程不断地从任务队列里取出任务并运行
	一个函数负责向任务队列里push任务。并且返回future
	使用“package_task和future”比起使用“function且没有返回值”更为优秀
	因为对于使用者而言，哪怕现在不需要返回值，将来是否会需要返回值也是一个问题
	所以干脆保留返回值的余地。并且future<void>支持无返回值，不需要直接丢弃即可

	再和前文的线程池比较。ui框架的任务是不停地由ui线程执行。因为这些任务都是为了更新ui
	而只有ui线程能更新ui
	线程池则是提前创建了若干线程，一旦任务中有队列了，就取出并唤醒一个线程
	任务是交给其他线程去执行的

	package_task 使一个future 关联了可执行对象的返回值
	而 promise 则使一个future 关联变量本身
	promise<T> 代表一个T类型的变量，可以用 get_future 获取future
	当 promise调用了set_value后，该future就准备就绪
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
			遍历每个链接
			如果该链接有接收数据，则将该数据赋给其id对应的promise
			promise被链接管理，每个id与promise对应，promise的future可能在其他线程正等待使用
			一旦数据接受到，就设置promise，时其future被读取成功，所在线程继续执行

			如果该链接有数据发生，则将该数据发生，并将数据对应的promise设true
			此过程可能由其他线程发起。线程X需要发送数据，并在“发送成功”后继续执行
			因此它将数据交给链接管理线程，并得到一个future，当数据真正被发送时，future得到true
			于是线程X继续执行
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
	四种工具的比较
	多等待多唤醒：notify_all
	多等待单唤醒：notify_one
	多线程首次唯一事件：call_once
	一次性：future
	多等待一次性：shared_future
*/

/*
	4.4运用同步简化代码

	1.函数式编程
	考虑管道风格的巨大计算任务的处理，由于任务庞大，所以被拆为了各个部分
	然后交给管道中的多个部件去完成。每个部件的输出是上一个部件的输入
	缺点在于：1.可能存在有些不依赖其他数据就可以计算的独立数据，因此可以利用多线程
	2.切换部件相当于切换了上下文，因此状态必须保存于上下文以外，例如全局变量或类成员

	利用多线程同步，将计算任务交给多线程，并存留future，主函数只需在任务进行到某步时获取future的值即可
	所有逻辑在同一片上下文中，所需数据可以由参数传递而不依赖全局变量，更易理解和调试
	这种不依赖全局变量的单个函数，其 作用完全限制于它的返回值，而不会改动任何外部状态又称 纯pure函数
	纯函数是最易使用的函数。只要传入的参数相同，得到的结果也必然相同，不受外界影响

	已排序为例
*/

template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input)
{
	if (input.empty())
	{
		return input;
	}

	std::list<T> result;

	result.splice(result.begin(), input, input.begin()); // 将input的首元素移动到 result
	const T& pivot = *result.begin();
	auto divide_point = std::partition(input.begin(), input.end(), [&](const T& t) {return t < pivot;}); // 将小于pivot的元素整理到链表前面，返回后半部分的第一个元素的迭代器

	std::list<T> lower_part;
	lower_part.splice(lower_part.end(), input, input.begin(), divide_point);// 将前半部分移动给lp

	// 递归调用自身 
	// 将lp移动给sequential_quick_sort的参数，参数是值传递
	// 同时接受返回值构造一个新的链表，而sequential_quick_sort的返回值也是值传递的，自动移动
	// 整个过程没有发生任何拷贝
	auto new_lower(sequential_quick_sort(std::move(lower_part));

	// 对于input里剩下的后半部分做同样的操作
	auto new_higher(sequential_quick_sort(std::move(input));

	// 将前半部分移动到result前面，后半部分移动到result后面。（pivot此前已包含在result里）
	result.splice(result.begin(), new_lower);
	result.splice(result.end(), new_higher);

	return result;
}

/*
	注意到，在排序前半部分时并不依赖后半部分，所以是可以在主线程处理前半部分，同时另开一个线程处理后半部分
	主线程排序完成时，future.get取得后半部分的结果即可

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

	// 与串行版本相比唯一不同之处在于前半部分在新线程里排序，并且结果在future中
	std::future<std::list<T> new_lower(std::async(&parallel_quick_sort, std::move(lower_part));

	auto new_higher(parallel_quick_sort(std::move(input));

	result.splice(result.begin(), new_lower.get());// 从future里拿取
	result.splice(result.end(), new_higher);

	return result;
}

/*
	利用package_task和future可以实现一个简约的函数
	传入可执行对象和参数，返回future，在需要的时候再从future里获取返回值
*/

template<typename F, typename A>
std::future<std::invoke_result_t<F(A&&)>> spawn_task(F&& f, A&& a)
{
	using result_t = std::invoke_result_t<F(A&&)>;
	std::packaged_task<result_t(A&&)> task{ std::move(f) };

	auto res = t.get_future();

	std::thread t{ std::move(task), std::move(a) };// packaged_task 也是可执行对象，所以也能用于构造thread
	t.detach();
	return;
}