#pragma once
#include <vector>

/*
	函数重载匹配优先级：
		1.可以完全匹配非模板函数
		2.不隐式转型下可匹配的函数模板
		3.不隐式转型下存在多个可匹配的函数模板时，选择最具体的一个
		4.没有匹配的函数模板时，考虑隐式转型后可以匹配的非函数模板
		5.可变长参数函数

	当没有完全匹配，又没有函数模板时，会发生隐式转型
	此时如果存在多个转型后可用的重载，就会发生ambiguous多义性报错：有多个实例与参数列表匹配
	数值类型中，double不会和其他类型产生多义歧义
	（存在float和int形参，double实参转型有歧义；存在double和int形参，float实参转型不会有歧义）
*/
void func(int) {}
void func(long) {}
void func(float) {}
void test()
{
	//func(5u); // error: func(int) or func(long)
	//func(5.0); // error: func(int) or func(long) or func(float)
}
/*
	函数模板的优先级高于转型匹配，于是可以为可能存在多义性错误的函数定义函数模板重载
*/
void func2(int) {}
void func2(long) {}
template<typename T> void func2(T) {}

void test2()
{
	func2(5u);// func(T)
}

/*
	函数模板重载可以有多个，会有限匹配更具体的版本
	这种行为类似类模板的偏特化
*/

template<typename T> void func3(T) {}
template<typename T> void func3(T*) {}
template<typename T> void func3(std::vector<T>) {}

void test3()
{
	int i = 0;
	func3(i);
	func3(&i);// 实参为int*，func3(T*) 比 func3(T)更具体
	std::vector<int> arr;
	func3(arr);// 实参为std::vector<int>，func3(std::vector<T>) 比 func3(T)更具体
}

/*
	隐式转型不会发生在函数模板上
	但显示指定函数模板参数后，函数模板就具体化为了普通函数，于是也可以发生隐式转型
*/

template<typename T> void func4(T) {}
void test4()
{
	int i = 0;
	func4<long>(i); // = func4(long). i converseto long
}

/*
	默认参数不会被用于模板推导，如果调用所提供的参数（或没有参数）不能够推导出所有模板参数，则失败
*/
template<typename T> void func5(T = 0) {}
void test5()
{
	func5(1); // ok. T = int
	func5(1l); // ok. T = long
	//func5(); // cannot detect with default param
	func5<long>(); // ok. T = long. then use default param value 0 as long
}

/*
	何为SFNIAE？
	Substitution Failure Is Not An Error
	推导模板失败，会继续去匹配其他模板或可变参数函数
	因此尚不能因为当前模板的推导失败而产生编译错误
	而即使最终没有任何函数匹配，产生的错误也不是推导失败错误，而是未匹配错误
	即：函数模板的推导失败不是编译错误
*/
struct A
{
	struct A_in {};
	float a;
	A_in ain;
};
template<typename T> void func6(T t, typename T::A_in) {}

void test6()
{
	A obja;
	func6(obja, obja.ain); // T = A
	//func6(1, 0); // T  = int，但是并不存在int::A_in。此时的报错并不是模板推导失败，而是没有匹配的函数
}

// func6的题外话
namespace extra6
{
/*
	如下是一个无法成功的函数模板，因为其试图使用T::A_in去推导T，但是类型推导是无法通过内部类来推导
*/
template<typename T> void func6(typename T::A_in) {}
void test()
{
	A obja;
	//func6(obja.ain);
}

/*
	三种方式：
		1.显式指定。直接指定T的类型 ，也就不需要推导
		2.修改声明T::A_in => T，让模板直接去推导T，而不关心它是否是内部类
		3.dispatch/重载，也就是 外面的func6(T t, typename T::A_in) 先推导出T，再去验证是否存在T::A_in

	对于第二种方法，T直接推导为A::A_in，此时无法直接使用A，可以以type traits的方式提取出A
*/
template<typename T>
struct extra_outer;

template<>
struct extra_outer<A::A_in> { using type = A; };// 特化A::A_in，提取A

template<typename T> void func6(T) 
{
	using out_t = typename extra_outer<T>::type;//如果直接推导出A::A_in，再用 extra_outer 提取出A
}
}

/*
	最典型的例子是基本类型没有内置类型，而函数模板的模板类型参数却存在内置类型
*/
template<typename T> void func7(T*) {}// 普通指针，类型为T
template<typename T, typename C> void func7(T C::*) {}// C成员指针，类型为T
void test7()
{
	A obja;
	func7(&obja); // func7(A*) 对象指针，T = A
	func7(&obja.a); // func7(int*) 对象成员指针，与普通指针一样 T = int
	func7(&A::a); // func7(int A::*) 类int成员指针 T = int， C = A
}

template<typename T>
class is_class
{
	template<typename C> static int test(...);//重载1，无限制

	template<typename C> static char test(int C::*);//重载2，限制有成员

public:
	// sizeof 取的是函数的返回值，并不会调用函数
	// 显式指定了T为模板参数后，就能通过最佳匹配规则知道是test的哪个重载
	static constexpr bool value = 1 == sizeof(test<T>(nullptr));
};

void test_is_class()
{
	is_class<int>::value; // false
	is_class<A>::value; // true
}

/*
	关于类模板成员的问题，还有以下错误例子
	可以注意到 Base  没有func2函数
	如果不使用Derived<Base>::func2，程序可以编译通过并运行
	但是如果使用了，编译时会发生错误
	这也是SFINAE要解决的问题
	如果一个类型不合适用来作为 类模板的模板参数，那么就 使它成为错误，而不是隐藏直到编译才发现
*/
struct Base
{
	void func() {}
};

template<typename T>
struct Derived :public T
{
	void func()
	{
		T::func();
	}

	void func2()
	{
		T::func2();
	}
};

void test8()
{
	Derived<Base> obj;
	obj.func();
	obj.func2();
}


/*
	考虑sort
	1.如果有成员函数sort，则使用该sort
	2.如果有range，则使用std::sort
	3.无法sort
*/

struct have_sort { char c; }; // size == 1
struct have_range { char c; have_sort c1; }; // size == 2
struct have_nothing { char c; have_range c1; }; // size == 3

template<typename T, size_t>
struct sort_helper;

template<typename T>
struct sort_helper<T, sizeof(have_sort)>
{
	static void sort(T& t)
	{
		t.sort();
	}
};

template<typename T>
struct sort_helper<T, sizeof(have_range)>
{
	static void sort(T& t)
	{
		std::sort(t.begin(), t.end());
	}
};

template<typename T>
struct sort_helper<T, sizeof(have_nothing)>
{
	static void sort(T& t) {}
};

// 匹配成员sort，为了和begin end统一参数，有两个参数
template<typename T>
have_sort test_sort(decltype(&T::sort), decltype(&T::sort));

// 匹配range
template<typename T>
have_range test_sort(decltype(&T::begin), decltype(&T::end));

// 匹配任意，优先级最低
template<typename T>
have_nothing test_sort(...);

template<typename T>
void my_sort(T& t)
{
	sort_helper<T, sizeof(test_sort<T>(nullptr, nullptr))>::sort(t);
}