enum { __ALIGN = 8 };
enum { __MAX_BYTES = 128 };
enum { __NFREELISTS = __MAX_BYTES / __ALIGN };

template <bool threads,int inst>
class __default_alloc_template {

private:
	//��bytes����ȡ8�ı���
	//byte+7���ٺ�7ȡ����ֵ���������
	//��Ϊ7�Ķ�����ȡ����000����bytes+7������������൱�ڽ������λ���㣬������8�ı���
	static size_t ROUND_UP(size_t bytes) {
		return (((bytes)+__ALIGN - 1) & ~(__ALIGN - 1));
	}

private:
	//������obj�ں���ָ������obj��ָ�롢һ������
	//��һ���ڴ��д������ʱ��ָ�뱻���ǣ����ڴ���free_list������
	//���ڴ���ͷź�����д��ָ�룬���ڴ����½���free_list
	union obj {
		union obj* free_list_link;
		char client_data[1];
	};

private:
	//volatile���εı�������ȡʱ��ֻ�ܴ��ڴ��ַ�ж�ȡ�������ᱻ�������Ż�
	static obj* volatile free_list[__NFREELISTS] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};

	//��ȡbytes��С���ڴ�Ӧ�ô�����һ��free_list�У����ص����±꣬0��1��2...��Ӧ8��16��24...
	//bytes=8ʱ�������±�Ϊ0��free_list�У���bytes=9�������±�Ϊ1��free_list��
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

		if (n > (size_t)__MAX_BYTES) {//���n����128�͵���һ��������
			return (malloc_alloc::allocate(n));
		}

		my_free_list = free_list + FREELIST_INDEX(n);//�Ҷ��Ӧ��free_list
		result = *my_free_list;//resultָ���Ӧ��free_list

		if (result == 0) {//���resultû���ҵ����õ�С���ڴ棬�ͽ�free_list�������
			void* r = refill(ROUND_UP(n));
			return r;
		}

		*my_free_list = result->free_list_link;//result��ָ��С�鱻ʹ�ã�my_free_listָ����һ��С��
		return (result);
	}

	static void deallocate(void* p, size_t n) {
		obj* q = (obj*)p;
		obj* volatile* my_free_list;

		if (n > (size_t)__MAX_BYTES) {
			malloc_alloc::deallocate(p, n);
			return;
		}

		my_free_list = free_list + FREELIST_INDEX(n);//my_free_list�ҵ���Ӧ��free_list
		q->free_list_link = *my_free_list;//q�Ŀ�ָ��ָ��my_free_list��
		*my_free_list = q;//my_free_list��ָ��q������q�ͽ���free_list��������
	}

	static void* reallocate(void* p, size_t old_sz, size_t new_sz) {
	}
};


