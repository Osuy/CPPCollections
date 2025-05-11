#pragma once
#include <string>
/*
	替换宏常量，需要取地址的，用const常量
			不需要取地址的，用enum
	替换宏函数，用inline template函数
	
	尽量以const、enum代替宏常量，以inline template代替宏函数
	亦言：可以编译器替代预处理器
*/

namespace case2
{
	// 宏常量只会单纯地替换，不会记入符号表。当调试时，可能会疑惑其来源
#define ASPECT_RATIO 1.653

	// const 常量会被加入符号表可以追踪，取地址和类型检查，因此更加安全
	const float AspectRatio = 1.653f;
	
	// 对于字符指针常量，希望既不改变所指之物，也不改变指向，需要写为：
	const char* const Name = "A Name";
	// 而使用const string可以同时达到两个目的，而且更整洁。
	// 其原因在于string内部维护了char*，以const修饰后，指针作为成员变量无法改变指向，而指针的->和[]操作符恰好又是非const的
	const std::string Name2 = "AName";

	// 对于整型成员常量，既然不能修改，则该类型的所有对象具有相同的成员，为节省空间，应以static修饰
	class Test1
	{
		const static int Num = 5;
		int arr[Num];
	};
	// 虽然Test1的静态成员Num具有初始值5，但是这仍然是声明而非定义，而是in-class初值
	// 如果你不对其取地址（可以使用），则此写法是允许的
	// 但是如果你对其取地址，或者你的编译器不支持in-class，则对与类的静态成员变量，你需要在.cpp文件里提供定义
	const int Test1::Num; // = 5; 此时可以不用初始化。或者此处初始化而声明时不设初始值

	// 另外，Test1使用了Num作为arr数组的大小，这是允许的。这也侧面说明in-class 发生在编译期
	// 因为C/C++的数组必须在编译器就确定大小。
	// 而当编译器不支持in-class时，可以使用enum hack
	class Test2
	{
		enum { Num = 5 };
		int arr[Num];
	};
	//enum hack更接近宏常量。enum在编译时会被转化为整数，因此不可以被取地址，也不占用类的内存空间
	//关于enum hack 是一项模板元编程技术。可见case 48

	//对于一个宏函数
#define FUNC(a, b) f( (a) > (b) ? (a) : (b) )
	//由于宏只做文本替换，因此当a或b不是一个变量，而是一个表达式时，宏函数不会计算表达式再传参
	//而是直接替换，最终该表达式将被执行两次，如
	void f(int) {}
	void test()
	{
		int a = 1;
		FUNC(++a, 1); //替换后++a执行两次
		FUNC(++a, 2); //替换后++a执行一次
	}
	// 当宏被多次嵌套时，以上的错误很难被排查
	// C++的函数模板可以传递任意类型的参数，并且在编译时进行类型检查
	// inline修饰，可以使函数在调用处进行代码替换，节省转调的开销
	// 因此对于以上的宏函数，可以改为
	template<class T>
	inline void TFUNC(T a, T b)
	{
		f((a) > (b) ? (a) : (b));
	}
	// 因此几乎没有必要使用宏函数，除非需要用到除节省转调以外的宏的特性

}