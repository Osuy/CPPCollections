#pragma once
#include <utility>

/*
	右值引用
	常规的引用，如下面的语句：
		int& b=a;
		a是一个左值，b是一个左值引用。
	所谓左值，就是可以取得地址、有名字的值
	相对地，右值就是不可以取得地址、没有名字的值

	如果说，左值引用就是给一个左值取别名。那么，右值引用就是给一个右值取别名。
	如下语句：
		int c=fun();//1
		int&& d==func();//2
		如果函数func是值拷贝的返回类型，将会生成一个临时int变量，它是没有名字的，是一个右值。
		语句1，新建int变量c，从临时对象中拷贝值
		语句2，临时变量直接赋给右值引用

	右值引用获取到右值后，右值的生命周期就和右值引用一样长，因此不必担心临时对象被系统回收
*/

/*
	移动构造函数和移动赋值函数
	仅能包含一个同类型的右值引用参数。
	如
	TestClass(TestClass&& a){...}
	TestClass& operator=(TestClass&& a){...}

	和拷贝构造函数的异同：
		拷贝构造函数：如果有指针成员，进行浅拷贝，指针成员指向参数对象的指针所指，自身和参数的指针成员共同指向同一块地址
								   或者深拷贝，分配内存并复制指针指向的对象。
		移动构造函数：因为参数是右值，会被回收，直接将自身的指针成员指向参数的指针成员，再将参数的指针成员置空
*/

class TestClass
{
private:
	int* _m;
public:
	TestClass() {};
	TestClass(const TestClass& a) { _m = new int(*a._m); }//深拷贝，分配内存并复制对象
	TestClass(TestClass&& a) :_m(a._m) { a._m = nullptr; }//移动，将右值a的指针赋给自己然后把a的指针置空。
														  //移花接木，获取了右值的指针所指的对象
	TestClass& operator=(const TestClass& a) { _m = new int(*a._m); }												  //必须把a的指针置空，这样a被回收后，不会析构掉自身持有的对象
};



/*
	移动语义：std::move
	move(lvalue)的作用是把一个左值转化为右值
	例如
	int a = 3;//a是一个左值
	int& b = a;//正确，b是一个左值引用
	int&& c = a;//错误，右值引用不能用左值初始化
	int&& d = std::move(a);//正确，move将a转化为一个右值

	注意，move将一个左值转化为右值，那么原先的左值不能再使用，就像临时变量被回收一样。
	以上，四条语句执行后，a和b就会失效，只能使用d
*/

//利用move实现swap
//传统的swap，使用拷贝复制，如果TestClass占用内存很大，就会造成大量数据拷贝。
void swap(TestClass& a, TestClass& b)
{
	TestClass tmp;
	tmp = a;
	a = b;
	b = tmp;
}

//swap的move版本。是哦也能够move实现数据转移，成本极低。stl的swap就是用move实现的
void swap(TestClass& a, TestClass& b)
{
	TestClass tmp(std::move(a));
	a = std::move(b);
	b = std::move(tmp);
}

/*
	完美转发：std::forward
	移动语义已经大大优化了代码，但是仍然有一些缺陷使它不够完美。
	考虑以下情况：
*/
//两个函数，一个接收左值引用，一个接收右值引用
template<class T>
void interFun(T& a) {}
template<class T>
void interFun(T&& a) {}

//壳函数，调用interFun
template<class T>
void exterFun(T a)
{
	interFun(a);
}

/*
	以上，exterFun希望根据参数a的类型调用不同版本的interFun
	但现实是，无论如何，它都只能调用接收左值的interFun

	因为即便exterFun接收的a是左值还是右值，在函数体内，a是一个有名称，可取地址的左值。
	直接调用：第一个interFun
	使用move：第二个interFun
	无法做到根据T的类型决定调用哪一个内函数。
	forward就是用来将外部的参数类型原样传给内函数
*/

template<class T>
void exterFun(T a)
{
	interFun(std::move(a));//只能调用第二个interFun
}

/*
	如何才能让壳函数根据接收的参数调用希望的版本呢？
	换言之，如何让壳函数的参数其值连同类型一起传给内函数呢？

	std::forward便实现了这一点。
*/
template<class T>
void exterFun(T a)
{
	interFun(std::forward(a));//将a的值连同类型一起传给interFun
}
	
/*

	在STL库中的某些函数，又或者用户的函数模板，希望通过参数调用合适的函数的重载。
	最典型的就是拷贝构造函数和移动构造函数。还有上面的interFun。
	由于引用折叠的问题，往往会调用错误的版本。

*/