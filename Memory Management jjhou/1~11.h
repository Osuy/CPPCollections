#pragma once
#include <stdlib.h>
#include <xmemory>
#include <vector>
#include <iostream>

/*
	分配内存的方法：
		1.C 的 malloc （基于系统api）
		2.C++的 operator new （基于malloc，仅分配内存。分配内存并构造对象是new表达式）
		3.stl 的 allocator （基于new）
		4.直接使用操作系统的api：heapalloc、virtualalloc

	malloc
		C 的库函数，不可覆盖

	new
		C++ 表达式 不可覆盖，会被编译器翻译更具体的代码

	operator new/new[]/new()
		C++操作符，可覆盖，被new表达式使用
		new 表达式使用 operator new
		new 类名[] 表达式使用 operator new[]
		new(...) 表达式使用 placement new （即带额外参数的 operator new ）
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
		1.调用operator new ，使用 sizeof 获取类的大小
		2.进行static_cast转型为类指针
		3.调用类构造函数
	（事实上还有一层 try-catch 用于捕获异常）
*/

void example2()
{
	// new 表达式的翻译
	ClassA* ptr = nullptr;
	try {
		void* mem = operator new(sizeof(ClassA));
		ptr = static_cast<ClassA*>(mem);
		//ptr->ClassA(); // 这一行只有编译器能完成（或placement new）
	}
	catch (std::bad_alloc)
	{
	}

	// 验证手动调用构造析构的可行性
	{
		using std::vector;
		vector<int>* vec = new vector<int>;
		//vec->vector<int>(); // 不能用指针调用构造函数
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
	如果需要为每个对象调用非默认构造，可以
	循环对每个对象的地址使用placement new
*/

/* 
	必须配套使用new[] / delete[]
	如果使用new[] 而对应使用delete
	则析构函数只会调用一次（前提是析构有意义）
	但是整块内存依然会被回收，发生内存泄露的地方不是本块内存
	而是对象内部的动态内存
	由于仅有一次析构的调用，其余对象的动态内存就会泄露

	delete[]调用析构的顺序是由后往前，与new[]调用构造的顺序相反（为了使delete是new的逆过程）
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
	try {
		void* mem = operator new(sizeof(ClassA), buf); // 将buf传入 operator new，由于重载，会自动调用对应的版本
		ptr = static_cast<ClassA*>(mem);
		//ptr->ClassA();
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

/*
	重载operator new
	既然new表达式会调用operator new
	便有机缘可以由程序员来重载operator new以控制内存分配的细节，这便是内存管理

	重载的目标有两个：
		1.global命名空间下的operator new ，即 ::operator new
		2.每个类命名空间下的operator new， 即 XX::operator new

	由于所有的基本类型和没有定义成员operator new的类都会使用 ::operator new （C++的函数匹配机制）
	所以重载::operator new的影响十分深远。非常少见
	一般针对特殊的类型重载成员operator new即可
*/

/*
	重载::operator new
*/

void* operator new(size_t size)
{
	std::cout << "::" << __func__ << std::endl;
	return malloc(size);
}

// inline void* operator new(int size); // 第一参数必须是size_t

void* operator new[](size_t size)
{
	std::cout << "::" << __func__ << std::endl;
	return malloc(size);
}

void operator delete(void* mem)
{
	std::cout << "::" << __func__ << std::endl;
	return free(mem);
}

void operator delete[](void* mem)
{
	std::cout << "::" << __func__ << std::endl;
	return free(mem);
}

/*
	类成员operator new
	必须是static，因为new对象之前对象不存在，无法用关联对象 之this指针来调用
	但是可以省略
*/
class ClassB
{
	int var = 0;
public:
	ClassB()
	{
		std::cout << "ClassB::" << __func__ << ". this = " << this << " var = " << var << std::endl;
	}

	ClassB(int v) : var{ v }
	{
		std::cout << "ClassB::" << __func__ << ". this = " << this << " var = " << var << std::endl;
	}

	~ClassB()
	{
		std::cout << "ClassB::" << __func__ << " this = " << this << " var = " << var << std::endl;
	}

	inline void* operator new(size_t size)
	{
		std::cout << "ClassB::" << __func__ << ". size = " << size << std::endl;
		auto mem = ::operator new(size);
		std::cout << "return = " << mem << std::endl;
		return mem;
	}

	inline void* operator new[](size_t size)
	{
		std::cout << "ClassB::" << __func__ << ". size = " << size << std::endl;
		auto mem = ::operator new[](size);

		std::cout << "return = " << mem << std::endl;
		return mem;
	}

	inline void operator delete(void* mem, size_t)
	{
		std::cout << "ClassB::" << __func__ << ". mem = " << mem << std::endl;
		return ::operator delete(mem);
	}

	inline void operator delete[](void* mem, size_t)
	{
		std::cout << "ClassB::" << __func__ << ". mem = " << mem << std::endl;
		return ::operator delete[](mem);
	}
};

void example5()
{
	std::cout << "part 1" << std::endl;
	auto pb = new ClassB{ 1 };

	std::cout << std::endl << "part 2" << std::endl;
	delete pb;

	std::cout << std::endl << "part 3" << std::endl;
	auto pba = new ClassB[3];
	auto tmp = pba;
	for (int i = 0; i < 3; ++i)
	{
		::new(tmp++) ClassB{ i }; // 由于ClassB 重载了operator new 而没有重载placement new ，基于函数覆盖规则，它不能使用placement new ，此处用全局的placement new 替代
	}

	std::cout << std::endl << "part 4" << std::endl;
	delete[] pba;
}

/*
	placement new
	本质上是operator new的重载。因此会被类重载的operator new覆盖
	既然是重载，参数就要有所不同，最常见的版本是多了一个void*参数，已默认提供
	但是并不代表placement new只能传void*
	可以根据需要重载任意参数的 placement new
*/

class ClassC
{
public:
	ClassC()
	{
		std::cout << "ClassC::" << __func__ << std::endl;
	}
	ClassC(int)
	{
		std::cout << "ClassC::" << __func__ << std::endl;
		throw std::exception();
	}

	inline void* operator new(size_t size)
	{
		std::cout << "ClassC::" << __func__ << "(size_t). size = " << size << std::endl;
		auto mem = ::operator new(size);

		std::cout << "return = " << mem << std::endl;
		return mem;
	}

	// placement new 的常规版本，第二参数的void*
	inline void* operator new(size_t size, void* mem)
	{
		std::cout << "ClassC::" << __func__ << "(size_t, void*). size = " << size << std::endl;
		std::cout << "return = " << mem << std::endl;
		return mem;
	}

	// placement new 第二参数是size_t
	inline void* operator new(size_t size, long extra_size) // 这里的extra_size的类型 为什么不写为size_t？
	{
		std::cout << "ClassC::" << __func__ << "(size_t, size_t). size = " << size + extra_size << std::endl;
		auto mem = ::operator new(size + extra_size);
		std::cout << "return = " << mem << std::endl;
		return mem;
	}

	// placement new 第二参数是size_t
	inline void* operator new(size_t size, long extra_size, char c) // 这里的extra_size的类型 为什么不写为size_t？
	{
		std::cout << "ClassC::" << __func__ << "(size_t, size_t, char). size = " << size + extra_size << std::endl;
		auto mem = ::operator new(size + extra_size);
		std::cout << "return = " << mem << std::endl;
		return mem;
	}

	inline void operator delete(void* mem, size_t)
	{
		std::cout << "ClassC::" << __func__ << "(void*, size_t). mem = " << mem << std::endl;
		return ::operator delete(mem);
	}

	// 对应placement，也需要写出对应的 placement delete，但它们绝不会任何delete表达式调用。见Effective C++ case 52
	inline void operator delete(void* mem, void*)
	{
		std::cout << "ClassC::" << __func__ << "(void*, void*). mem = " << mem << std::endl;
		return ::operator delete(mem);
	}

	inline void operator delete(void* mem, long)
	{
		std::cout << "ClassC::" << __func__ << "(void*, long). mem = " << mem << std::endl;
		return ::operator delete(mem);
	}

	inline void operator delete(void* mem, long, char)
	{
		std::cout << "ClassC::" << __func__ << "(void*, long, char). mem = " << mem << std::endl;
		return ::operator delete(mem);
	}

	/*
		为什么placement delete 的第二参数的类型不可以是 size_t？
		因为operator delete 的参数列表是 (void* mem, size_t)，而对于 placement delete， size_t 会变为 placement new 的额外参数类型
		如果placement new 的第额外参数就是 size_t，其对于版本的placement delete就和operator delete的格式完全一样
		也即不存在对于的placement delete
	*/
};

void example6()
{
	ClassC c1;
	auto pc1 = new ClassC;
	auto pc2 = new(&c1) ClassC;
	auto pc3 = new(100) ClassC;
	auto pc4 = new(100, 'a') ClassC;
	// 以下都是使用operator delete 去释放 placement new申请的内存（除了pc2）。没有问题
	delete pc1;
	delete pc3;
	delete pc4;

	// 验证异常是否能调用placement delete
	// 调用带抛出异常的构造
	auto pc5 = new ClassC{ 1 };
	auto pc6 = new(&c1) ClassC{ 1 };
	auto pc7 = new(100) ClassC{ 1 };
	auto pc8 = new(100, 'a') ClassC{ 1 };
	// 以下代码无用因为内存已经被placement delete回收
	delete pc5;
	delete pc7;
	delete pc8;
}

/*
	placement delete唯有在使用了 placement new 后构造函数抛出异常时才会被调用
	也唯有这种情况，才直到使用的是哪种 placement new ，因为编译器知道
	如果 定义了  placement new 却不定义对应版本的  placement delete，意味着要放弃异常处理
*/

/*
	标准库的string内部除了字符串以外，还持有一个引用计数对象
	因此也使用replacement new技巧，为每个string对象都申请额外的大小用以构造引用计数对象
*/