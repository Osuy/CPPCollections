#pragma once
#include <mutex>
#include <memory>
#include <queue>

/*
	考虑第3章的线程安全栈，使用一个互斥锁住整个数据结构
	保证了只有唯一的线程访问数据

	pop函数没有像std的接口那样，单纯地弹出。从而不需要前置判空，增加多线程访问到中间不安全的状态
	if (!st.empty()) 
	{
		// empty之后，pop之前，其他线程可能读写，导致接下来的pop失败
		st.pop();
	}

	只要在empty用互斥锁住，就能保护不安全状态
	线程安全栈的pop提供了两个版本，一个返回值返回元素，一个出参返回元素
	它们把empty判空包含进互斥保护内，避免了上述的隐患

	一个互斥保护整个数据结构的缺点在于，对数据的访问退化为了串行
	对此，线程安全队列使用条件变量来优化

	所有要出队的线程都等待条件变量以及队列非空的判断
	而push一个元素后，条件变量会唤醒一个线程
	出参返回可以直接移动元素；而对于返回值返回，避免内存管理，可返回智能指针

	智能指针构造时可能异常，此时抛出，由于还未pop，数据依然安全，但却浪费了 notify_one 的机会
	因为每次push只唤醒一个线程，所以如果这次唤醒由于异常而失败，则只能等待下次唤醒，也就是等待下次push
	于是队列里依然有元素，但却可能还存在被阻塞的线程
	除非有一个线程要pop，那么它会首次判空成功而正常 pop
	解决办法可以是把 notify_one 改为 notify_all， 但显然等待的线程里只有一个能继续执行，其他线程在竞争失败后依然阻塞
	又或者让异常处理再次notify_one
	不过更为优秀的改进是，让队列直接存储只能指针，避免在pop处构建智能指针，而是复制。智能指针的复制是不会异常的
*/

template<typename T>
class threadsafe_queue
{
	mutable std::mutex mtx;
	std:queue<std::shared_ptr<T>> data_queue;
	std::condition_variable data_cond;

public:
	threadsafe_queue() = default;

	bool try_pop(T& value)
	{

		std::unique_lock lk{ mtx };
		if (data_queue.empty())return false;
		value = std::move(*data_queue.front());
		data_queue.pop();
		return true;
	}

	std::shared_ptr<T> try_pop()
	{

		std::unique_lock lk{ mtx };
		if (data_queue.empty())return {};
		auto res = data_queue.front();
		data_queue.pop();
		return res;
	}
	
	void wait_and_pop(T& value)
	{
		std::unique_lock lk{ mtx };
		data_cond.wait(lk, [this] {return !data_queue.empty();});
		value = std::move(*data_queue.front());
		data_queue.pop();
	}

	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_lock lk{ mtx };
		data_cond.wait(lk, [this] {return !data_queue.empty();});
		auto res = data_queue.front();// 复制智能指针，不会异常
		data_queue.pop();
		return res;
	}

	void push(T value)
	{
		auto data = std::make_shared<T>(std::move(value));// 在push时构建智能指针，如果异常，不会加锁和修改队列
		std::lock_guard lk{ mtx };
		data_queue.push(data);
		data_cond.notify_one();
	}

	bool empty()const
	{
		std::lock_guard lk{ mtx };
		return data_queue.empty();
	}
};

/*
	精细粒度的锁实现线程安全队列
	队列只有出队和入队的操作，所以更好的做法是只锁住队列的头尾节点
	如此，push时，其他线程仍然可以pop，提高并发性能

	而单独对头尾加锁，以单向前闭后闭链表作为容器更为直观

	对于push，它往尾节点插入元素，由于链表可能为空，所以第一个插入的节点还要被赋值为head
	于是push函数既访问尾节点，也可能访问头节点，于是push可能会要锁住两个互斥

	因此更好的做法是多一个空白节点。初始时，head和tail都指向空白节点
	随着数据的插入，头节点依然指向第一个元素，而tail则始终指向空白节点，空白节点不断后移
	于是即便链表为空，head也至少指向空白节点，于是不会在push时修改head（但是会修改head的data和next）

	tail加锁的情况只有读和写，其中写只有push，它在修改的全程都要互斥保护
	读则只需要短暂加锁，以提高锁的精度，因此可以单独封装一个get_tail函数

	pop设计到对head的修改，所以需要加锁；然后弹出元素的逻辑不用保护
	如果链表为空，则弹出空指针。判断链表为空使用 head == tail。由于访问tail，所以get_tail更为合适
*/

template<typename T>
class threadsafe_queue2
{
	struct node
	{
		std::shared_ptr<T> data;
		std::unique_ptr<node> next;
	};

	std::mutex head_mtx;
	std::mutex tail_mtx;
	std::unique_ptr<node> head;
	node* tail;
	std::condition_variable data_cond;

	// 除了push，其他地方使用此函数获取tail，以便和push修改tail的逻辑互斥，保护不安全状态
	node* get_tail()
	{
		std::lock_guard  lk{ tail_mtx };
		return tail;
	}

	std::unique_ptr<node> pop_head_with_lock()
	{
		// auto tail_ = get_tail();
		std::lock_guard lk{ head_mtx };
		if (head.get() == get_tail()) // get_tail必须也在head_mtx的保护之内，防止head与tail比较时，拿到的tail是旧的tail
		{
			return nullptr;
		}

		auto old_head = std::move(head);
		head = std::move(old_head->next);
		return old_head;
	}

	std::unique_ptr<node> pop_head()// 封装的弹出head函数，调用前必须锁住head_mtx
	{
		auto old_head = std::move(head);
		head = std::move(old_head->next);
		return old_head;
	}

	std::unique_lock<std::mutex> wait_for_data()// 封装的等待数据函数，它会把锁移动返回给调用者
	{
		std::unique_lock lk{ head_mtx };
		data_cond.wait(lk, [this] {return head.get() != get_tail();});
		return std::move(lk);
	}

	std::unique_ptr<node> wait_pop_head()
	{
		auto lk{ wait_for_data() };
		return pop_head();
	}

	std::unique_ptr<node> wait_pop_head(T& value)
	{
		auto lk{ wait_for_data() };
		value = std::move(*head->data);
		return pop_head();
	}

	// 此函数 和pop_head_with_lock 一样
	std::unique_ptr<node> try_pop_head()
	{
		std::lock_guard lk{ head_mtx };
		if (head == get_tail())
		{
			return nullptr;
		}

		return pop_head();
	}

	std::unique_ptr<node> try_pop_head(T& value)
	{
		std::lock_guard lk{ head_mtx };
		if (head == get_tail())
		{
			return nullptr;
		}

		value = std::move(*head->data);
		return pop_head();
	}

public:
	threadsafe_queue2()
		: head{ std::make_unique<node>() }
		, tail{ head.get() }
	{
	}

	threadsafe_queue2(const threadsafe_queue2&) = delete;
	threadsafe_queue2& opeartor=(const threadsafe_queue2&) = delete;

	bool empty()const
	{
		std::lock_guard lk{ head_mtx };
		return head == get_tail();
	}

	std::shared_ptr<T> try_pop()
	{
		auto old_head = pop_head();
		return old_head ? old_head->data : {};
	}

	bool try_pop(T& value)
	{
		const auto old_head = try_pop_head(value);
		return !!old_head;
	}

	std::shared_ptr<T> wait_and_pop()
	{
		const auto old_head = wait_pop_head();
		return head->data;
	}

	void wait_and_pop(T& value)
	{
		const auto old_head = wait_pop_head(value); 
	}

	void push(T value)
	{
		auto new_data = std::make_shared<T>(std::move(value));
		auto p = std::make_unique<node>();

		{
			// 创建好data指针和新的空白节点后，就要开始修改tail，局部互斥保护
			std::lock_guard lk{ tail_mtx };
			tail->data = new_data;
			tail->next = std::move(p);
			tail = tail->next.get();
		}

		data_cond.notify_one();
	}
};


/*
	线程安全查找表，以hash表为底层数据结构

	采用拉链法，一个bucket数组，每个元素是一个链表的头节点
	每个节点都有独立的shared_mutex，使并发访问，串行修改
*/

template<typename Key, typename Value, typename Hash = std::hash<Key>>
class threadsafe_lookup_table
{
	class bucket_type
	{
		using bucket_value = std::pair<Key, Value>;
		using bucket_data = std::list<bucket_value>;
		using bucket_iterator = typename bucket_data::iterator;

		bucket_data data;
		mutable std::shared_mutex mtx;

		bucket_iterator find_entry_for(const Key& key)const
		{
			return std::find_if(data.begin(), data.end(), [&](const bucket_value& item) {return item.first == key;});
		}

	public:
		Value value_for(const Key& key, const Value& default_value)const
		{
			std::shared_lock lk{ mtx }; // 加共享锁
			const auto found_entry = find_entry_for(key);
			return data.end() == found_entry ? default_value : found_entry->second;
		}

		void add_or_update_mapping(const Key& key, const Value& value)
		{
			std::unique_lock lk{ mtx };// 加排他锁
			const auto found_entry = find_entry_for(key);
			if (data.end() == found_entry)
			{
				data.emplace_back(key, value);
			}
			else
			{
				found_entry->second = value;
			}
		}

		void remove_mapping(const Key& key)
		{
			std::unique_lock lk{ mtx };
			const auto found_entry = find_entry_for(key);
			if (data.end() != found_entry)
			{
				data.erase(found_entry);
			}
		}
	};

	std::vector<std::unique_ptr<bucket_type>> buckets;
	Hash hasher;
	bucket_type& get_bucket(const Key& key)const
	{
		const auto bucket_index = hasher(key) % buckets.size();
		return *buckets[bucket_index];
	}

public:
	using key_type = Key;
	using mapped_type = Value;
	using hash_type = Hash;

	threadsafe_lookup_table(unsigned num_buckets = 19, const Hash& hasher_ = Hash())
		: buckets(num_buckets)
		, hasher{ hasher_ }
	{
		for (unsigned i = 0;i < num_buckets;++i)
		{
			buckets[i].reset(new bucket_type);
		}
	}

	threadsafe_lookup_table(const threadsafe_lookup_table&) = delete;
	threadsafe_lookup_table& operator(const threadsafe_lookup_table&) = delete;

	Value value_for(const Key& key, const Value& default_value = Value{})const
	{
		return get_bucket(key).value_for(key, default_value);
	}

	void add_or_update_mapping(const Key& key, const Value& value)
	{
		get_bucket(key).add_or_update_mapping(key, value);
	}

	void remove_mapping(const Key& key)
	{
		get_bucket(key).remove_mapping(key);
	}

	std::map<Key, Value> get_map()const
	{
		std::vector<std::unique_lock<std::shared_mutex>> locks;
		for (unsigned i = 0;i < buckets.size();++i)
		{
			locks.push_back(std::unique_lock<std::shared_mutex>{ buckets[i].mutex });
		}

		std::map<Key, Value> res;

		for (unsigned i = 0;i < buckets.size();++i)
		{
			for (auto it = buckets[i].data.begin(); it != buckets[i].data.end();++it)
			{
				res.insert(*it);
			}
		}

		return res;

	}
};

/*
	粒度更精细的线程安全的链表
	每个节点都独立拥有锁，遍历时，最多只对前后两个节点加锁
	比起单一互斥锁住整个链表
	只锁住两个节点，可以允许多个线程同时遍历链表，并且同步允许
	假如一个线程在遍历时锁住了i和 i+1节点
	那么在访问它前面节点的线程就不会超过这个线程
	在访问它后面节点 的线程则不受影响
	于是所有线程会以出发时的顺序遍历完链表
	链表将最大允许 size/2的线程同时访问
*/

template<typename T>
class threadsafe_list
{
	struct node
	{
		std::mutex m;
		std::shared_ptr<T> data;
		std::unique_ptr<node>next;
		node() = default;
		node(const T& value)
			: data{ std::make_shared<T>(value) }
		{
		}
	};

	node head;

public:
	threadsafe_list() = default;
	threadsafe_list(const threadsafe_list&) = delete;
	threadsafe_list& operator(const threadsafe_list&) = delete;
	~threadsafe_list()
	{
		remove_if([] {return  true});
	}

	void push_front(const T& value)
	{
		auto new_node = std::make_unique<T>(value);
		std::lock_guard lk{ head.m };
		new_node->next = std::move(head.next);
		head.next = std::move(new_node);
	}

	template<typename Func>
	void for_each(Func f)
	{
		node* current = &head;
		std::unique_lock lk{ head.m };
		while (const node* next = current->next.get())
		{
			std::unique_lock next_lk{ next->m };
			lk.unlock();// 解开当前节点的锁
			f(*next->data);
			current = next;
			lk = std::move(next_lk);// 切换到下一个节点的锁
		}
	}

	template<typename Predicate>
	std::shared_ptr<T> find_first_if(Predicate p)
	{
		node* current = &head;
		std::unique_lock lk{ head.m };

		while (const node* next = current->next.get())
		{
			// 先加next锁，再解current锁，使保护范围之间不会有不安全状态
			std::unique_lock next_lk{ next->m };
			lk.unlock();
			if (p(*next->data))
			{
				return next->data;
			}
			current = next;
			lk = std::move(next_lk);
		}

		return nullptr;
	}

	template<typename Predicate>
	std::shared_ptr<T> remove_if(Predicate p)
	{
		node* current = &head;
		std::unique_lock lk{ head.m };

		while (const node* next = current->next.get())
		{
			std::unique_lock next_lk{ next->m };
			if (p(*next->data))
			{
				// 如果要移除next，不用解开当前锁，修改current->next后解开next锁即可
				// 因为当前节点依然要被继续锁住
				auto old_next = std::move(current->next);
				current->next = std::move(next->next);
				next_lk.unlock();
			}
			else
			{
				lk.unlock();// 解开当前节点的锁
				current = next;
				lk = std::move(next_lk);
			}
		}

		return nullptr;
	}
};