#pragma once
#include <vector>
/*
	loki allocator������֮���ܼ򵥾���˵������������������������ָ��
	��Ȼ��������ϸ�ڣ�����һ���ڴ��ֻ�ܱ����յ����������Ǹ�chunk
	��˲���cookieҲ�ܹ�ʹ�ڴ��ϲ���
	�����ǹ黹�ڴ�ʱҪ��һѰ�ҵ�����chunk����ȻҲ����������
	����Ϊ������ͷ��ڴ������ڴ潻���ģ�����Щ��
	���ȶ��ڴ����ĸ�chunk���Ǵ���ֵ�Ƚϣ�������cpu�ڣ��ڴ��޹أ����Էǳ���
	��Ҫ��Ϊ��������ʱ�临�ӶȾ;��ò��ã�

	std.allocator�������ڴ涼���ո�free_list��������free_list�м���malloc��֪��
	ʹ�õ��ǵ��������Բ�֪��ǰһ���ڴ棻��һ���ڴ�Ҳ��һ������
	��loki��ÿ��chunk����һ��malloc���䣬���ҹ黹�ڴ�Ҫ�ҵ�������chunk
	��ʱ��ͨ��ͳ��������ͷŵĴ���������֪��chunk�Ƿ�ȫ�黹��ȫ�黹��chunkҲ�Ϳ����ͷŵ����н��л�
*/

class chunk
{
	unsigned char* _data = nullptr;
	unsigned short _first_available_block = 0; // �����ڴ������
	unsigned short _available_blocks = 0; // ʣ������ڴ�����

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
		_first_available_block = 0; // ��ʼ��һ�������ڴ���0���ڴ�
		_available_blocks = blocks;// ʣ������ڴ������� blocks 

		unsigned char i = 0;
		unsigned char* p = _data;
		for (; i != blocks; p += block_size)
		{
			*p = ++i;// ����ÿ���ڴ��ǰ1���ֽڴ������ 1~block_size. Ϊʲô���Ǵ�0��ʼ
			// ��Ϊ��ŵ��� ��һ�������ڴ�����������0���ڴ����һ�������ڴ���1
			// block_size - 1���ڴ�û����һ�������ڴ棬������Ȼд��block_size
		}
	}

	void release()
	{
		delete[] _data;
	}

	void* allocate(size_t block_size)
	{
		if (0 == _available_blocks)return nullptr;

		// ָ���һ������ڴ棺data�׵�ַ + ��һ������ڴ������ * ���С
		unsigned char* result = _data + (_first_available_block * block_size);

		// ��ȡ�ڴ��ĵ�һ���ֽڣ���Ϊ��������һ������ڴ������
		_first_available_block = *result;
		--_available_blocks;

		return result;
	}

	void deallocate(void* p, size_t block_size)
	{
		auto to_release = static_cast<unsigned char*>(p);

		// �ѵ�ǰ�ĵ�һ�������ڴ����������黹�ڴ�ĵ�һ���ֽ�
		*to_release = _first_available_block;

		// Ҫ�ͷŵ��ڴ������ = (Ҫ�ͷŵ��ڴ�ĵ�ַ - data�׵�ַ) / ���С
		// �ٸ�����ǰ�ĵ�һ�������ڴ�����
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
					// Ϊ��Ҫ���� _dealloc_chunk ��
					// ��Ϊ��vector ���Ԫ�صĶ��������ܻᷢ������
					// ���Ծɵ�����Ԫ��ָ����ʧЧ�Ŀ����ԣ�����ֻ����Ϊָ��front

					_alloc_chunk->init(block_size, blocks);
					break;
				}

				if (it->get_available_blocks() > 0)
				{
					_alloc_chunk = &(*it); // �Ƚ����õ��������õ�chunk&����ȡ��ַ�õ�chunk*
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

		auto lo = _dealloc_chunk;// ������ǰ����
		auto hi = _dealloc_chunk + 1;// ����������
		auto lo_bound = &_chunks.front();// ǰ�߽�
		auto hi_bound = &_chunks.back() + 1;// ��߽�

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

		// _dealloc_chunk �����Ƿ�ȫ�黹�����ȫ�黹�ˣ�����Ҫ�ͷ�
		// ���ǲ�һ���ͷ� _dealloc_chunk����Ϊ���д��ڶ����ͷ�
		if (_dealloc_chunk->get_available_blocks() == blocks)
		{
			auto& last_chunk = _chunks.back();
			if (&last_chunk == _dealloc_chunk)
			{
				// ������һ��chunk����_dealloc_chunk������������һ��chunkҲȫ�黹��
				// ��黹last chunk
				// Ϊʲô��ô�����أ��������
				if (_chunks.size() > 1 && _dealloc_chunk[-1].get_available_blocks() == blocks)
				{
					last_chunk.release();
					_chunks.pop_back();
					_alloc_chunk = _dealloc_chunk = &_chunks.front();
				}
				return;
			}

			// ���last chunkҲȫ�黹�ˣ��ͷ�last chunk����������
			if (last_chunk.get_available_blocks() == blocks)
			{
				last_chunk.release();
				_chunks.pop_back();
				_alloc_chunk = _dealloc_chunk;
			}
			// ���� _dealloc_chunk �� last chunk ����
			else
			{
				std::swap(*_dealloc_chunk, last_chunk);
				_alloc_chunk = &_chunks.back();
			}
		}

		/*
			do_deallocate����Ϊ��Щ�ѽ�
			���� p �黹���� _dealloc_chunk�����_dealloc_chunkû��ȫ�黹�������·���
			����_dealloc_chunkȫ�黹�ˣ���ô��Ҫ��������ͷ�
			����vector���м�ɾ��Ч�ʲ��ѣ�����Ҫ�ͷŵ�Ŀ���������һ��chunk
			����_dealloc_chunkǡ�þ������һ��chunk����ô˵��������Ҫ���ͷŵ�Ŀ��
			������ΪҪ���Թ黹�����Լ��һ��������һ���Ƿ�ȫ�黹��
			��ȫ�黹�ˣ��ã�_dealloc_chunk ���µ�ȷ��Ҫ���ͷ��ˣ����������ͷ�
			Ȼ�� _dealloc_chunk �� _alloc_chunk ��ָ��������Ԫ�أ���return
			_dealloc_chunk��Ϊ���ͷ��ˣ�����Ҫ��������Ϊʲô_alloc_chunkҲҪ������
			�Լ�Ϊʲô����������Ԫ�أ�
				_alloc_chunk����ָ��lastchunk������Ҳ��Ҫ�޸ģ������޸ĵ��δ�ȴ���Ѷ���
				���Ըɴ�ָ����Ԫ�ذ�
					
			��_dealloc_chunk�������һ��chunk����ȥ�������һ��chunk�Ƿ�ȫ�黹��
			���һ��chunkȫ�黹�ˣ����ͷŲ�pop��ͬ��������_alloc_chunk����ָ��last chunk
			����Ҫ��������ʱΪ��Ҫ������_dealloc_chunk�أ���Ϊ_alloc_chuk���ָ��һ���п����ڴ��chunk
			��_dealloc_chunk��Ϊȫ�黹�ˣ����������п����ڴ档�������治һ��
			�����last chunk δȫ�黹����_dealloc_chunk��last chunk����
			�������ܱ�֤ȫ�黹��chunk���ǳ���������ĩβ
			ͬ��������_alloc_chunk����ָ��last chunk
			����Ҫ��������ʱlast chunk Ҳ��_dealloc_chunkһ���п����ڴ棬����_alloc_chunkȥָ����

			������δ������bug

		*/

		// �°汾loki��ʵ��
		if (_dealloc_chunk->get_available_blocks() == blocks)
		{
			chunk* empty_chunk; // ��һ����Ա������ָ��һ��ȫ���յ�chunk�����û�У���nullptr
			if (!empty_chunk)
			{
				// empty chunk �ǿգ����������ͷ�

				// ���_dealloc_chunk�������һ����������ָ�� empty chunk��Ȼ���ȥ�ͷ����һ��chunk
				auto last_chunk = &_chunks.back();
				if (last_chunk == _dealloc_chunk)
				{
					_dealloc_chunk = empty_chunk;
				}
				// _dealloc_chunk �������һ���� empty chunkҲ���ǣ��� empty chunk ���������һ��ȥ
				else if (last_chunk != empty_chunk)
				{
					std::swap(*last_chunk, *empty_chunk);
				}

				// �������һ��chunk ������
				last_chunk->release();
				_chunks.pop_back();

				// ����alloca_chunk
				if (_alloc_chunk == last_chunk || _alloc_chunk->get_available_blocks() == 0)
				{
					_alloc_chunk = _dealloc_chunk;
				}
			}

			// empty chunk Ϊ�� �����Ѿ��ͷ��ˣ�ָ�� _dealloc_chunk 
			// ��_dealloc_chunk ����ָ��һ��ȫ���յ�chunk�����Կ��Ը�ֵ�� empty chunk
			empty_chunk = _dealloc_chunk;
		}
	}
};