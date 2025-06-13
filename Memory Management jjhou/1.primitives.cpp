#include "1.primitives.h"

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
	std::cout << "::" << __func__ << "(void*)" << std::endl;
	return free(mem);
}

// 默认调用这个 delete ，如果没有，则调用上面的
void operator delete(void* mem, size_t size)
{
	std::cout << "::" << __func__ << "(void*, size_T)" << std::endl;
	return free(mem);
}

void operator delete[](void* mem)
{
	std::cout << "::" << __func__ << std::endl;
	return free(mem);
}



namespace my
{ 

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

Screen* Screen::freeStore = nullptr;

void* Screen::operator new(size_t size)
{
	Screen* p = nullptr;
	if (!freeStore)
	{
		size_t chunk = screenChunk * size;
		freeStore = p = reinterpret_cast<Screen*>(new char[chunk]);

		for (;p != &freeStore[screenChunk - 1];++p)
		{
			p->next = p + 1;
		}

		p->next = nullptr;
	}

	p = freeStore;
	freeStore = freeStore->next;
	return p;
}

void Screen::operator delete(void*data, size_t)
{
	(static_cast<Screen*>(data))->next = freeStore;
	freeStore = static_cast<Screen*>(data);
}


}