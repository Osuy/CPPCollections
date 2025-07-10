#pragma once
#include <mutex>
#include <list>
#include <stack>

/*
	3.1共享数据的问题

	假设有两个线程读写同一个链表，其中一个在遍历，而另一个在删除某个节点
	删除一个迭代器，意味着要重新调整前驱、后继节点的指针，因此会影响三个节点
	如果A线程访问的迭代器不是这三个节点的迭代器，那么访问正常，如果是，或者即将在删除之前访问
	（删除一个节点所执行的指令数量显然大于读取，因此可能删除完成之前就遍历到了）
	那么就会得到错误的遍历结果
	（一般而言遍历总是正序的，因此，如果先调整前驱节点的后继指针，再调整后继节点的前驱指针
	那么遍历结果也是正确的，但是如果逆序访问，则是错误的
	所以建议调整指针的循环是先前驱后后继）

	上例也揭示了一个线程同时访问资源的错误的必要条件
	写操作是一个非原子操作，可能有多个步骤。而在所有步骤完成之前，资源出于非法状态
	此时却被其他线程访问，得到了一个非法的结果
	而如果写操作是原子操作，单指令立即完成，就不会有发生错误的机会

	资源竞争难以复现而且难以察觉，因为计算机完全认为那是正确的，但结果不符合人类的预期
	并且可能在调试时又恢复正常，因为调试环境和实际运行环境有些许差异，使资源竞争的错误没有发生

	3.2互斥保护
	最通用的共享保护
	只有请求互斥成功后才能继续执行，否则阻塞。一个线程如果请求成功，那么其他线程只能等待
	mutex 的 lock 必须和 unlock 配合使用。但是更建议使用标准库提供的raii类lock_guard去管理

	同样，对于链表，为了禁止两个线程同时竞争，可以对读写操作互斥
*/

std::mutex mtx;
std::list<int> l;
void read_list()
{
	std::lock_guard<std::mutex> guard{ mtx };
	std::find(l.begin(), l.end(), 1);
}

void write_list()
{
	std::lock_guard<std::mutex> guard{ mtx };
	l.push_back(1);
}

/*
	互斥全局变量mtx在以上两个函数里对链表l进行互斥保护
	然而对于其他链表，也需要声明同样的互斥变量
	因此更好的做法是将互斥变量和要保护的资源封装为一个类
	这样每个互斥变量都能保护与它关联的资源

	如果函数返回了资源的指针或引用，或者使用了由外界传入的可执行对象，那么即使锁住了互斥，也只能保证
	资源在本作用域内被保护，而资源在外界的读写处于未保护状态
*/

class some_data
{
	int a;
	std::string b;
public:
	void do_something() {}
};

class data_wrapper
{
	some_data data;
	std::mutex m;
public:
	template<typename Function>
	void process_data(Function func)
	{
		std::lock_guard l{ m };
		func(data);
	}
};

some_data* unprotected;

void malicious_function(some_data& protected_data)
{
	// 将参数所引用的data的地址提取给全局变量
	unprotected = &protected_data;
}

data_wrapper x;
void foo()
{
	// x 调用 process_data。data的地址被外界保存下来，于是可以绕过锁去修改
	// 这不仅破坏了互斥保护，也破坏的封装。一个类的私有成员被外界获取到了地址
	// 于是可以被任意修改而不被对象感知
	x.process_data(malicious_function);
	unprotected->do_something();
}

/*
	考虑stack的代码
	一种习惯是，在调用top或pop之前先对stack判空。因为对空栈top或pop会导致未定义行为
	然而由于一个简单的操作被分为了两个语句，所以在判空和读取之间，其他线程可能会pop
	于是即便通过判空，pop和top仍然是不可靠的
*/

void test()
{
	std::stack<int> s;
	if (!s.empty())
	{
		// 在empty和top之间，可能其他线程执行了pop，导致不安全
		auto t = s.top();
		// 在top和pop之间，其他线程执行了pop，那么栈pop了两次，然而两个线程的变量t都等于旧栈顶。这是更隐蔽的错误
		s.pop();
	}
}

/*
	那为何标准库的接口要设计成这样呢？
	因为它抛弃线程安全，而选择了更为需要的另一个安全：数据安全
	假设pop删除栈顶，并返回栈顶元素的值，就可以把pop和top合为一句，减少多线程错误的风险
	然而返回值在复制的过程中可能会抛出异常，此时栈顶已被弹出，如果要复原，就要重新入栈
	而入栈所需要的那个栈顶在复制时因为异常已经丢失了。即：栈不可能复原
	所以stack不得不一分为二，避免异常导致数据永久丢失

	假如自己设计stack，考虑线程安全，该如何设计接口？
	pop返回值的痛点在于对象的拷贝发生在pop函数外，并且可能会抛异常。一旦抛异常，数据永久丢失
	所以可以：
		1.把拷贝移动到pop函数内，用非const引用参数传入来拷贝。但是缺点是外界必须提前构造好一个用于拷贝的对象。即会调用一次无意义的构造
		2.要求T类型本身的拷贝构造和移动构造不抛异常。限制较大，要求使用者仔细阅读注释或手册。把出错的风险留给了使用者，不是一个好的设计
		3.返回值为指针。指针的复制是不会抛异常的。但是涉及到额外的内存管理。因此可以使用智能指针（智能指针的复制和移动也是不抛异常的）

	结合1、3。将pop重载，以便使用者自主选择使用何种方法
*/

template<typename T>
class my_stack
{
	// 占位enum，用于重载pop区分不同版本
	enum class p2{ _2 };
	enum class p3{ _3 };
	enum class p4{ _4 };
public:
	// 1.引用出参
	// 优点：把复制行为放在pop内部，如果发生异常，不执行pop。保证数据不丢失
	// 缺点：外界必须提前构造好一个T对象；T必须支持复制
	void pop(T& out);

	// 2.T提供不抛异常的拷贝构造或移动构造
	// 利用 std::nothrow_copy_constructor 和 std::nothrow_move_constructor
	T pop(p2 = p2::_2);

	// 3.返回指针，并且是智能指针，减少内存管理的责任歧义
	// 指针/智能指针的复制不会抛异常，所以不会有数据丢失的风险
	std::shared_ptr<T> pop(p3 = p3::_3);
};

struct empty_stack : std::exception
{
	const char* what()const noexcept;
};

template<typename T>
class my_stack2
{
	std::stack<T> data;
	mutable std::mutex m;
	friend void test2(my_stack2& s1, my_stack2& s2);
	friend void test3(my_stack2& s1, my_stack2& s2);
	friend void test4(my_stack2& s1, my_stack2& s2);
public:
	my_stack2() = default;
	my_stack2(const my_stack2&)
	{
		// 不使用初始化列表，而在函数体内复制。让复制行为被互斥保护
		std::lock_guard lock{ m };
		data = other.data;
	}

	my_stack2& operator = (const my_stack2&) = delete;
	
	void push(T new_value)
	{
		std::lock_guard lock{ m };
		data.push(std::move(new_value));// 直接把形参移动到容器内
	}

	std::shared_ptr<T> pop()
	{
		std::lock_guard lock{ m };
		if (data.empty())
		{
			throw empty_stack{};
		}

		const std::shared_ptr<T> ret{ std::make_shared<T>(data.top()) };// 如果复制失败， 下文的pop不会执行
		data.pop();
		return ret;
	}

	void pop(T& value)
	{
		std::lock_guard lock{ m };
		if (data.empty())
		{
			throw empty_stack{};
		}

		value = data.top(); // 如果复制失败， 下文的pop不会执行
		data.pop();
	}
	bool empty()const;
};

/*
	stack由于只有栈顶可以访问，因此一个互斥锁住整个容器。
	但是对于其他容器，这个锁的精度就太低了。
	就比如链表，删除链表的某个节点，只需要锁住三个节点。更细化的精度，只锁住这三个节点的逆序读写所

	细化锁的精度是重要的优化手段，可以提高数据的使用效率。然而也带来了一个问题：死锁

	任何需要使用两个以上的互斥的情况，都有可能出现死锁
	防范死锁
		1.尽可能只使用一个锁。死锁只发生在多个锁的情况，一个锁永远不可能死锁
		2.避免在加锁后的代码中调用参数提供的接口。防止用户私自获取数据地址，也能防止另外加锁
		3.永远以相同的次序上锁。有局限，因为上锁次序本身可能经过刻意设计，无法随意更改
		又或者上锁次序取决于运行时，无法在编译器知晓

	例如一个函数，它接收两个对象，并在内部按次序上锁。看似遵守了相同次序上锁的原则，
	但是只要两个线程传参时次序相反，就会发生死锁
*/

void test2(my_stack2<int>& s1, my_stack2<int>& s2)
{
	// 先锁住参数1，再锁住参数2
	std::lock_guard lock1{ s1.m };
	std::lock_guard  lock2{ s2.m };

	// 如果两个线程分别传递两个对象，但是次序相反，则发生死锁
}

/*
	标准库提供了一次锁住多个互斥的方法 std::lock，并且没有死锁风险
	std::lock要么成功全部加锁，要么失败全不加锁
	但是该方法不是raii类，所以依然需要手动解锁
	并且由于可能抛异常，所以即便仔细手动解锁，仍然会在异常时没有解锁
	为了避免遗漏解锁，使用raii类依然是最佳手段。好在std::lock_guard 支持基于已经上锁的互斥进行构造
	因为已经加锁的互斥再次加锁会导致未定义行为，而lock_guard的构造会默认加锁
	因此还需要第二参数来匹配重载，使调用不加锁的版本
	如此依然可以使用raii类自动解锁
*/

void test3(my_stack2<int>& s1, my_stack2<int>& s2)
{
	// 一次对多个互斥加锁
	std::lock(s1.m, s2.m);

	// 使用第二参数 std::adopt_lock 调用不加锁版本的构造
	// 并且 即便后文抛异常，raii类也能自动解锁
	std::lock_guard lock1{ s1.m, std::adopt_lock };
	std::lock_guard lock2{ s2.m, std::adopt_lock };
}

/*
	更进一步，标准库提供了另一个raii类 scoped_lock
	它是变参模板，可以用多个互斥来构造，效果等同于 std::lock + std::lock_guard
*/

void test4(my_stack2<int>& s1, my_stack2<int>& s2)
{
	// 直接用一个raii类锁住多个互斥
	std::scoped_lock lock{ s1.m, s2.m };
}

/*
	此外，关于上锁的顺序，再以链表为例
	正向读取链表时，已经锁住了自己，要去锁住后继节点，并且解锁前驱节点。同一时刻只需要锁住两个节点。逆序同理
	如果有两个线程分别正序和逆序读取链表，那么一定会在某两个节点上发生死锁：
		一个线程已经锁住A，将要锁住B，并解锁A，读取B
		另一个线程已经锁住C，将要锁住B，并解锁C，读取B
		其中某个线程会先锁住B，另一个线程等待B的锁。假设正序获得了B的锁
		那么正序线程解锁A，读取B，然后继续遍历，试图获取C的锁
		而C的锁被逆序线程获取，并且它正在阻塞，等待B的锁。
		即一个线程锁住B，等待C；另一个线程锁住C，等待B

	同理。正逆序读取，也会和删除节点发生死锁
	因为死锁必须获取到三个节点的锁。而遍历需要获取两个锁，于是（可能）会在某两个节点上发生死锁
	但是删除和遍历可能时机会错开，不一定会死锁。而正逆序遍历则时一定会在某个时机相遇，发生死锁
*/

/*
	另类死锁
	互斥本质是阻塞线程。除了互斥能造成死锁，其他的任何形式的阻塞也都可能造成死锁
	例如join就是阻塞本线程，等待目标线程结束。假如两个线程互相join，就会发生死锁
*/

/*
	防范死锁之4：按层级加锁
	一种更宽松的顺序加锁。要求必须按照从高到低层次去加锁

	每个锁都定义在不同的层级，在加锁前，确保current_hierarchy是严格小于该锁的层级，否则就抛出异常使加锁失败
	而在加锁后，就要更新current_hierarchy使后继其他锁的加锁能有效判断

	而在解锁时，需要把current_hierarchy复原到之前的层级。于是需要一个last_hierarchy保存加锁前的层级
	由于锁只能被加一次，也只能被复原一次，所以last_hierarchy可以作为锁的成员变量

	但是对于多个线程，每个线程都可以试图加一个锁，而必须是每个线程的current_hierarchy去和锁的层级比较
	所以 current_hierarchy是一个线程独有变量。可以用 static thread_local 来修饰
	static 表示变量不是成员而是静态。thread_local表示每个线程都会有一个副本

	线程静态变量是层级加锁的主要手段
*/

class hierarchical_mutex
{
	std::mutex internal_mutex;
	unsigned long const hierarchy;
	unsigned long last_hierarchy;
	static thread_local unsigned long current_hierarchy;

	void check_for_hierarchy_violation()
	{
		// 加锁前调用，检查当前层级是否锁层级
		// 如果锁层级高于当前线程层级，则加锁失败（同一层级也失败）
		if (current_hierarchy <= hierarchy)
		{
			throw std::logic_error{ "mutex hierarchy  violated" };
		}
	}

	void update_hierarchy()
	{
		// 加锁后调用，更新旧层级和当前层级
		// 旧层级 = 当前线程层级，当前线程层级 = 锁层级
		last_hierarchy = current_hierarchy;
		current_hierarchy = hierarchy;
	}

public:
	explicit hierarchical_mutex(unsigned long value)
		: hierarchy{ value }
		, last_hierarchy{ 0 }
	{}

	void lock()
	{
		check_for_hierarchy_violation();
		internal_mutex.lock();
		update_hierarchy();
	}

	void unlock()
	{
		if (current_hierarchy != hierarchy)
		{
			throw std::logic_error{ "mutex hierarchy violated" };
		}

		current_hierarchy = last_hierarchy;
		internal_mutex.unlock();
	}

	bool try_lock()
	{
		check_for_hierarchy_violation();
		if (!internal_mutex.try_lock())
		{
			return false;
		}

		update_hierarchy();
		return true;
	}
};

// 当前线程层级初始从最大值开始，可以逐步往低层级加锁
thread_local unsigned long hierarchical_mutex::current_hierarchy(ULONG_MAX);

/*
	unique_lock
	支持：1.自动上锁；2.已加锁的锁；3.不上锁，等待需要时调用lock
	其内部保存着锁的状态。如果自动上锁，则状态为占有，析构时需要解锁
	若关联已上锁的锁，同理
	若关联未上锁的锁，且始终未调用lock，也就未占有锁，析构也不会去解锁
*/

void test5()
{
	std::mutex m1;
	std::unique_lock u1{ m1 };//自动上锁

	std::mutex m2;
	m2.lock();
	std::unique_lock u2{ m2, std::adopt_lock };// 管理已经上锁的锁

	std::mutex m3;
	std::unique_lock u2{ m2, std::defer_lock };// 管理未锁的锁，也不自动上锁。待需要时上锁
	u2.lock();
}

/*
	在不同作用域中转移互斥
	典型的例子是一个函数对互斥上锁，并把锁返回给调用者。这样不仅保护了函数内部，也保护了调用处
*/

std::unique_lock<std::mutex> prepare()
{
	std::unique_lock lk{ mtx };
	// do some prepare
	return lk;
}

void process()
{
	auto lk{ prepare() };
	// do some thing
}

/*
	选择合适粒度加锁
	粒度越大，越容易阻塞；
	在保护必要数据的同时，尽可能做到粒度小
	在同一个函数里，使用互斥保护数据时，保护范围是整个作用域
	但是并非整个作用域都在访问数据
	尽可能只在访问数据时上锁
*/

void test6()
{
	std::unique_lock<std::mutex> lk{ mtx };
	// do 访问数据
	lk.unlock(); // 不访问时解锁，让其他线程能够进入
	// do other thing
	lk.lock(); // 再次访问数据，上锁
}

/*
	上锁期间内不要进行耗时操作。例如io。因为io几乎总是比cpu慢上万被
	访问io其实约等于阻塞，不需要上锁
*/

/*
	3.3保护共享数据的其他手段

	保护共享初始化
	假如有一个对象，它会被多个线程访问。初始未空，第一个线程访问时，将其初始化
	如此就涉及到一个问题，如果有两个线程同时访问，就会发生两次初始化。并且总是丢弃第一个初始化的对象
	C++11以前的单例懒汉模式（饿汉模式会在程序开始时立刻初始化，线程访问时已经能直接使用）
	有一种粗糙的解决办法，双重检验锁定 double check locking pattern/ dclp
*/

/*
	如果不考虑多线程安全：
		只存在一个if
		一个线程进入if内，并初始化G_a；但是在G_a被赋值之前，可能另一个线程也判断了if，然后进入
		于是第二次初始化G_a，于是第一个初始化的对象就发生泄露

	初步考虑线程安全：
		由于一个线程在初始化G_a期间，应该禁止其他线程访问G_a，所以应该在if的前面加锁，阻止它们用正在初始化的G_a来if判断
		但是这个位置的锁会锁住if的上一层作用域，粒度非常大。它并不是锁住了“G_a的初始化”，而是锁住了“G_a的初始化所在的作用域”
		假如A只有初始化需要考虑线程安全，而在使用时不需要考虑
		也就是说，这个if前的锁，使所有对G_a的访问，都不得不加上一个毫无意义的锁
		（另一种解释：把锁放入if内，只锁住new语句。但是问题的矛盾在于会有多个线程进入if。
		即便锁住了new。另一个进入if的线程会在得到锁之后再次new。于是又需要在new之前用if判断是否已经初始化）
		（二者的解释都会引向一个结果：dclp）
	
	dclp
		为了解决锁粒度过大的问题，可以把锁和if放入到一个作用域里，防止干涉外界的“非初始化”操作
		那么这个新的作用域是？
		当然是“第一次初始化的作用域”啦
		于是进入这个作用域的条件是 if (!G_a)
		也就是说 if (!G_a)进行了两次。第一次是判断是否需要初始化；第二次是判断是否已经被初始化
		（可是为了防止其他线程在创建时访问，才在if前加锁，现在又在锁前再加了if。）

*/

class A {};
A* G_a = nullptr;
void example_dclp()
{
	if (!G_a)
	{
		std::unique_lock lk{ mtx };
		if (!G_a)
		{
			// 如果已经被其他线程初始化了，则不进入
			G_a = new A{};
		}
	}

	G_a; // 调用A的函数
}
/*
	dclp的缺点
	假如两个线程是为了调用共享对象的某个函数
	那么dclp会让第二个线程的调用建立在“第一次调用的结果”之上
	那不一定是正确的。
	dclp的问题的根源是：它在锁外读取了锁内要写入的数据

	C++标准库提供了一个能确保仅被调用一次的方法 call_once 和 once_flag
	once_flag是一个状态，它作为call_once的第一参数，只能被修改一次
	如果希望多个线程中的某一段代码只被执行一次，其余代码每个线程照常执行
	可以把这部分代码封装未可执行对象，并交给 call_once 去调用
*/

std::once_flag G_a_flag;
void prepare_G_a()
{
	G_a = new A{};
}

void do_some_thing_by_G_a()
{
	std::call_once(G_a_flag, prepare_G_a);
	G_a; // 调用A的函数
}