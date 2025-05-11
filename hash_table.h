#pragma once
#include <vector>
using std::vector;

template <class Value>
struct __hashtable_node
{
	__hashtable_node* next;
	Value val;
};

template<class Value, class Key, class HashFcn,class ExtractKey,class EqualKey,class Alloc>
struct __hashtable_iterator
{
	typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> hashtable;
	typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
	typedef __hashtable_node<Value> node;

	typedef forward_iterator_tag iterator_category;//哈希表的迭代器为forward类型
	typedef Value value_type;
	typedef ptrdiff_t size_type;
	typedef size_t size_type;
	typedef Value& reference;
	typedef Value* pointer;

	node* cur;
	hashtable* ht;

	__hashtable_iterator(node* n, hashtable* tab) :cur(n), ht(tab) {}
	__hashtable_iterator() {}

	reference operator*()const { return cur->val; }
	pointer operator->()const { return &(operator*()); }
	bool operator==(const iterator& it)const { return cur == it.cur; }
	bool operator!=(const iterator& it)const { return cur != it.cur; }
	iterator& operator++()
	{
		const node* old = cur;
		cur = cur->next;
		if (!cur)
		{
			size_type bucket = ht->bkt_num(old->val);
			while (!cur && ++bucket < ht->buckets.size())
				cur = ht->buckets[bucket];
		}
		return *this;
	}
	iterator operator++(int)
	{
		iterator tmp = *this;
		++* this;
		return tmp;
	}


};

static const int __stl_num_prime = 12;//28
static const unsigned long __stl_prime_list[__stl_num_prime] =
{
	53, 97,193,389,769,1543,3079,6151,12289,24593,49157,98317
};

inline unsigned long __stl_next_prime(unsigned long n)
{
	const unsigned long* first = __stl_prime_list;
	const unsigned long* last = __stl_prime_list + __stl_num_prime;
	const unsigned long* pos = lower_bound(first, last, n);
	return pos == last ? *(last - 1) : *pos;
}

/*
	HashFcn：将key数值化，以便散列到桶里
	ExtractKey：从value里提取key，可以和hash
	EqualKey：比较两个key是否相等

	extractkey是根据存储元素的结构特化的，例如，hashtable后续被封装称unoradered_map/set
		unoradered_set存放的就是value,extractkey就是identity，即直接取出value
		unoradered_map存放的是pair<const Key,Value>，extractkey就是取出pair的first
	而如果希望key的比较和散列有自定义行为，操作的是hash和equal。

	成员bkt_num调用getkey从value获取key，再调用hash把key数值化，然后再对n取模
	insert_unique调用getkey从节点的val上获取key，再通过equalkey来比较两个value的key是否相等

	也就是说，不论hash和equal，都会调用到getkey。因此getkey可以自定义从而进行复杂的操作


*/
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc = alloc>
class hashtable
{
	typedef size_t size_type;
	typedef Key key_type;
	typedef Value value_type;
	typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
private:
	typedef __hashtable_node<Value> node;
	
	HashFcn hash;
	EqualKey equals;
	ExtractKey get_key;

	vector<node*> buckets;
	size_type num_elements;

	size_type max_bucket_count()const
	{
		return __stl_prime_list[__stl_num_prime - 1];
	}

	node* new_node(const value_type& obj)
	{
		node* n = alloc::allocate(node);
		construct(&n->val, obj);//obj去构造node的val。最好不要直接复制
		n->next = 0;
	}
	void delete_node(node* n)
	{
		destroy(&n->val);//析构val
		alloc::deallocate(n);
	}

	void initialize_buckets(size_type n)
	{
		const size_type n_buckets = __stl_next_prime(n);
		buckets.reserve(n_buckets);
		buckets.insert(buckets.end(), n_buckets, (node*)0);//把桶组填满空指针。桶组里放的并不是元素，而是指向元素的指针。因此要先填满
		num_elements = 0;
	}

	size_type bkt_num(const value_type& obj, size_t n)const
	{
		return bkt_num_key(get_key(obj), n);
	}
	size_type bkt_num(const value_type& obj)const
	{
		return bkt_num_key(get_key(obj), buckets.size());
	}
	size_type bkt_num_key(const key_type& key, size_t n)
	{
		return hash(key) % n;
	}

	pair<iterator, bool> insert_unique(const value_type& obj)
	{
		resize(num_elements + 1);
		return insert_unique_noresize(obj);
	}
	iterator insert_equal(const value_type& obj)
	{
		resize(num_elements + 1);
		return insert_equal_noresize(obj);
	}

	void resize(size_type num_elements_hint)
	{
		const size_type old_n = buckets.size();
		if (num_elements_hint > old_n)
		{
			const size_type n = __stl_next_prime(num_elements_hint);
			if (n > old_n)
			{
				vector<node*>tmp(n, (node*)0);
				for (size_type bucket = 0; bucket < old_n; ++bucket)
				{
					node* first = buckets[bucket];
					while (first)
					{
						//头删法从旧容器取出节点，头插法插入到新容器中
						size_type new_bucket = bkt_num(first->val, n);
						buckets[bucket] = first->next;
						first->next = tmp[new_bucket];
						tmp[new_bucket] = first;
						first = buckets[buckets];
					}
				}
				buckets.swap(tmp);
			}
		}
	}

	pair<iterator, bool> insert_unique_noresize(const value_type& obj)
	{
		const size_type n = bkt_num(obj);
		node* first = buckets[n];

		for (node* cur = first; cur; cur = cur->next)
		{
			if (equals(get_key(cur->val), get_key(obj)))
				return pair<iterator, bool>(iterator(cur, this), false);
		}

		node* tmp = new_node(obj);
		tmp->next = first;
		buckets[n] = tmp;
		++num_elements;
		return pair<iterator, bool>(iterator(tmp, this), true);
	}
	iterator insert_equal_noresize(const value_type& obj)
	{
		const size_type n = bkt_num(obj);
		node* first = buckets[n];

		node* tmp = new_node(obj);
		tmp->next = first;
		buckets[n] = tmp;
		++num_elements;
		return iterator(tmp, this);
	}
};


/*
	hash_set
		容器里实际存放是value，提取key就是直接取出value
*/
template <class Value, class HashFcn = hash<Value>, class EqualKey =  equal_to<Value>, class Alloc = alloc>
class hash_set
{
private:
	typedef hashtable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc> ht;
};

/*
	hash_map
		容器里实际存放的是pair<const Key, T>，提取key就是取出pair的first
		基于红黑树的map实际也是存放pair
*/
template<class Key,class T, class HashFcn=hash<Key>, class EqualKey = equal_to<Key>, class Alloc = alloc>
class hash_map
{
	typedef hashtable<pair<const Key, T>, Key, HashFcn, select1st< pair<const Key, T>> EqualKey, Alloc> ht;
};
/*
	对于一个string做key、忽略大小写的unoradered_map，两个只有大小写不同的string做key，会视为重复，因此插入失败
	回顾一下插入操作：
		getkey->hash得到新元素所在桶，然后逐个getkey桶内的元素，用equal去比较新元素的getkey

	可以看到hash和equal不是最底层的操作。最底层的是getkey。不论是新元素，还是已有元素，都需要getkey
	因此提供一个忽视大小写的getkey即可实现护士大小写的hashtable

*/