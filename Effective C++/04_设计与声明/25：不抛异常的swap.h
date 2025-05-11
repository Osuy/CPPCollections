#pragma once
#include <algorithm>

namespace case25
{
	/*
		如何为自定义类实现swap，并且有良好的性能和统一的形式呢？
		（统一的形式是指 swap(a, b)，在标准库中任何对象的互换都是这种形式）
		1.如果类无需深拷贝，那么直接使用std::swap，因为它们的互换就是它们的bits的互换
		2.如果是pimpl类，只需互换持有的指针，可以提供一个成员函数swap用于交换指针
			2.1.然后特化一个std::swap，调用成员swap
			2.2.但如果类是一个类模板，特化就变成了偏特化，而函数是不能偏特化的，于是
				不在std命名空间里特化swap，而是在本地命名空间里声明一个no-member-swap，调用成员swap
				非类模板也可以声明这样一个no-member-swap
			2.3.最后在任何调用swap的地方的前面加上 using std::swap，让编译器去自动匹配最佳swap
	
		std::swap，它是一个函数模板。std::swap除了程序员使用，标准库也会使用
		std::swap的泛化实现是使用中间变量temp对两个参数进行互换，当对象是pimpl时性能不佳
		因为那将会进行3次深拷贝和1次析构
		std的许多内部内进行了特化或重载，使得swap用于std内部不会有多余的拷贝，已经具有了高效的性能
		但对于自定义类，如果程序员不做特殊的处理，那么它的效率将不尽如人意
		pimlp的互换要点在于把两个指针进行互换
		可以为自定义类型特化一个std::swap，但那需要将swap声明为friend，才能操作私有成员
		因此更好的做法是：在类的内部声明一个public函数swap
	*/
	class Test1
	{
		int* p;

	public:
		Test1() :p(new int) {}
		Test1(int* P) :p(P) {}
		~Test1() { if (p)delete p; }

		void swap(Test1& rhs)
		{
			using std::swap;
			swap(p, rhs.p);
		}
	};
	/*
		如此，对于两个对象，可以 a.swap(b); 来进行互换
		但是这样还无法和std的其他算法配接，因为标准的写法应该是 std::swap(a, b);

		为此你还需要对std::swap进行特化	
	*/
}

namespace std
{
	template<>
	void swap<case25::Test1>(case25::Test1& a, case25::Test1& b)
	{
		a.swap(b);
	}
}

	/*
		这样的实现已经接近许多stl容器了。stl中的许多容器都有自己的swap，并且对std::swap进行了特化已去调用成员swap
		如此这个自定义类已经适配了stl的算法，可以很轻松高效地 互换
		前提是这个类不是一个类模板

		是的，当自定义类是一个类模板时，你在特化std::swap时会发现一个问题
	*/
namespace case25
{
	template<class T>
	class Test2
	{
		T* p;

	public:
		Test2() :p(new int) {}
		Test2(int* P) :p(P) {}
		~Test2() { if (p)delete p; }

		void swap(Test2& rhs)
		{
			using std::swap;
			swap(p, rhs.p);
		}
	};

	/*
		template<class T>
		void swap<case25::Test2<T>>(case25::Test2<T>& a, case25::Test2<T>& b)
		{
			a.swap(b);
		}
		以上代码将无法通过编译，因为模板参数T用于Test2类模板的模板参数，这是典型的偏特化
		而函数模板是不支持偏特化的。其原因在于函数支持重载，偏特化几乎没有意义

		那么重载不就行了吗
		template<class T>
		void swap(case25::Test2<T>& a, case25::Test2<T>& b)
		{
			a.swap(b);
		}

		这牵扯到一个原则性问题。特化是一个函数模板针对某一类型的实例化，它仍然是原本的那个函数模板
		但是函数重载在底层已经是另一个函数了，只不过是同名
		而这个std::swap毫无疑问是需要写在std命名空间里的，这相当于在std里加入了新的东西
		虽然说不会有人阻止你这么做，但是向std里加入新东西不应当是可取的行为

		为此，应该在类模板所在的命名空间里声明一个no-member swap
	*/
	template<class T>
	void swap(Test2<T>& a, Test2<T>& b)
	{
		a.swap(b);
	}
	/*
		这样一个实现，在不用命名空间调用时swap(a,b)，会正确地调用

		不过这这情况非常狭隘，需要类模板+pimpl，才用得上
		如果你希望代码具有统一的风格，可以
		using std::swap;
		swap(a,b);
		如此，所在命名空间里有nomenber swap时，会调用那个版本
		所在命名空间没有时，由于using std::swap，所以会去匹配std::swap，
		在有特化版本时，swap会去调用特化版，没有特化时，会去调用泛化版
		三者具有了统一的风格
	*/
}

