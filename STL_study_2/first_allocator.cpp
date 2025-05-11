#if 0
#	include<new>
#	define __THROW_BAD_ALLOC throw_bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#	include<iostream>
#	define __THROW_BAD_ALLOC cerr<<"out of memory" <<endl;exit(1)//实现一个宏函数
#endif

template <int inst>
class __malloc_alloc_template {

private:
	//提供两个oom函数和一个函数指针
	static void* oom_malloc(size_t);
	static void* oom_malloc(void*,size_t);
	static void (*__malloc_alloc_oom_handler)();

public:
	//配置内存的函数，返回内存首地址
	static void* allocate(size_t n) {//第一级配置器直接使用malloc
		void* result = malloc(n);
		if (0 == result)result = oom_malloc(n);//如果内存不足，调用oom_malloc
		return result;
	}

	//释放p的内存地址
	static void deallocate(void* p, size_t /* n */) {//第一级配置器直接使用free释放内存
		free(p);
	}

	static void* reallocate(void* p, size_t /* old_sz*/, size_t new_sz) {
		void* result = realloc(p, new_sz); //第一级配置器直接使用realloc
		if (result == 0)result = oom_realloc(p, new_sz);//如果内存不足，调用oom_realloc
		return result;
	}

	static void (*set_malloc_handler(void(*f)()))(){//公开函数指针，无参数，，将void*返回类型的函数指针f作为失败调用函数
		void (*old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return (old);
	}
};

template <int inst>
void (* __malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;//函数指针置0

template <int inst>
void* __malloc_alloc_template<inst>::oom_malloc(size_t n) {
	//一个函数指针和void*指针result
	void (*my_malloc_handler)();
	void* result;

	for (;;) {//不断尝试释放、配置、再释放、再配置，直到result成功分配
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