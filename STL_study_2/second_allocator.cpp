enum { __ALIGN = 8 };
enum { __MAX_BYTES = 128 };
enum { __NFREELISTS = __MAX_BYTES / __ALIGN };

template <bool threads,int inst>
class __default_alloc_template {

private:
	//将bytes向上取8的倍数
	//byte+7，再和7取反的值进行与操作
	//因为7的二进制取反是000，和bytes+7进行与操作，相当于将最低三位归零，正好是8的倍数
	static size_t ROUND_UP(size_t bytes) {
		return (((bytes)+__ALIGN - 1) & ~(__ALIGN - 1));
	}

private:
	//共用体obj内含：指向其他obj的指针、一个数据
	//当一个内存块写入数据时，指针被覆盖，该内存块从free_list中脱离
	//当内存块释放后，重新写入指针，该内存重新进入free_list
	union obj {
		union obj* free_list_link;
		char client_data[1];
	};

private:
	//volatile修饰的变量被读取时，只能从内存地址中读取，而不会被编译器优化
	static obj* volatile free_list[__NFREELISTS] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};

	//获取bytes大小的内存应该存在哪一个free_list中，返回的是下标，0，1，2...对应8，16，24...
	//bytes=8时，存在下标为0的free_list中，当bytes=9，存在下标为1的free_list中
	static size_t FREELIST_INDEX(size_t bytes) {
		return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
	}

	static void* refill(size_t n) {
		int nobjs = 20;
		char* chunk = chunk_alloc(n, nobjs);
		obj* volatile* my_free_list;
		obj* result;
		obj* current_obj, * next_obj;
		int i;

		if (1 == nobjs)return (chunk);

		my_free_list = free_list + FREELIST_INDEX(n);
		result = (obj*)chunk;
		*my_free_list = next_obj = (obj*)(chunk + n);

		for (i = 1;; i++) {
			current_obj = next_obj;
			next_obj = (obj*)((char*)next_obj + n);
			if (nobjs - 1 == i) {
				current_obj->free_list_link = 0;
				break;
			}
			else {
				current_obj->free_list_link = next_obj;
			}
		}
		return (result);
	}


	static char* chunk_alloc(size_t size, int& nobjs)[


	]

	static char* start_free=0;
	static char* end_free=0;
	static size_t heap_size=0;

public:
	static void* allocate(size_t n) {

		obj* volatile* my_free_list;
		obj* result;

		if (n > (size_t)__MAX_BYTES) {//如果n大于128就调用一级配置器
			return (malloc_alloc::allocate(n));
		}

		my_free_list = free_list + FREELIST_INDEX(n);//找多对应的free_list
		result = *my_free_list;//result指向对应的free_list

		if (result == 0) {//如果result没有找到可用的小块内存，就将free_list重新填充
			void* r = refill(ROUND_UP(n));
			return r;
		}

		*my_free_list = result->free_list_link;//result所指的小块被使用，my_free_list指向下一个小块
		return (result);
	}

	static void deallocate(void* p, size_t n) {
		obj* q = (obj*)p;
		obj* volatile* my_free_list;

		if (n > (size_t)__MAX_BYTES) {
			malloc_alloc::deallocate(p, n);
			return;
		}

		my_free_list = free_list + FREELIST_INDEX(n);//my_free_list找到对应的free_list
		q->free_list_link = *my_free_list;//q的块指针指向my_free_list，
		*my_free_list = q;//my_free_list再指向q，于是q就进入free_list，被回收
	}

	static void* reallocate(void* p, size_t old_sz, size_t new_sz) {
	}
};


