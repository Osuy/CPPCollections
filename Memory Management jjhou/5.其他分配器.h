#pragma once
#include <array>
/*
	new_allocator
		�ڲ�ֱ�ӵ��� operator new/delete

	malloc_allocator
		�ڲ�ֱ�ӵ��� malloc/free

	array_allocator
		���ṩһ���Ѵ��ڵ����飬Ȼ������鵱���ڴ����ʹ�ã����������ջ�ϣ�����죬�����뿪�������ʧЧ��
		�������C��buf
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

	alloc.deallocate(p); // ������
}

/*
	debug_allocator
		��Ҫһ��ģ�������Ҳ��һ��allocator
		�䲻ʵ�ʷ����ڴ棬�������ڲ���allocatorȥ����
		���ҷ���Ĵ�С������һЩ����������debug��Ϣ

	pool_allocator
		�ڴ�ط��������ڲ�ά����һ��free_list

	bitmap_allocator
		��һ�������ڴ����Ƶķ����������ǲ���ʹ���������Ǻ�loki����ʹ������
		����loki��ʵ˼������������ֻ�ǲ�����ָ�봮������������һ�������ڴ������������
		��bitmap��ѹ��������û��ϵ�ˣ���ʹ�ö�����ͣ����͵�ÿ���ֽ�����¼�����һ����Ԫ�Ƿ�ʹ��

*/