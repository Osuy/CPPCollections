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

	typedef forward_iterator_tag iterator_category;//��ϣ��ĵ�����Ϊforward����
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
	HashFcn����key��ֵ�����Ա�ɢ�е�Ͱ��
	ExtractKey����value����ȡkey�����Ժ�hash
	EqualKey���Ƚ�����key�Ƿ����

	extractkey�Ǹ��ݴ洢Ԫ�صĽṹ�ػ��ģ����磬hashtable��������װ��unoradered_map/set
		unoradered_set��ŵľ���value,extractkey����identity����ֱ��ȡ��value
		unoradered_map��ŵ���pair<const Key,Value>��extractkey����ȡ��pair��first
	�����ϣ��key�ıȽϺ�ɢ�����Զ�����Ϊ����������hash��equal��

	��Աbkt_num����getkey��value��ȡkey���ٵ���hash��key��ֵ����Ȼ���ٶ�nȡģ
	insert_unique����getkey�ӽڵ��val�ϻ�ȡkey����ͨ��equalkey���Ƚ�����value��key�Ƿ����

	Ҳ����˵������hash��equal��������õ�getkey�����getkey�����Զ���Ӷ����и��ӵĲ���


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
		construct(&n->val, obj);//objȥ����node��val����ò�Ҫֱ�Ӹ���
		n->next = 0;
	}
	void delete_node(node* n)
	{
		destroy(&n->val);//����val
		alloc::deallocate(n);
	}

	void initialize_buckets(size_type n)
	{
		const size_type n_buckets = __stl_next_prime(n);
		buckets.reserve(n_buckets);
		buckets.insert(buckets.end(), n_buckets, (node*)0);//��Ͱ��������ָ�롣Ͱ����ŵĲ�����Ԫ�أ�����ָ��Ԫ�ص�ָ�롣���Ҫ������
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
						//ͷɾ���Ӿ�����ȡ���ڵ㣬ͷ�巨���뵽��������
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
		������ʵ�ʴ����value����ȡkey����ֱ��ȡ��value
*/
template <class Value, class HashFcn = hash<Value>, class EqualKey =  equal_to<Value>, class Alloc = alloc>
class hash_set
{
private:
	typedef hashtable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc> ht;
};

/*
	hash_map
		������ʵ�ʴ�ŵ���pair<const Key, T>����ȡkey����ȡ��pair��first
		���ں������mapʵ��Ҳ�Ǵ��pair
*/
template<class Key,class T, class HashFcn=hash<Key>, class EqualKey = equal_to<Key>, class Alloc = alloc>
class hash_map
{
	typedef hashtable<pair<const Key, T>, Key, HashFcn, select1st< pair<const Key, T>> EqualKey, Alloc> ht;
};
/*
	����һ��string��key�����Դ�Сд��unoradered_map������ֻ�д�Сд��ͬ��string��key������Ϊ�ظ�����˲���ʧ��
	�ع�һ�²��������
		getkey->hash�õ���Ԫ������Ͱ��Ȼ�����getkeyͰ�ڵ�Ԫ�أ���equalȥ�Ƚ���Ԫ�ص�getkey

	���Կ���hash��equal������ײ�Ĳ�������ײ����getkey����������Ԫ�أ���������Ԫ�أ�����Ҫgetkey
	����ṩһ�����Ӵ�Сд��getkey����ʵ�ֻ�ʿ��Сд��hashtable

*/