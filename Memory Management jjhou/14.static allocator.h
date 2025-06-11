#pragma once
#include <stdlib.h>
/*
	per-class allocator 2 的设计已经足够好了
	那么现在面临的问题是，对于每个需要用到内存池的类，都需要重载operator new 和 operator delete
	并且他们的代码几乎一样
	为了提高复用性，设计一个专门实现内存池的类，并让需要内存池的类的operator new 和 operator delete 调用这个类的函数

	具体实现上有所不同
	由于不知道具体的类型，所以不再使用union，但思想没变
	因为分配内存时一定会传入类的大小，所以以该大小为间隔串联出链表即可
*/

class allocator
{
	enum { CHUNK = 5 };// 池大小定为5。如果顺序创建，那么每五个对象是相邻的
	struct obj
	{
		struct obj* next = nullptr;
	};

	obj* freeStore = nullptr;

public:
	void* allocate(size_t size)
	{
		obj* p = nullptr;

		if (!freeStore)
		{
			size_t chunk = CHUNK * size;
			p = freeStore = (obj*)malloc(chunk);

			for (int i = 0; i < CHUNK; ++i)
			{
				p->next = (obj*)((char*)p + size);// 转为char* 并前进size个字节。相当于指向下一个对象的头部
				p = p->next;
			}

			p->next = nullptr;
		}

		p = freeStore;
		freeStore = freeStore->next;
		return p;
	}

	void deallocate(void* p, size_t)
	{
		((obj*)p)->next = freeStore;
		freeStore = (obj*)p;
	}
};

/*
	于是可以给每个要使用分配器的类都重载如下的operator new/delete
*/

class ClassD
{
	static allocator alloc;
public:
	static void* operator new(size_t size)
	{
		return alloc.allocate(size);
	}
	static void operator delete(void* data, size_t size)
	{
		return alloc.deallocate(data, size);
	}
};

// 静态成员变量需类外定义
allocator ClassD::alloc{};

/*
	当然，重复书写如上的代码，但是可以发现，除了类外定义的静态成员变量
	其他所有代码都是与类名无关的，所以可以使用一行宏来封装
	对于类外的静态成员变量的定义，也可以使用宏，但勉为其难要写上类名

	如下，便可只用两行宏就使类具有了内存池功能
*/
#define DECLARE_POOL_ALLOC()\
protected:\
	static allocator alloc;\
public:\
	static void* operator new(size_t size)\
	{\
		return alloc.allocate(size);\
	}\
	static void operator delete(void* data, size_t size)\
	{\
		return alloc.deallocate(data, size);\
	}

#define IMPLEMENT_POOL_ALLOC(class_name)\
allocator class_name::alloc{};

/*
	版本1：类自己实现链表和池
	版本2：利用union优化掉聊表所需的额外指针
	版本3：专门的类实现内存池并被所有类调用
	版本4：宏封装
*/
