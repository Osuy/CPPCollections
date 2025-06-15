#pragma once
#include <array>
/*
	new_allocator
		内部直接调用 operator new/delete

	malloc_allocator
		内部直接调用 malloc/free

	array_allocator
		需提供一个已存在的数组，然后把数组当作内存池来使用（数组可以在栈上，会更快，但是离开作用域后失效）
		可以替代C的buf
*/

template<typename T, size_t N>
class array_allocator
{
	std::array<T, N>* buf;
	size_t used = 0;
public:
	array_allocator(std::array<T, N>* Buf) : buf(Buf) {}

	T* allocate(size_t size)
	{
		if (!buf || used + size > buf->size())
		{
			return nullptr;
		}

		auto ret = buf->data() + used;
		used += size;

		return ret;
	}

	void deallocate(void*) {};
};

void example1()
{
	std::array<int, 10> buf;

	array_allocator<int, 10> alloc{ &buf };

	int* p = alloc.allocate(3);

	alloc.deallocate(p); // 不做事
}

/*
	debug_allocator
		需要一个模板参数，也是一个allocator
		其不实际分配内存，而是由内部的allocator去分配
		并且分配的大小会扩大一些，用以增加debug信息

	pool_allocator
		内存池分配器，内部维护了一个free_list

	bitmap_allocator
		另一个做了内存池设计的分配器，但是不是使用链表，而是和loki很像，使用数组
		但是loki其实思想上仍是链表，只是不是用指针串联，而是用下一个可用内存的索引来串联
		而bitmap就压根和链表没关系了，它使用多个整型，整型的每个字节来记录数组的一个单元是否被使用

*/