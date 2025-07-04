#pragma once
#include <vector>
/*
	loki allocator的优秀之处很简单就能说明：用数组和索引代替链表和指针
	当然还有其他细节，例如一个内存块只能被回收到它所属的那个chunk
	如此不用cookie也能够使内存块合并。
	代价是归还内存时要逐一寻找到所属chunk，当然也不慢就是了
	（因为申请和释放内存是与内存交互的，会慢些；
	而比对内存在哪个chunk里是纯数值比较，发生在cpu内，内存无关，所以非常快
	不要因为它增加了时间复杂度就觉得不好）

	std.allocator的所有内存都回收给free_list，至于是free_list有几次malloc不知道
	使用的是单链表，所以不知道前一块内存；后一块内存也不一定相邻
	而loki的每个chunk都由一次malloc分配，并且归还内存要找到所属的chunk
	此时再通过统计申请和释放的次数，就能知道chunk是否全归还，全归还的chunk也就可以释放掉，有借有还
*/

class chunk
{
	unsigned char* _data = nullptr;
	unsigned short _first_available_block = 0; // 可用内存的索引
	unsigned short _available_blocks = 0; // 剩余可用内存数量

public:
	auto get_available_blocks()const { return _available_blocks; }
	auto get_data()const { return _data; }

	void init(size_t block_size, unsigned char blocks)
	{
		_data = new unsigned char[block_size * blocks];
		reset(block_size, blocks);
	}

	void reset(size_t block_size, unsigned char blocks)
	{
		_first_available_block = 0; // 起始第一个可用内存是0号内存
		_available_blocks = blocks;// 剩余可用内存数量是 blocks 

		unsigned char i = 0;
		unsigned char* p = _data;
		for (; i != blocks; p += block_size)
		{
			*p = ++i;// 借用每块内存的前1个字节存放索引 1~block_size. 为什么不是从0开始
			// 因为存放的是 下一个可用内存索引，所以0号内存的下一个可用内存是1
			// block_size - 1号内存没有下一个可用内存，但是仍然写入block_size
		}
	}

	void release()
	{
		delete[] _data;
	}

	void* allocate(size_t block_size)
	{
		if (0 == _available_blocks)return nullptr;

		// 指向第一块可用内存：data首地址 + 第一块可用内存的索引 * 块大小
		unsigned char* result = _data + (_first_available_block * block_size);

		// 读取内存块的第一个字节，因为它就是下一块可用内存的索引
		_first_available_block = *result;
		--_available_blocks;

		return result;
	}

	void deallocate(void* p, size_t block_size)
	{
		auto to_release = static_cast<unsigned char*>(p);

		// 把当前的第一个可用内存索引赋给归还内存的第一个字节
		*to_release = _first_available_block;

		// 要释放的内存的索引 = (要释放的内存的地址 - data首地址) / 块大小
		// 再赋给当前的第一个可用内存索引
		_first_available_block = static_cast<unsigned char>((to_release - _data) / block_size);

		++_available_blocks;
	}
};

class fixed_allocator
{
	std::vector<chunk> _chunks;
	chunk* _alloc_chunk = nullptr;
	chunk* _dealloc_chunk = nullptr;

public:
	void* allocate()
	{
		if (nullptr == _alloc_chunk || 0 == _alloc_chunk->get_available_blocks())
		{
			auto it = _chunks.begin();

			for (;; ++it)
			{
				if (_chunks.end() == it)
				{
					_chunks.emplace_back();
					_alloc_chunk = &_chunks.back();
					_dealloc_chunk = &_chunks.front();
					// 为何要重设 _dealloc_chunk ？
					// 因为往vector 添加元素的动作，可能会发生扩容
					// 所以旧的所有元素指针有失效的可能性，于是只好设为指向front

					_alloc_chunk->init(block_size, blocks);
					break;
				}

				if (it->get_available_blocks() > 0)
				{
					_alloc_chunk = &(*it); // 先解引用迭代器，得到chunk&，再取地址得到chunk*
					break;
				}
			}
		}
		return _alloc_chunk->allocate(block_size);
	}

	void deallocate(void* p)
	{
		_dealloc_chunk = vicinity_find(p);
		do_deallocate(p);
	}

	chunk* vicinity_find(void* p)
	{
		const size_t chunk_length = block_size * blocks;

		auto lo = _dealloc_chunk;// 负责向前查找
		auto hi = _dealloc_chunk + 1;// 负责向后查找
		auto lo_bound = &_chunks.front();// 前边界
		auto hi_bound = &_chunks.back() + 1;// 后边界

		for (;;)
		{
			if (lo)
			{
				if (p >= lo->get_data() && p < lo->get_data() + chunk_length)
				{
					return lo;
				}

				if (lo == lo_bound)lo = nullptr;
				else --lo;
			}

			if (hi)
			{
				if (p >= hi->get_data() && p < hi->get_data() + chunk_length)
				{
					return hi;
				}

				if (++hi == hi_bound)hi = nullptr;
			}
		}

		return nullptr;
	}

	void do_deallocate(void* p)
	{
		_dealloc_chunk->deallocate(p, block_size);

		// _dealloc_chunk 区块是否全归还，如果全归还了，则需要释放
		// 但是不一定释放 _dealloc_chunk。因为当中存在惰性释放
		if (_dealloc_chunk->get_available_blocks() == blocks)
		{
			auto& last_chunk = _chunks.back();
			if (&last_chunk == _dealloc_chunk)
			{
				// 如果最后一个chunk就是_dealloc_chunk，并且它的上一个chunk也全归还了
				// 则归还last chunk
				// 为什么这么自信呢？结合下文
				if (_chunks.size() > 1 && _dealloc_chunk[-1].get_available_blocks() == blocks)
				{
					last_chunk.release();
					_chunks.pop_back();
					_alloc_chunk = _dealloc_chunk = &_chunks.front();
				}
				return;
			}

			// 如果last chunk也全归还了，释放last chunk，数组缩减
			if (last_chunk.get_available_blocks() == blocks)
			{
				last_chunk.release();
				_chunks.pop_back();
				_alloc_chunk = _dealloc_chunk;
			}
			// 否则 _dealloc_chunk 与 last chunk 互换
			else
			{
				std::swap(*_dealloc_chunk, last_chunk);
				_alloc_chunk = &_chunks.back();
			}
		}

		/*
			do_deallocate的行为有些费解
			首先 p 归还给了 _dealloc_chunk，如果_dealloc_chunk没有全归还，那无事发生
			但是_dealloc_chunk全归还了，那么就要处理惰性释放
			由于vector从中间删除效率不佳，所以要释放的目标总是最后一个chunk
			假如_dealloc_chunk恰好就是最后一个chunk，那么说明它就是要被释放的目标
			但是因为要惰性归还，所以检查一下它的上一个是否全归还。
			是全归还了，好，_dealloc_chunk 这下的确是要被释放了，从数组里释放
			然后 _dealloc_chunk 和 _alloc_chunk 都指向数组首元素，再return
			_dealloc_chunk因为被释放了，所以要调整，但为什么_alloc_chunk也要调整？
			以及为什么都调整到首元素？
				_alloc_chunk可能指向lastchunk，所以也需要修改，但是修改到何处却很难定夺
				所以干脆指向首元素吧
					
			若_dealloc_chunk不是最后一个chunk，那去看看最后一个chunk是否全归还，
			最后一个chunk全归还了，则释放并pop，同样，由于_alloc_chunk可能指向last chunk
			所以要调整，这时为何要调整到_dealloc_chunk呢？因为_alloc_chuk最好指向一个有可用内存的chunk
			而_dealloc_chunk因为全归还了，毫无疑问有可用内存。这点和上面不一样
			而如果last chunk 未全归还，则_dealloc_chunk和last chunk互换
			这样就能保证全归还的chunk总是出现在数组末尾
			同样，由于_alloc_chunk可能指向last chunk
			所以要调整，此时last chunk 也即_dealloc_chunk一定有可用内存，所以_alloc_chunk去指向它

			但是这段代码存在bug

		*/

		// 新版本loki的实现
		if (_dealloc_chunk->get_available_blocks() == blocks)
		{
			chunk* empty_chunk; // 是一个成员变量，指向一个全回收的chunk，如果没有，则nullptr
			if (!empty_chunk)
			{
				// empty chunk 非空，触发惰性释放

				// 如果_dealloc_chunk就是最后一个，则让它指向 empty chunk，然后会去释放最后一个chunk
				auto last_chunk = &_chunks.back();
				if (last_chunk == _dealloc_chunk)
				{
					_dealloc_chunk = empty_chunk;
				}
				// _dealloc_chunk 不是最后一个， empty chunk也不是，则将 empty chunk 互换到最后一个去
				else if (last_chunk != empty_chunk)
				{
					std::swap(*last_chunk, *empty_chunk);
				}

				// 总是最后一个chunk 被回收
				last_chunk->release();
				_chunks.pop_back();

				// 调整alloca_chunk
				if (_alloc_chunk == last_chunk || _alloc_chunk->get_available_blocks() == 0)
				{
					_alloc_chunk = _dealloc_chunk;
				}
			}

			// empty chunk 为空 或者已经释放了，指向 _dealloc_chunk 
			// 而_dealloc_chunk 比如指向一个全回收的chunk，所以可以赋值给 empty chunk
			empty_chunk = _dealloc_chunk;
		}
	}
};