#pragma once

namespace case4
{
/*
	不要寄希望于编译器和类构造函数帮你完成初始化
	如果是你自己设计的类，确保构造函数初始化列表初始化了所有的成员变量
	如果成员是内置类型、const或引用，必须初始化
	如果成员是类，不初始化也会调用默认构造，但是为了不使忘记，尽量也在初始化列表里写上

	初始化的顺序总是按照声明顺序来的
	而后续考虑内存对齐时可能会改动声明顺序，因此无需关心初始化列表的顺序

	static变量有两种：
		1.出现在函数内部的，局部静态变量
		2.其他，包括没被static修饰，但声明在全局或namespace中的变量
	局部静态变量的强大之处在于：只会在初次进入函数时初始化。
	而其他静态变量的生命周期是与程序同步的。所以如果一个函数至始至终都未被调用，那么它内部的静态变量从未被构造过（但还是会占用静态区内存）
	其他静态变量需要在头文件中声明，cpp文件中定义，然而它们的顺序是未知的，也没有任何编译器保证以怎样的顺序初始化它们
	因此假如A的定义用到了B，而B尚未初始化，那么A的初始化结果的可用性也便丧失了
	一个改进的方法是使用返回局部静态变量引用的函数
	因为是局部静态变量，进入函数时它必然会被初始化，所以可以安全使用

*/

	int b;
	int a;
	int& get_b()
	{
		int static b = 1;
		return b;
	}
}

// a和b的初始化顺序不能保证，因此a = b，可能只得到一个未知的值
int case4::a = b;
int case4::b = 1;

int case4::a = get_b();// get_b一定会先初始化局部静态变量再返回