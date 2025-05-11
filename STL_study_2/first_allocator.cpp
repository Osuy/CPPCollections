#if 0
#	include<new>
#	define __THROW_BAD_ALLOC throw_bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#	include<iostream>
#	define __THROW_BAD_ALLOC cerr<<"out of memory" <<endl;exit(1)//ʵ��һ���꺯��
#endif

template <int inst>
class __malloc_alloc_template {

private:
	//�ṩ����oom������һ������ָ��
	static void* oom_malloc(size_t);
	static void* oom_malloc(void*,size_t);
	static void (*__malloc_alloc_oom_handler)();

public:
	//�����ڴ�ĺ����������ڴ��׵�ַ
	static void* allocate(size_t n) {//��һ��������ֱ��ʹ��malloc
		void* result = malloc(n);
		if (0 == result)result = oom_malloc(n);//����ڴ治�㣬����oom_malloc
		return result;
	}

	//�ͷ�p���ڴ��ַ
	static void deallocate(void* p, size_t /* n */) {//��һ��������ֱ��ʹ��free�ͷ��ڴ�
		free(p);
	}

	static void* reallocate(void* p, size_t /* old_sz*/, size_t new_sz) {
		void* result = realloc(p, new_sz); //��һ��������ֱ��ʹ��realloc
		if (result == 0)result = oom_realloc(p, new_sz);//����ڴ治�㣬����oom_realloc
		return result;
	}

	static void (*set_malloc_handler(void(*f)()))(){//��������ָ�룬�޲���������void*�������͵ĺ���ָ��f��Ϊʧ�ܵ��ú���
		void (*old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return (old);
	}
};

template <int inst>
void (* __malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;//����ָ����0

template <int inst>
void* __malloc_alloc_template<inst>::oom_malloc(size_t n) {
	//һ������ָ���void*ָ��result
	void (*my_malloc_handler)();
	void* result;

	for (;;) {//���ϳ����ͷš����á����ͷš������ã�ֱ��result�ɹ�����
		my_malloc_handler = __malloc_alloc_oom_handler;
		if (0==my_malloc_handler) { __THROW_BAD_ALLOC; }
		(*my_malloc_handler)();
		result = malloc(n);
		if (result)return (result);
	}
}

template<int inst>
void* __malloc_alloc_template<inst>::oom_malloc(void* p, size_t n) {
	void(*my_malloc_handler)();
	void result;

	for (;;) {
		my_malloc_handler = __malloc_alloc_oom_handler;
		if (0 = my_malloc_handler) { __THROW_BAD_ALLOC; }
		(*my_malloc_handler)();
		result = realloc(p, n);
		if (result)return(result);
	}
}