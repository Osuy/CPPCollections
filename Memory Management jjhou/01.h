#pragma once
#include <stdlib.h>
#include <xmemory>
#include <vector>

/*
	分配内存的方法：
		1.C的malloc（基于系统api）
		2.C++的new、new[]、new()、operator new（基于malloc）
		3.stl的allocator（基于new）
		4.直接使用操作系统的api：heapalloc、virtualalloc

	malloc
		C的库函数，不可覆盖

	new
		C++表达式 不可覆盖，会被编译器翻译更具体的代码

	::new、operator new/new[]/new()
		C++操作符，可覆盖，被new表达式使用
		new 表达式使用 operator new
		new 类名[] 表达式使用 operator new[]
		new(...) 表达式使用 placement new（即带额外参数的operator new）
*/

class ClassA
{
	int x, y, z;
};

void example1()
{
	void* p1 = malloc(512);
	free(p1);

	ClassA* p2 = new ClassA;
	delete p2;

	void* p3 = ::operator new(512);
	::operator delete(p3);

	int* p4 = std::allocator<int>().allocate(3); // 3个int大小的内存
	std::allocator<int>().deallocate(p4, 3); // 归还指针，并且还要指定分配之时的申请大小
/*
	缘于allocator的释放deallocate不仅要指针，还需指定归还的size
	于是不适合直接使用。因为可能已经不晓得或无从知道到底分配多大
	分配多少，归还也分毫不少地归还，唯有容器能做到
*/
}



/*
	new 表达式会编译器翻译为如下代码：
		1.调用operator new，使用sizeof获取类的大小
		2.进行static_cast转型为new后面的类型的指针
		3.调用类构造函数
	（事实上还有一层try-catch用于捕获异常）
*/

void example2()
{
	// new 表达式的翻译
	ClassA* ptr = nullptr;
	try {
		void* mem = operator new(sizeof(ClassA));
		ptr = static_cast<ClassA*>(mem);
		ptr->ClassA(); // 这一行只有编译器能完成（或placement new）
	}
	catch (std::bad_alloc)
	{
	}

	// 验证手动调用构造析构的可行性
	{
		using std::vector;
		vector<int>* vec = new vector<int>;
		vec->vector<int>(); // 不能用指针调用构造函数
		vec->~vector<int>(); // 可以用指针调用析构函数
	}

	// delete 表达式的翻译
	ptr->~ClassA();// 手动调用析构是可以的
	operator delete(ptr);
}

void example3()
{
	ClassA* ptr = new ClassA[3];
/*
	由于array new 没有机会传参，所以类必须有默认构造
	如果需要为每个对象调用非默认构造，可以在new表达式后面
	循环对每个对象的地址使用placement new
*/

/* 
	必须配套使用new[] / delete[]
	如果使用new[] 而对应使用delete
	则析构函数只会调用一次（前提是析构有意义）
	但是整块内存依然会被回收，发生内存泄露的地方不是本块内存
	而是对象内部的动态内存
	由于仅有一次析构的调用，其余对象的多态内存就会泄露

	delete[]调用析构的顺序是由后往前，与new[]调用构造的顺序相反
*/
	delete[] ptr;

/*
	new/new[]申请的内存，除了对象自身大小，还会有额外的信息，其通常有
		1.首尾cookie，2字节
		2.debugheader 32字节
		3.总大小调整为16的倍数所需的填充字节

	对于new[]，如果类的析构有意义，则在new分配的内存的前端会有一个字节用于记录数组的长度
	有了这个长度就能知道调用多少次构造和析构
	如果类的析构无意义，则不会有这个记录长度的字节，即整体申请的内存小一字节
*/
}

void example4()
{
	// placement new
	char* buf = new char[sizeof(ClassA)];
	ClassA* ptr = new(buf) ClassA{}; // placement new， 在new后带括号，传入额外参数，通常是一个已经分配好的指针，可以重载
	
	// placement new 的翻译
	ClassA* ptr = nullptr;
	try {
		void* mem = operator new(sizeof(ClassA), buf); // 将buf传入 operator new，由于重载，会自动调用对应的版本
		ptr = static_cast<ClassA*>(mem);
		ptr->ClassA();
	}
	catch (std::bad_alloc)
	{
	}

	// placement new 的版本实际上什么都不做，单纯把第二参数返回。所以需要自己保证传入的内存地址是有效的
}

/*
	总结new表达式和operator new
	operator new是分配内存的工具，不涉及转型和调用构造析构
	真正牛逼的是new表达式，它会被编译器翻译为更复杂具体的代码，以
	表现出面向对象语音的特征
*/