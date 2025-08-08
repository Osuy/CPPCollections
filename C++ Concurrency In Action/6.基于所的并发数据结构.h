#pragma once
#include <mutex>
#include <memory>
#include <queue>

/*
	���ǵ�3�µ��̰߳�ȫջ��ʹ��һ��������ס�������ݽṹ
	��֤��ֻ��Ψһ���̷߳�������

	pop����û����std�Ľӿ������������ص������Ӷ�����Ҫǰ���пգ����Ӷ��̷߳��ʵ��м䲻��ȫ��״̬
	if (!st.empty()) 
	{
		// empty֮��pop֮ǰ�������߳̿��ܶ�д�����½�������popʧ��
		st.pop();
	}

	ֻҪ��empty�û�����ס�����ܱ�������ȫ״̬
	�̰߳�ȫջ��pop�ṩ�������汾��һ������ֵ����Ԫ�أ�һ�����η���Ԫ��
	���ǰ�empty�пհ��������Ᵽ���ڣ�����������������

	һ�����Ᵽ���������ݽṹ��ȱ�����ڣ������ݵķ����˻�Ϊ�˴���
	�Դˣ��̰߳�ȫ����ʹ�������������Ż�

	����Ҫ���ӵ��̶߳��ȴ����������Լ����зǿյ��ж�
	��pushһ��Ԫ�غ����������ỽ��һ���߳�
	���η��ؿ���ֱ���ƶ�Ԫ�أ������ڷ���ֵ���أ������ڴ�����ɷ�������ָ��

	����ָ�빹��ʱ�����쳣����ʱ�׳������ڻ�δpop��������Ȼ��ȫ����ȴ�˷��� notify_one �Ļ���
	��Ϊÿ��pushֻ����һ���̣߳����������λ��������쳣��ʧ�ܣ���ֻ�ܵȴ��´λ��ѣ�Ҳ���ǵȴ��´�push
	���Ƕ�������Ȼ��Ԫ�أ���ȴ���ܻ����ڱ��������߳�
	������һ���߳�Ҫpop����ô�����״��пճɹ������� pop
	����취�����ǰ� notify_one ��Ϊ notify_all�� ����Ȼ�ȴ����߳���ֻ��һ���ܼ���ִ�У������߳��ھ���ʧ�ܺ���Ȼ����
	�ֻ������쳣�����ٴ�notify_one
	������Ϊ����ĸĽ��ǣ��ö���ֱ�Ӵ洢ֻ��ָ�룬������pop����������ָ�룬���Ǹ��ơ�����ָ��ĸ����ǲ����쳣��
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
		auto res = data_queue.front();// ��������ָ�룬�����쳣
		data_queue.pop();
		return res;
	}

	void push(T value)
	{
		auto data = std::make_shared<T>(std::move(value));// ��pushʱ��������ָ�룬����쳣������������޸Ķ���
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
	��ϸ���ȵ���ʵ���̰߳�ȫ����
	����ֻ�г��Ӻ���ӵĲ��������Ը��õ�������ֻ��ס���е�ͷβ�ڵ�
	��ˣ�pushʱ�������߳���Ȼ����pop����߲�������

	��������ͷβ�������Ե���ǰ�պ��������Ϊ������Ϊֱ��

	����push������β�ڵ����Ԫ�أ������������Ϊ�գ����Ե�һ������Ľڵ㻹Ҫ����ֵΪhead
	����push�����ȷ���β�ڵ㣬Ҳ���ܷ���ͷ�ڵ㣬����push���ܻ�Ҫ��ס��������

	��˸��õ������Ƕ�һ���հ׽ڵ㡣��ʼʱ��head��tail��ָ��հ׽ڵ�
	�������ݵĲ��룬ͷ�ڵ���Ȼָ���һ��Ԫ�أ���tail��ʼ��ָ��հ׽ڵ㣬�հ׽ڵ㲻�Ϻ���
	���Ǽ�������Ϊ�գ�headҲ����ָ��հ׽ڵ㣬���ǲ�����pushʱ�޸�head�����ǻ��޸�head��data��next��

	tail���������ֻ�ж���д������дֻ��push�������޸ĵ�ȫ�̶�Ҫ���Ᵽ��
	����ֻ��Ҫ���ݼ�������������ľ��ȣ���˿��Ե�����װһ��get_tail����

	pop��Ƶ���head���޸ģ�������Ҫ������Ȼ�󵯳�Ԫ�ص��߼����ñ���
	�������Ϊ�գ��򵯳���ָ�롣�ж�����Ϊ��ʹ�� head == tail�����ڷ���tail������get_tail��Ϊ����
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

	// ����push�������ط�ʹ�ô˺�����ȡtail���Ա��push�޸�tail���߼����⣬��������ȫ״̬
	node* get_tail()
	{
		std::lock_guard  lk{ tail_mtx };
		return tail;
	}

	std::unique_ptr<node> pop_head_with_lock()
	{
		// auto tail_ = get_tail();
		std::lock_guard lk{ head_mtx };
		if (head.get() == get_tail()) // get_tail����Ҳ��head_mtx�ı���֮�ڣ���ֹhead��tail�Ƚ�ʱ���õ���tail�Ǿɵ�tail
		{
			return nullptr;
		}

		auto old_head = std::move(head);
		head = std::move(old_head->next);
		return old_head;
	}

	std::unique_ptr<node> pop_head()// ��װ�ĵ���head����������ǰ������סhead_mtx
	{
		auto old_head = std::move(head);
		head = std::move(old_head->next);
		return old_head;
	}

	std::unique_lock<std::mutex> wait_for_data()// ��װ�ĵȴ����ݺ�������������ƶ����ظ�������
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

	// �˺��� ��pop_head_with_lock һ��
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
			// ������dataָ����µĿհ׽ڵ�󣬾�Ҫ��ʼ�޸�tail���ֲ����Ᵽ��
			std::lock_guard lk{ tail_mtx };
			tail->data = new_data;
			tail->next = std::move(p);
			tail = tail->next.get();
		}

		data_cond.notify_one();
	}
};


/*
	�̰߳�ȫ���ұ���hash��Ϊ�ײ����ݽṹ

	������������һ��bucket���飬ÿ��Ԫ����һ�������ͷ�ڵ�
	ÿ���ڵ㶼�ж�����shared_mutex��ʹ�������ʣ������޸�
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
			std::shared_lock lk{ mtx }; // �ӹ�����
			const auto found_entry = find_entry_for(key);
			return data.end() == found_entry ? default_value : found_entry->second;
		}

		void add_or_update_mapping(const Key& key, const Value& value)
		{
			std::unique_lock lk{ mtx };// ��������
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
	���ȸ���ϸ���̰߳�ȫ������
	ÿ���ڵ㶼����ӵ����������ʱ�����ֻ��ǰ�������ڵ����
	����һ������ס��������
	ֻ��ס�����ڵ㣬�����������߳�ͬʱ������������ͬ������
	����һ���߳��ڱ���ʱ��ס��i�� i+1�ڵ�
	��ô�ڷ�����ǰ��ڵ���߳̾Ͳ��ᳬ������߳�
	�ڷ���������ڵ� ���߳�����Ӱ��
	���������̻߳��Գ���ʱ��˳�����������
	����������� size/2���߳�ͬʱ����
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
			lk.unlock();// �⿪��ǰ�ڵ����
			f(*next->data);
			current = next;
			lk = std::move(next_lk);// �л�����һ���ڵ����
		}
	}

	template<typename Predicate>
	std::shared_ptr<T> find_first_if(Predicate p)
	{
		node* current = &head;
		std::unique_lock lk{ head.m };

		while (const node* next = current->next.get())
		{
			// �ȼ�next�����ٽ�current����ʹ������Χ֮�䲻���в���ȫ״̬
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
				// ���Ҫ�Ƴ�next�����ý⿪��ǰ�����޸�current->next��⿪next������
				// ��Ϊ��ǰ�ڵ���ȻҪ��������ס
				auto old_next = std::move(current->next);
				current->next = std::move(next->next);
				next_lk.unlock();
			}
			else
			{
				lk.unlock();// �⿪��ǰ�ڵ����
				current = next;
				lk = std::move(next_lk);
			}
		}

		return nullptr;
	}
};