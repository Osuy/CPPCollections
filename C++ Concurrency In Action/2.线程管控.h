#pragma once
#include <thread>
#include <string>
#include <functional>

/*
	2.1基本管控
	每个C++程序至少有一个线程，即main线程，由C++运行时系统启动
	其他线程以其他函数为入口。不一定是函数，任何可执行对象都可以作为入口
	main函数renturn时，main线程结束，程序退出
	入口函数return时，其他线程结束
	std::thread 管控线程

	线程在 std::thread 构造时开始执行

	匿名可执行对象作为 std::thread 的参数时，会解析为函数声明，所以建议用{}来构造
*/

struct func_obj
{
	void operator()() {}
};

void test1()
{
	// 被解析为函数声明， 函数名t1，返回值std::thread，参数是一个函数指针（返回值func_obj，无参）并且是匿名参数
	std::thread t1(func_obj()); 
	
	// 增加一对圆括号使其能解析为对象
	std::thread t2((func_obj())); 
	
	// 新式初始化，不会被解析为函数声明
	std::thread t3{ func_obj{} };

	//t1.join(); // t1 不是对象
	t2.join();
	t3.join();
}

/*
	std::thread对象构造后，线程已经开始执行，但是在对象析构之前，必须要决定汇合或是分离
	例如，一个局部的 std::thread 对象，执行一个复杂的函数，并且在离开作用域时，线程还未结束
	但是此时，局部的std::thread对象要被析构。析构函数发现线程没有汇合或分离，就会调用std::terminate终止整个程序
		汇合：调用join，等待线程结束
		分离：调用detach，不等待，继续执行本线程，即便std::thread对象已经析构

	join看起来没用？
	确实多数情况下会使用detach，本线程和新线程同时运行
	如果本线程等待新线程执行完再继续，那么等待的过程中本线程本身不做事
	但是如果本线程是发起了多个线程分配任务同时进行，并在所有任务完成后才继续
	那么就应该使用join

	
	如果一个线程以分离的方式运行，而它里面使用了其他线程的局部变量的引用，但是不能保证该引用的对象生命周期是否结束
	如果使用了该引用，就会发生未定义行为
*/

void test2()
{
	int local_integer = 0;

	std::thread t1{ [&local_integer]() {} }; 
	t1.detach();
	// t1分离，test2返回，局部变量local_integer释放，但是t1线程里引用了local_integer
	// 使用local_integer会发生未定义行为
	// 如果t1 join，则不会有这种情况，因为test2必定在t1线程结束后才return
}

/*
	线程异常
	由于std::thread对象在析构前必须汇合或分离
	那么一段手动汇合或分离的代码无论写在何处都要面临一个问题：
		在它之前如果发生异常，那么就不会分离或汇合，但是std::thread对象会析构
	于是这个异常即便能被抛出处理，但是总是会因为std::thread的析构函数调用了std::terminate而终止程序
	解决办法是把线程对象的汇合看作是资源，由资源管理类负责
	不应该使用分离，让发生异常后的线程还继续执行，因为该线程可能会引用本线程的变量。因此最好汇合
	让线程安全结束
*/

struct thread_guard
{
	std::thread& t;
	thread_guard(std::thread& T) :t{ T } {}

	// 禁止复制，因为复制可能引发多次汇合（但是析构里已经判断了joinable，所以复制也没事）
	thread_guard(const thread_guard&) = delete;
	thread_guard& operator=(const thread_guard&) = delete;
	~thread_guard()
	{
		if (t.joinable())
		{
			t.join();
		}
	}
};

void test3()
{
	std::thread t{ []() {} };
	thread_guard tg{ t };

	// to other thing
	// 此时即便发生异常，tg析构会汇合t线程，避免终止程序
}

/*
	分离后，std::thread对象与线程再无关联，不可汇合。线程的归属权和控制器都转移给C++运行时库
	这些线程被叫做守护线程，通常会长时间在后台运行

	同样，必须joinable才能调用detach
	*/
	
	
/*
	2.2向线程函数传参
	首先明确一件事：向线程函数传递参数 和 向std::thread的构造函数传递参数 是两件不同的事

	std::thread 的构造函数接受任意个参数，并且会把除第一个参数（入口函数）传入入口函数
	因为std::thread的构造是一个变参函数模板
	但既然是函数模板，意味着它只会一五一十地把实参推导

	但是：入口函数的参数可以是string，而向std::thread构造传入的可以是一个const char*
	这是为何？
*/


void test4()
{
	// 传入入口函数和const char*，入口函数有一个string类型的参数
	// 如果第二参数是不能隐式转型为string的变量，则编译失败
	std::thread t1{ [](std::string) {}, "void func" }; 
	t1.join();

	//std::thread t2{ [](std::string) {}, 1.f }; // error：未找到匹配的重载函数
	//t2.join();
}

/*
	这是因为 std::thread 构造函数会拷贝传入的所有变量，再将它们传入入口函数
	而不是在模板推导时，检查传参类型和入口函数类型是否一一对应
	所以编译失败的报错是：invoke未找到匹配的重载函数，而不是函数模板推导失败
	
	由于const char* 可以构造string，所以调用入口函数时，会构造出匿名string传入

	但是，假如参数类型是char*，并且指向的字符串有可能在入口函数执行前已经结束生命周期
	那么后续 用char* 构造string就会发生未定义行为

	这种错误的条件是：1.使用的指向局部变量的指针；2.线程分离
*/

void test5()
{
	char buffer[1024];
	sprintf_s(buffer, "%i", 1024);

	// 线程运行时，test4可能已经结束，此时buffer指针悬垂，构造string就会是未定义行为
	std::thread t1{ [](std::string) {}, "void func" };
	t1.detach();
}

/*
	因此依然建议 传入 std::thread 构造的参数类型和入口函数的参数类型完全匹配

	std::thread 的构造拷贝参数还有一个特点，就是全部会是临时变量，传入入口函数时，实参的类型会是右值
	这对值传递没有影响，但是对于引用传递，会发生编译错误：未找到匹配的重载函数
*/

void test6()
{
	std::string str{ __func__ };
	std::thread t1{ [](std::string&) {}, str }; // error：未找到匹配的重载函数
	t1.detach();

	// 这种情况和bind类似，不同之处在于bind拷贝的变量会保存在可执行对象内部，所以实参参不是右值
	// bind绑定的函数可以传引用，不会有编译错误，但达不到预期
	// 因为引用的是可执行对象内部的变量，而不是外部的目标变量
	auto op1 = std::bind([](std::string&) {}, str); // 传入str到bind，但是最终会拷贝到可执行对象内
	op1();

	// 解决办法是使用std::ref
	std::thread t2{ [](std::string&) {}, std::ref(str) };
	t2.detach();

	auto op2 = std::bind([](std::string&) {}, std::ref(str)); // 传入str到bind，但是最终会拷贝到可执行对象内
	op2();
}

/*
	以成员函数为入口函数，则第二参数必须是对象指针，入口函数的参数从第三个开始计数
*/

void test7()
{
	struct Aop
	{
		void func(int) {}
	};

	Aop obj;
	std::thread t1{ Aop::func, &obj, 1 }; // 第二参数是对象指针，第三参数开始才是传参
}

/*
	std::thread 的构造留了这么一个大坑给我们：
		1.复制所有参数
		2.以右值的形式传入入口函数
	它导致了要么使用std::ref传给std::thread构造，面对可能的生命周期结束的风险
	要么入口函数的参数使用值传递，面对拷贝复杂对象的额外开销

	好在，C++11引入了移动语义，它可以通过转移所有权来避免拷贝
	移动语义要配合值传递，因为值传递才会构造形参（左值/右值引用传递不会构造形参）
	由于实参是右值，形参构造时会匹配到移动构造函数，从而发生移动
*/

void test8()
{
	std::string str{ __func__ };
	std::thread t1{ [](std::string) {}, std::move(str) };// 入口函数需为值传递
	t1.detach();

	/*
		当std::thread 拷贝 std::move(str)，由于是个右值，所以拷贝会直接挪用其动态内存
		于是外界的str已经被“掏空”，不能再被使用，而str的数据已被移动到std::thread构造里
		再然后传递给入口函数，虽然形参是值传递，但是通过临时对象构造，发生移动，临时变量的动态内存直接挪动到形参里
		整个过程中没有发生真正的拷贝，只是多了两次移动
	*/
}

/*
	2.3 std::thread 本身也支持移动，所以管理 std::thread 对象并非如之前的 thread_guard 那样使用 std::thread& 的方式
	如果一个函数创建一个std::thread并返回给调用方来负责，或者不创建std::thread对象，而是由参数传入
	都可以算作是转移std::thread的归属权，这种情况std::thread&并不能实现
	实际上大可直接使用std::thread即可，需要转移的地方，使用std::move来匹配移动构造，实现转移
*/