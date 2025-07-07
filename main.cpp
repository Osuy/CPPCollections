#pragma once
#include <iostream>
#include <deque>


template<typename T>
class pool_allocator
{
	struct data_t
	{
		union
		{
			char buf[sizeof(T)] = {};
			size_t next_available;
			T t;
		};

		~data_t() {}
	};
	std::deque<data_t> pool;
	size_t first_available = 0;
	size_t available_blocks = 0;

	void init()
	{
		const auto end = first_available + available_blocks;
		for (auto i = first_available; i < end; ++i)
		{
			pool[i].next_available = i + 1;
		}
	}

public:
	pool_allocator() = default;
	pool_allocator(size_t size)
		: pool{ size }
		, available_blocks{ size }
	{
		init();
	}

	void* allocate(size_t size)
	{
		if (0 == available_blocks)
		{
			const auto chunk = std::max(pool.size(), 23ull);
			available_blocks = chunk * 3 - pool.size();
			pool.resize(chunk * 3);
			init();
		}

		auto& result = pool[first_available];

		first_available = result.next_available;
		--available_blocks;

		return &result;
	}

	void deallocate(void* p, size_t)
	{
		auto it = std::find_if(pool.begin(), pool.end(), [p](const data_t& el) { return &el == p; });
		if (pool.end() == it)
		{
			throw std::exception{ "bad deallocate" };
		}

		auto to_release = static_cast<data_t*>(p);
		to_release->next_available = first_available;

		first_available = std::distance(pool.begin(), it);
		++available_blocks;
	}
};


#define DECLARE_POOL_ALLOC(Class)\
protected:\
	static pool_allocator<Class>& get_pool_allocator()\
	{\
		static pool_allocator<Class> alloc;\
		return alloc;\
	}\
public:\
	static void* operator new(size_t size)\
	{\
		return get_pool_allocator().allocate(size);\
	}\
	static void operator delete(void* data, size_t size)\
	{\
		return get_pool_allocator().deallocate(data, size);\
	}

class A
{
	unsigned data[256] = {};
	DECLARE_POOL_ALLOC(A);
public:
	A()
	{
		std::cout << "A::" << __func__ << std::endl;
	}

	~A()
	{
		std::cout << "A::" << __func__ << std::endl;
	}
};

int main()
{
	auto aobj = new A{};
	delete aobj;
	return 0;
}
