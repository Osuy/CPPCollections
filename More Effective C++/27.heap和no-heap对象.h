#pragma once

/*
	Heap-Based Object 仅存在于Heap的对象
	只让对象存在heap中，可能的目的：可以delete this
	不让对象存在heap中，可能的目的：永不内存泄露

	为限制对象只存在heap中，所有局部对象都不应通过，因此可以将构造设为private
	然而编译器提供的构造函数“们”总为public，因此如果不愿意自己提供，可以将析构设为private
	并让公开成员函数来负责析构
*/
namespace case27
{
class NSObject
{
	~NSObject() {};
public:
	void destroy()
	{
		delete this; // 可以调用析构
	}
};

void example1()
{
	NSObject obj;// 析构私有，于是不能创建栈对象

	auto p = new NSObject;
	delete p; // 无法调用析构
	p->destroy();
}

/*
	然而私有化构造或析构，也意味着这个类无法被继承或内含。
	VS2022对这两种情况只会给出警告
*/
class NSObject_Derived :public NSObject {};

class NSObject_Containment 
{
	NSObject _nsobj;
};

/*
	对于前者，可以把private改为protected，如此子类可以析构父类
		然而子类如果没有特殊设计，可以在栈上创建，那么子类对象内部包含的那个父类对象也相当于是在栈上
		构造函数被调用时，无法得知自己是被直接创建，还是被子类间接创建的

	对于后者，可以把内含关系替换为组合关系。还可以避免包含头文件，降低依赖。这是常用做法
		若要析构，同栈对象一样，使用destroy
*/


/*
	判断某个对象是否在heap中
	没有绝对完美的方法，因为C++在语言层面未支持

	是否可以通过重载new来实现呢？在堆上创建对象一定通过分配内存来实现。通过在new操作符里插入一些操作
	就能判断构造函数被调用时，到底是使用的栈内存还是堆内存
	当然可以，但是面临两个问题：多线程、new[]
	如果重载new中的操作没考虑多线程，或者没重载new[]，那么：1.多个对象同时创建；2.创建数组时，除第一个，其他可能判断错误

	如果明确了所属平台的操作系统对内存的组织：heap往高增长，stack往低增长，便可设计出一个函数用来判断
	因为当进入onHeap函数时，stack向低处增长，于是里面的所有变量都必然比之前的栈变量地址要低。如果不满足，则那个地址不在栈上
*/

bool onHeap(const void* address)
{
	char onStack;
	return address < &onStack;
}
}