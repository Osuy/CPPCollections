#pragma once
#include <vcruntime_exception.h>
#include <new.h>
/*
new
	一、必须得到正确的指针，不足时请调用new-handler，没有new-handler时抛出错误
		（因此你必须提供一个真的做了什么事的new-handler，不然就只能寄希望于其他程序退让出内存）
		如果你没法拿到new-handler，可以通过两次set_new_handler取出再放回的方式拿到，但再多线程环境下必须考虑加锁

	二、能有对size为0的情况有正常表现。默认new会在四则为0时申请1大小的内存

	三、对于非希望的size有一般正常表现。如果你专门为一个类定制了operator new，而它又存在子类并且子类没有重写new
		那么子类会继承父类的new。可是Base::operator new是专门为sizeof(Base)准备的，对于Derived，可能表现不佳
		你可以这么做：
			if(size != sizeof(Base)) return ::operator new(size);
		无需担心size为0的情况，因为Base至少有1的大小，所以为0时就会由::new处理

	四、对于member operator new[]，你不能假定有多少元素，因为它可能用于创建子类对象数组，那么变不能通过size / sizeof(Base)来获取数量

delete
	一、确保释放对象是正确的。即
		if(!p)return;

	二、对于member operator delete，如果size不正确则交给::delete以能够有一般表现
		if(size != sizeof(Base))
		{
			::operator delete(p);
			return;
		}
		如果前提是你将Derived的析构函数声明为virtual，否则传给delete的size仍然是Base的大小

		因为如果是delete一个指向子类对象的基类指针，是Base的大小传入delete，于是发生内存泄漏
		而如果delete一个指向子类对象的子类指针，那么size会是子类的大小，但是因为Base::delete被子类继承了
		这也是为啥一个Base::delete传入的size不一定是Base的大小的缘故
		所以delete可能收到一个子类的大小，于是应该交给全局delete去处理

*/

namespace case51
{
void* operator new(size_t size)throw (std::bad_alloc)
{
	if (0 == size) size = 1;

	// 必须死循环，为了能不断调用 new_handler 来试图释放内存，以供分配内存
	while (true)
	{
		if (1/*分配成功*/) return 0/*分配得到的内存*/;

		// 两次 set_new_handler 来获取当前的 new_handler
		new_handler globalHandler = set_new_handler(0);
		set_new_handler(globalHandler);

		// 如果有 new_handler 调用，进入下次循环
		// 如果没有，抛出异常
		if (globalHandler) (*globalHandler)();
		else throw std::bad_alloc();
	}
}

class Base
{
public:
	static void* operator new(size_t size) throw (std::bad_alloc)
	{
		if (size != sizeof(Base))// sizeof(Base) 不会是0
		{
			// 如果此operator new 不是用于new一个Base大小的内存，则交给全局new去做
			// 因为一个类的new，应该被认为是为了它自己专属使用的，只对自己的size有良好表现
			// 其他size都表现不佳，所以应该用全局new
			return ::operator new(size);
		}

		return ::operator new(size);
	}

	static void operator delete(void* memory, size_t size) throw()
	{
		if (0 == memory)return;
		if (size != sizeof(Base))
		{
			::operator delete(memory, size);
			return;
		}

		return;
	}
};
}