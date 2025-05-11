#pragma once
/*
	ǰ���Ѿ������˻���union obj�������ڴ��
	��Ҳ��stl��pool allocator�Ļ���˼·
	���ּ���Ҳ��ΪǶ��ָ�룬������е��ڴ棨��δ������ݣ��н�4���ֽ�����Ϊָ��
		�������ڴ汻����ʱ�����ݻḲ����ȥ�����ڴ�黹ʱ�����ݲ�����Ҫ������
		��4���ֽ���Ϊָ�룬ͷ�巨���뵽�����ڴ�������

	
	�ɴ�Ҳ������һ��union��ʹ��˼�룺
		union�ʺ�����ֻд���������Ϊ������д��Ḳ��
		����ȡ�Ļ������ڲ�֪���ĸ���Ա����Ч�ģ������Ҫ�����ⲿ��ǣ�
		������Ҫ����Ŀռ䣬����ʹ��union��Ŀ�ı���������

	
	pool allocaterʵ��ϸ��
		pool allocator�ڲ���ά����һ��free_list��һ��16��Ԫ��
		��ÿ��Ԫ�ش�����ָ�Ŀ����ڴ�Ĺ�ģ�������������8 �� 128
		���� 0Ԫ��ָ���ֽ�Ϊ8�Ŀ����ڴ�����
			1Ԫ��ָ���ֽ�Ϊ16�Ŀ����ڴ�����
			...
			15��Ԫ��ָ���ֽ�Ϊ128�Ŀ����ڴ�����
			�����û�������ڴ泬��128ʱ��������Ϊcookie΢����΢�����Ǽ���ʹ��Ĭ��allocator

			����������к������أ�
				���ȣ�ά��һ�������С���ڴ������ǲ���ʵ�ʵģ�ֻ�ֶܷ�ά��
				8~128������Ϊ8������Ҫ16��ָ��Ķ��⿪���������Ҫ���ⳤ�ȣ�
				�Ǿ���120��ָ�룬����Щ��С���ڴ���ܼ��������õ���
				��ˣ�8~128������Ϊ8�Ѿ��ܹ����������ڴ��������󣬶���16��ָ��
				64�ֽڵ��ڴ�Ҳ����̫�˷�

				��ô����������һ��֮ǰû���������⣺
				֮ǰ��per class allocator��һ����һ���أ������Ǻ��ڴ�أ�Ҳ��
				���Լ�������new�����䣬���ع�ϵ�ڴ�Ĵ�С
				����������һ��free_list�ڴ�أ��黹�ڴ�ʱ����Ҫ�������С�����Ӧ�Ŀ���������
				��˾���Ҫʹ���߼�¼Ҫ�˶����ֽڵ��ڴ棬�黹ʱҲҪ�黹��Ӧ�ֽڵ��ڴ�
				������Ҫ����ڿ��̡����Ƿ����˶����ֽ����������

				�����Ϊʲô�Ƽ�ֻ��ʹ������ʱ��pool allocator
				��Ϊ�����ڵ�ÿ��Ԫ�ض���ͬ����С�������������͹黹�����Ƕ����ֽ�
		
		Ȼ�󣬳���Ա��Ҫ���������ֽ��ڴ�
			�Ƚ����ϵ���8�ı��������磬28 �ϵ���32����3������
			����������3�������Ƿ��п����ڴ�
			����У�ͷɾȡ����һ���ڴ棬����ָ�������Ա
			���û�У����ұ����ڴ棬���ڱ����ڴ���0�ֽ�
			������malloc���� 2 * 20 * 32 һ��1280�ֽڵ��ڴ�
			˼���ǣ�����20��ô�С���ڴ棬����Ҳ���٣�����Ҳ����̫�˷�
			��20���ڴ潻��3��������û��ʣ���ˣ��������Ҫ������С����Ҫ�ٴ�malloc
			��˷���������Ҳ����40���ڴ�1280�ֽڣ�����20���ڴ潻������ʣ��640�ֽ����ű���

		����Ա����Ҫ����48�ֽڵ��ڴ�
			����5�������У�ͷɾ���ص�һ�飻û�в鿴�����ڴ棬����640�ֽ�
			���ǽ������ڴ��г�48�ֽڣ��г�13������5�����������ڴ�ʣ��16�ֽ�

		����Ա����Ҫ����96�ֽڵ��ڴ�
			�鿴11�������У�ͷɾ���ص�һ�飻û�в鿴�����ڴ棬����16�ֽڣ�һ�鶼����
			���ǽ���16���ֽ���Ϊ1�飬����1������Ȼ����malloc����
			����������н����ģ�
				����2 * 20 * 96 = 3840�ֽڣ��ڴ�ػ��ж���Ŀ��ǣ�
				��һ������У��ڴ���������������Ǿ��ȵģ�����Խ��Խ��
				����������һ����Խ������������Խ����ˣ�׷���ѷ�����/16���ڴ�
				�ѷ�����1280�ֽڣ�����16��80�ֽ�
				���ǹ�����3920�ֽ�
			ȡ��20��96�ֽڽ���11�����������ڴ�ʣ��2000�ֽ�

		��������Ҫ����8�ֽ��ڴ�
			����0�������У�ͷɾ���ص�һ�飻û�в鿴�����ڴ棬����2000�ֽ�
			�г�160�ֽڣ�����20*8����0��������һ�鷵�ظ�����Ա
			�����ڴ�1840�ֽ�

		��������Ҫ����104�ֽڵ��ڴ�
			����12�������У�ͷɾ���ص�һ�飻û�У��鿴�����ڴ棬����1840�ֽ�
			�г�17 * 104 = 1768�ֽڣ�һ�鷵�ظ�����Ա��ʣ�മ��������12������
			�����ڴ�72�ֽ�

		����˼��һ������ô���ܳ��ֵ����⣺
			��ǰ�ܹ�������5200�ֽ��ڴ棬���裬ϵͳ�ڴ�һ��ֻ��5400��
			ֻ��200�ֽڿ����ˣ���ô��һ�η���ͻ�ʧ��
			������һ����Ҫ112�ֽ��ڴ棬13������Ϊ�գ������ڴ�72�ֽ�
			���ǽ�72�ֽ���Ϊһ�齻��8������Ȼ�����
			2 * 20 * 112 + 5200 / 16 = 773�ֽڣ���������ֻʣ200�ֽڣ������ʧ��

			�����ֽ���취��
			1.����200�ֽڣ���Ҫ112�ֽڣ����Ƿ���Ҫ773�ֽڣ�
				���Խ�773ѭ�����룬ֱ��С��200������112���Ϳ��Է���ɹ�
				��ȻҲ���ܻ��ǲ��ɹ�
			2.��14��15���������һ���ڴ档
				��ǰ����15��15������û�У����Ǽ�����Ҫ�Ĳ���112�ֽڣ���88�ֽ�
				�Ϳ�����96�ֽڵ��������

			
			96�ֽڣ���11������Ŀǰ��19���ڴ洦�ڿ��У�������Ҫһ��88�ֽڵ��ڴ棬
			����ϵͳ�ڴ�ľ����޷�����ɹ�
			�������ʮ�����Σ�
				�����������ɹ������������ڴ�
				��ϵͳ�ڴ�ľ����ٺ�����������ʧ��
				һ���Ա�֮������ռ��é�Ӳ���ʺ
			Ϊ�˴�����������ķ�����stl��pool allocator�������ǣ�
				������������п����ڴ����С����ȡһ�����
				��Ҫ88�ֽڣ���96�ֽڵ�����ȡһ���ڴ棬����88�ֽڷ��ظ�����Ա
				ʣ��8�ֽ�ͷ���0������
			���������ܳ�������Ѿ����䵫�Ǵ��ڿ��е��ڴ�

			���ǻ᲻�ᣬֻ��С��������ڴ洦�ڿ��У���
				����88�ֽڣ����Ǵ���88�ֽڵ�����Ϊ�գ�ֻ��С��88�ֽڵ������п���
			
			�ܲ��ܰ�С���ڴ��ϲ����г�88�ֽڣ�
				���������������ǿ��еģ�����ʵ�������ǳ�����
				��Ϊ�ڴ�龭��ʹ�ú͹黹֮�󣬲�һ����˳���
				����72�ֽڵ��ڴ濴�ƿ����г�88�ֽں�56�ֽڣ������������ڴ���
				����72�ֽڵ��ڴ���ܲ�����

			�����ڴ棬���߲������ڿ����ڴ棬�ⶼ�ǿ��е���ֵ�ó��Ե�
			���stlҲû��������
				
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

		//resultȡchunk�ĵ�һ�飬ʣ��ĵ�һ�鸳��next_obj������ͷ
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
		else//�����ڴ���һ��size���Ҳ������򽫱����ڴ潻����Ӧ����Ȼ��malloc�������ڴ�
		{
			if (bytes_left > 0)
			{
				obj** my_free_list = free_list + FREELIST_INDEX(bytes_left);
				((obj*)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj*)start_free;
			}

			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			start_free = (char*)malloc(bytes_to_get);

			//���mallocҲ���䲻������Ӻ���������и�һ��
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

				//ѭ���Ƴ���˵���������������һ��size
				end_free = 0;
				start_free = (char*)malloc_alloc::allocate(bytes_to_get);
			}

			//malloc�ɹ��ˣ�
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

		//���ڴ��������128ʱ����һ��������
		if (n > (size_t)__MAX_BYTES)
		{
			return (malloc_alloc::allocate(n));
		}

		//�ҵ���Ӧ������ͷ�����Ϊ�գ������
		//����᷵��һ���ڴ棬���Ҹ����������19���ڴ��
		my_free_list = free_list + FREELIST_INDEX(n);
		result = *my_free_list;
		if (0 == result)
		{
			void* r = refill(ROUND_UP(n));
			return r;
		}

		//�����Ϊ�գ���ͷɾȡ����һ���ڴ�
		*my_free_list = result->free_lisi_link;
		return result;
	}

	static void deallocate(void* p, size_t n)
	{
		obj* q = (obj*)p;
		obj** my_free_list;

		//���յ��ڴ����128�ֽڣ���һ������������
		if (n > (size_t)__MAX_BYTES)
		{
			malloc::deallocate(p);
			return;
		}

		//ͷ�巨���뵽��Ӧ������
		my_free_list = free_list + FREELIST_INDEX(n);
		q->free_list_link = *my_free_list;
		*my_free_list = q;
	}

	static void* reallocate(void* p, size_t old_size, size_t new_size);
};

}