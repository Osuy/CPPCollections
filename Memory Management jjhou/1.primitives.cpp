#include "1.primitives.h"

void* operator new(size_t size)
{
	std::cout << "::" << __func__ << std::endl;
	return malloc(size);
}

// inline void* operator new(int size); // ��һ����������size_t

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

// Ĭ�ϵ������ delete �����û�У�����������
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

	int* p4 = std::allocator<int>().allocate(3); // 3��int��С���ڴ�
	std::allocator<int>().deallocate(p4, 3); // �黹ָ�룬���һ�Ҫָ������֮ʱ�������С
	/*
		Ե��allocator���ͷ�deallocate����Ҫָ�룬����ָ���黹��size
		���ǲ��ʺ�ֱ��ʹ�á���Ϊ�����Ѿ������û��޴�֪�����׷�����
		������٣��黹Ҳ�ֺ����ٵع黹��Ψ������������
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