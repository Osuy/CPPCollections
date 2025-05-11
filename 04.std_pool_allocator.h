#pragma once
/*
	前面已经介绍了基于union obj的链表内存池
	这也是stl的pool allocator的基本思路
	这种技术也称为嵌入指针，即向空闲的内存（还未存放数据）中借4个字节来作为指针
		当空闲内存被交出时，数据会覆盖上去；而内存归还时，数据不再重要，于是
		借4个字节作为指针，头插法插入到空闲内存链表中

	
	由此也引出了一个union的使用思想：
		union适合用于只写的情况。因为单纯地写入会覆盖
		而读取的话，由于不知道哪个成员是有效的，因此需要借助外部标记，
		那样还要额外的空间，就与使用union的目的背道而驰了

	
	pool allocater实现细节
		pool allocator内部是维护了一个free_list，一共16个元素
		其每个元素代表所指的空闲内存的规模，逐个递增，从8 至 128
		即： 0元素指向字节为8的空闲内存链表，
			1元素指向字节为16的空闲内存链表，
			...
			15号元素指向字节为128的空闲内存链表
			而当用户分配的内存超过128时，可以认为cookie微乎其微，于是继续使用默认allocator

			这样的设计有何用意呢？
				首先，维护一个任意大小的内存链表是不切实际的，只能分段维护
				8~128，步长为8，就需要16个指针的额外开销。如果想要任意长度，
				那就是120个指针，而有些大小的内存可能几乎不会用到。
				因此，8~128，步长为8已经能够满足大多数内存分配的需求，而且16个指针
				64字节的内存也不会太浪费

				那么这样就引出一个之前没遇到的问题：
				之前的per class allocator是一个类一个池，哪怕是宏内存池，也是
				类自己的重载new来分配，不必关系内存的大小
				但是现在是一个free_list内存池，归还内存时，需要根据其大小插入对应的空闲链表中
				因此就需要使用者记录要了多少字节的内存，归还时也要归还相应字节的内存
				这样的要求过于苛刻。忘记分配了多少字节总是难免的

				这就是为什么推荐只在使用容器时用pool allocator
				因为容器内的每个元素都是同样大小，不必在意分配和归还到底是多少字节
		
		然后，程序员需要分配若干字节内存
			先将其上调至8的倍数，例如，28 上调至32，即3号链表
			分配器查找3号链表，是否有可用内存
			如果有，头删取出第一块内存，返回指针给程序员
			如果没有，查找备用内存，现在备用内存是0字节
			于是用malloc分配 2 * 20 * 32 一共1280字节的内存
			思想是：分配20块该大小的内存，不多也不少，够用也不会太浪费
			这20块内存交给3号链表，就没有剩余了，如果还需要其他大小，就要再次malloc
			因此分配两倍，也就是40块内存1280字节，其中20块内存交给链表，剩余640字节留着备用

		程序员又需要分配48字节的内存
			看看5号链表，有，头删返回第一块；没有查看备用内存，还有640字节
			于是将备用内存切成48字节，切除13个交给5号链表，备用内存剩余16字节

		程序员又需要分配96字节的内存
			查看11号链表，有，头删返回第一块；没有查看备用内存，还有16字节，一块都不够
			于是将这16个字节作为1块，交给1号链表，然后用malloc分配
			分配多少是有讲究的：
				除了2 * 20 * 96 = 3840字节，内存池还有额外的考虑，
				在一般程序中，内存需求的增长量不是均匀的，而是越来越多
				就像抛物线一样，越到后面需求量越大，因此，追加已分配量/16的内存
				已分配了1280字节，除以16即80字节
				于是共分配3920字节
			取出20块96字节交给11号链表，备用内存剩余2000字节

		现在又需要分配8字节内存
			查找0号链表，有，头删返回第一块；没有查看备用内存，还有2000字节
			切出160字节，串成20*8交给0号链表，第一块返回给程序员
			备用内存1840字节

		现在又需要分配104字节的内存
			查找12号链表，有，头删返回第一块；没有，查看备用内存，还有1840字节
			切出17 * 104 = 1768字节，一块返回给程序员，剩余串成链表交给12号链表
			备用内存72字节

		现在思考一个不那么可能出现的问题：
			当前总共分配了5200字节内存，假设，系统内存一共只有5400，
			只有200字节可用了，那么下一次分配就会失败
			假设下一次需要112字节内存，13号链表为空，备用内存72字节
			于是将72字节作为一块交给8号链表，然后分配
			2 * 20 * 112 + 5200 / 16 = 773字节，但是由于只剩200字节，会分配失败

			有两种解决办法：
			1.可用200字节，需要112字节，但是分配要773字节，
				可以将773循环减半，直至小于200，大于112，就可以分配成功
				当然也可能还是不成功
			2.向14、15号链表借用一块内存。
				当前案例15、15号链表没有，但是假设需要的不是112字节，是88字节
				就可以向96字节的链表借用

			
			96字节，即11号链表目前有19块内存处于空闲，现在需要一块88字节的内存，
			但是系统内存耗尽，无法分配成功
			这种情况十分尴尬：
				其他链表分配成功，有许多空闲内存
				而系统内存耗尽，再后面的链表分配失败
				一言以蔽之，就是占着茅坑不拉屎
			为了处理这种情况的发生，stl的pool allocator的做法是：
				向大于需求，且有空闲内存的最小链表取一块出来
				需要88字节，向96字节的链表取一块内存，其中88字节返回给程序员
				剩余8字节头插给0号链表
			这样，就能充分利用已经分配但是处于空闲的内存

			但是会不会，只有小于需求的内存处于空闲，即
				需求88字节，但是大于88字节的链表都为空，只有小于88字节的链表有空闲
			
			能不能把小的内存块合并，切出88字节？
				这样做在理论上是可行的，但是实现起来非常困难
				因为内存块经过使用和归还之后，不一定是顺序的
				两块72字节的内存看似可以切出88字节和56字节，但是在物理内存上
				两块72字节的内存可能不相邻

			整理内存，或者查找相邻空闲内存，这都是可行但不值得尝试的
			因此stl也没有这样做
				
*/
#include <cstddef>
#include <new>
#define __THROW_BAD_ALLOC\
	cerr << "out of memory"<<; exit(1);
	

namespace osuy
{
template<int inst>
class __malloc_alloc_template
{
private:
	static void* oom_malloc(size_t);
	static void* oom_realloc(void* size_t);
	static void (*__malloc_alloc_oom_hander)();

public:
	static void* allocate(size_t n)
	{
		void* result = malloc(n);
		if (0 == result)result = oom_malloc(n);
		return result;
	}

	static void deallocate(void* p, size_t)
	{
		free(p);
	}

	static void* reallocate(void* p, size_t, size_t new_size)
	{
		void* result = realloc(p, new_size);
		if (0 == result)result = oom_realloc(n, new_size);
		return result;
	}

	static void (*set_malloc_handler(void (*f)()))()
	{
		void (*old)() = __malloc_alloc_oom_hander;
		__malloc_alloc_oom_hander = f;
		return old;
	}
};


enum { __ALIGN  = 8};
enum {__MAX_BYTES = 128};
enum { __NFREELISTS = __MAX_BYTES / __ALIGN };
enum { __CHUNK_SIZE = 20 };

template<bool thread, int inst>
class __default_alloc_template
{
private:
	static size_t ROUND_UP(size_t bytes)
	{
		return (((bytes)+__ALIGN - 1) & ~(__ALIGN - 1));
	}

	union obj
	{
		union obj* free_lisi_link;
	};

	static obj* free_list[____NFREELISTS];
	
	static size_t FREELIST_INDEX(size_t bytes)
	{
		return (bytes + __ALIGN - 1) / __ALIGN - 1;
	}

	static void* refill(size_t n)
	{
		char* chunk = chunk_alloc(n, __CHUNK_SIZE);
		obj** my_free_list = free_list + FREELIST_INDEX(n);
		obj* result, current_obj, next_obj;
		int i;

		//result取chunk的第一块，剩余的第一块赋给next_obj和链表头
		result = (obj*)chunk;
		*my_free_list = next_obj = (obj*)(chunk + n);

		for (; (char*)next_obj != start_free;)
		{
			current_obj = next_obj;
			next_obj = next_obj + 1;
			current_obj->free_list_link = next_obj;
		}
		return result;
	}

	static char* chunk_alloc(size_t size, int& nobjs)
	{
		char* result;
		size_t total_bytes = size * nobjs;
		size_t bytes_left = end_free - start_free;

		nobjs = min(nobj, int(bytes_left / size));

		if (nobjs > 0)
		{
			total_bytes = size * nobjs;
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else//备用内存连一块size都且不出，则将备用内存交给对应链表，然后malloc更大块的内存
		{
			if (bytes_left > 0)
			{
				obj** my_free_list = free_list + FREELIST_INDEX(bytes_left);
				((obj*)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj*)start_free;
			}

			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			start_free = (char*)malloc(bytes_to_get);

			//如果malloc也分配不出，则从后面的链表中割一块
			if (0 == start_free)
			{
				int i;
				obj* p, ** my_free_list;
				for (i = size; i <= __MAX_BYTES; i += __ALIGN)
				{
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (0 != p)
					{
						*my_free_list = p->free_list_link;
						start_free = (char*)p;
						end_free = start_free + i;
						return chunk_alloc(size, __CHUNK_SIZE);
					}
				}

				//循环推出，说明连后续链表都割不出一块size
				end_free = 0;
				start_free = (char*)malloc_alloc::allocate(bytes_to_get);
			}

			//malloc成功了，
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return chunk_alloc(size, __CHUNK_SIZE);
		}
	}

	static char* start_free;
	static char* end_free;
	static size_t heap_size;

public:
	static void* allocate(size_t n)
	{
		obj** my_free_list;
		obj* result;

		//当内存需求大于128时，用一级分配器
		if (n > (size_t)__MAX_BYTES)
		{
			return (malloc_alloc::allocate(n));
		}

		//找到对应的链表头，如果为空，则填充
		//填充后会返回一块内存，而且该链表会填入19个内存块
		my_free_list = free_list + FREELIST_INDEX(n);
		result = *my_free_list;
		if (0 == result)
		{
			void* r = refill(ROUND_UP(n));
			return r;
		}

		//如果不为空，则头删取出第一块内存
		*my_free_list = result->free_lisi_link;
		return result;
	}

	static void deallocate(void* p, size_t n)
	{
		obj* q = (obj*)p;
		obj** my_free_list;

		//回收的内存大于128字节，用一级分配器回收
		if (n > (size_t)__MAX_BYTES)
		{
			malloc::deallocate(p);
			return;
		}

		//头插法插入到对应链表中
		my_free_list = free_list + FREELIST_INDEX(n);
		q->free_list_link = *my_free_list;
		*my_free_list = q;
	}

	static void* reallocate(void* p, size_t old_size, size_t new_size);
};

}