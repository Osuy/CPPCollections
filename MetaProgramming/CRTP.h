#pragma once
#include <iostream>
namespace _crtp
{
/*
	CRTP：Curiously Recurring Template Pattern 奇异递归模板模式，亦称 Do it for me

	用一个类模板作为基类，并且把派生类作为模板参数
	基类任何希望调用派生类的函数之处，用 static_cast 把this转为派生类去调用
*/
template<typename Derived>
struct Base
{
	void base_func()
	{
		static_cast<Derived*>(this)->derived_func();
	}
};

struct Derived : public Base<Derived>
{
	void derived_func()
	{
		std::cout << "Derived::derived_func executed" << std::endl;
	}
};

/*
	为什么能这样？
	CRTP的基类Base虽然使用子类Derived作为模板参数，但是内部除了转型，不会使用Derived
	Derived对Base来所就相当于是一个前置类型声明
	一个声明但未定义的class，可以用来声明指针，但不能声明成员
	转型时也并未需要Derived的完整定义

	调用了Derived的函数derived_func，不算是使用吗？
	类模板的所有成员函数都视为函数模板，函数模板只在存在被调用的情况才会去产生实例
	所以在类模板实例化过程中，即便在成员函数内使用了Derived，也不是实质上的使用
	而如果调用base_func，必然绑定于一个Derived对象，而一个Derived对象如果 能够存在，必然已经存在了Derived的定义
	于是base_func在实例化时也就能知道derived_func的定义
	
	static_cast 把this转为派生类安全吗？
	CRTP的要求就是把派生类作为基类的模板参数，并且永远不单独使用基类，因此在实际运行中，this必然是一个Derived对象
	于是将this转为Derived指针，实际上总是安全的。除非不遵守上述约定
*/

/*
	CRTP的限制
	CRTP不支持嵌套类型声明
	base_func能调用derived_func而不报错，在于它是函数模板，被使用才实例化
	而类型声明即便没被使用，也要推导。此时Derived对Base是不完整的，因为Derived的定义依赖于Base<Derived>的定义
	而Base<Derived>::T却依赖于Derived::T
*/

template<typename Derived> struct Base2 
{ 
	// using T = typename Derived::T;  // 声明嵌套T，编译时失败
};

struct Derived2 : public Base2<Derived2> { using T = int; };

/*
	用例1：为所有支持==的子类自动增加!=
	通常，一个类的!=操作符的实现都是对 ==操作符结果的取反，而这份代码几乎是完全一样的，因此可以直接交由base实现
*/
namespace example1
{
	template<typename Derived>
	struct Base
	{
		bool operator!=(const Derived& other)const
		{
			return static_cast<const Derived*>(this)->operator ==(other);
		}
	};

	struct Derived : public Base<Derived>
	{
		bool operator==(const Derived& other)const
		{
			return false;// implementation
		}
	};
}

/*
	C++20的concepts和C++23的显式this形参，可以取代CRTP的大部分功能，而且可读性更佳
*/
namespace example2
{
	// 使用显示this形参取代crtp
	struct Base
	{
		template<typename T>
		void base_func(this T&& self)  
		{
			self.derived_func();
		}
	};

	struct Derived : public Base
	{
		void derived_func()
		{
			std::cout << "Derived::derived_func executed" << std::endl;
		}
	};
}
}


