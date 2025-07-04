#pragma once
#include <stdlib.h>
/*
	per-class allocator 2 ������Ѿ��㹻����
	��ô�������ٵ������ǣ�����ÿ����Ҫ�õ��ڴ�ص��࣬����Ҫ����operator new �� operator delete
	�������ǵĴ��뼸��һ��
	Ϊ����߸����ԣ����һ��ר��ʵ���ڴ�ص��࣬������Ҫ�ڴ�ص����operator new �� operator delete ���������ĺ���

	����ʵ����������ͬ
	���ڲ�֪����������ͣ����Բ���ʹ��union����˼��û��
	��Ϊ�����ڴ�ʱһ���ᴫ����Ĵ�С�������Ըô�СΪ���������������
*/

class allocator
{
	enum { CHUNK = 5 };// �ش�С��Ϊ5�����˳�򴴽�����ôÿ������������ڵ�
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
				p->next = (obj*)((char*)p + size);// תΪchar* ��ǰ��size���ֽڡ��൱��ָ����һ�������ͷ��
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
	���ǿ��Ը�ÿ��Ҫʹ�÷��������඼�������µ�operator new/delete
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

// ��̬��Ա���������ⶨ��
allocator ClassD::alloc{};

/*
	��Ȼ���ظ���д���ϵĴ��룬���ǿ��Է��֣��������ⶨ��ľ�̬��Ա����
	�������д��붼���������޹صģ����Կ���ʹ��һ�к�����װ
	��������ľ�̬��Ա�����Ķ��壬Ҳ����ʹ�ú꣬����Ϊ����Ҫд������

	���£����ֻ�����к��ʹ��������ڴ�ع���
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
	�汾1�����Լ�ʵ������ͳ�
	�汾2������union�Ż����ı�����Ķ���ָ��
	�汾3��ר�ŵ���ʵ���ڴ�ز������������
	�汾4�����װ
*/
