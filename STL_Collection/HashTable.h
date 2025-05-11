#pragma once

/*
	STL��HashTable��hashmap��hashset�ĵײ�������
	hashtable��Ѱַ����Ϊ���������п�vector��ÿ�����ά��һ����������STL��list����Ϊ����Ҫ��ô�๦�ܣ�
	ע�⣺
		hashtable��Ȼ֧�ּ�ֵ��ʵֵ��Ԫ����ʽ����������ֻ�洢��ʵֵ����ֵ�Ǹ���ExtractKey������ʵֵ����ó��ġ�
		��ˣ����������ͬ��ʵֵ����ExtractKey�����õ��ļ�ֵ����ͬ�ģ����ܲ���������Ԫ�ء�
		hashSet�ļ�ֵ����ʵֵ�����ֻ�洢ʵֵ��ExtractKey��������ֱ�ӷ���ʵֵ
		hashMap�ļ�ֵ��Key,ʵֵ��pair<const Key,Value>����˼�ֵ��ʵֵ���洢��ExtractKey��������select1st<pair<...>>��
*/

//hash_table�Ľڵ㣬�����˽ڵ��ֵ��һ��ָ����һ���ڵ��ָ��
template <class Value>
struct __hashtable_node
{
	__hashtable_node* next;
	Value val;
};

//hash_table�ĵ�������ע�⣺ֻ֧��++����֧��--
template<class Value,class Key,class HashFcn,class ExtractKey,class EqualKey,class Alloc>
struct __hashtable_iterator
{
	typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>                     hashtable;
	typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>          iterator;
	typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>    const_iterator;
	typedef __hashtable_node<Value>                                                         node;

	typedef forward_iteratro_tag iterator_category;
	typedef Value value_type;
	typedef size_t size_type;
	typedef Value& reference;
	typedef Value* pointer;

	node* cur;
	hashtable* ht;

	__hashtable_iterator(node* n, hashtable* tab) :cur(n), ht(tab) {}
	__hashtable_iterator() {}


	reference operator*()const { return cur->val; }
	pointer   operator->()const { return &(operator*()); }
	iterator& operator++()
	{
		const node* old = cur;
		cur = cur->next;
		if (!cur)
		{
			size_type bucket = ht->bkt_num(old->val);//btk_num��hashtable�ĺ���������val�õ�����Ͱ������
			while (!cur && ++bucket < ht->buckets.size())//�����һ��Ͱ�ǿյģ���ȥ���¸�Ͱ��ֱ������vector��β��
			{
				cur = ht->buckets[bucket];
			}
		}
		return *this;
	}
	iterator  operator++(int)
	{
		iterator tmp = *this;
		++* this;
		return tmp;
	}
	bool      operator==(const iterator& it)const { return cur == it.cur; }
	bool      operator!=(const iterator& it)const { return cur != it.cur; }

};


//hashtable�����ݽṹ
template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc = alloc>
class hashtable
{
public:
	typedef HashFcn hasher;
	typedef EqualKey key_equal;
	typedef size_t size_type;
	typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;

	typedef __hashtable_node<Value>   node;
	typedef simple_alloc<node, Alloc> node_allocator;

private:
	hasher hash;
	key_equal equals;
	ExtractKey get_key;

	vector<node*, Alloc> buckets;
	size_type num_elements;


public:
	size_type size() const { return num_elements; }

	size_type bucket_count()const { return buckets.size(); }


	node* new_node(const value_type& obj)
	{
		node* n = node_allocator::allocate();
		n->next = 0;
		__STL_TRY
		{
			constructor(&n->val,obj);
			return n;
		}
		__STL_UNWIND(node_allocator::deallocate(n));
	}

	void delete_node(node* n)
	{
		destroy(&n->val);
		node_allocator::deallocate(n);
	}

	hashtable(size_type n, const HashFcn& hf, const EqualKey eql) :hash(hf), equals(eql), get_key(ExtractKey()), num_elementss(0)
	{
		initialize_buckets(n);
	}

	void initialize_buckets(size_type n)
	{
		const size_type n_buckets = next_size(n);//��ȡ�����ڵ���n�ĵ�һ��������������STL�ڶ���28���������飩
		buckets.reserve(n_buckets);//ʹ��vector��reserve��Ԥ�������Ǹ������Ŀռ䣬��vector����βԪ��û�䣬�˴�vectorΪ�ա�
		buckets.insert(buckets.end(), n_buckets, (node*)0);//��vector��β������0���˴�����vector��Ԫ��ȫ����Ϊ0
		num_elements = 0;
	}

	//�����趨hashtable��Ͱ�Ĵ�С
	void resize(size_type num_elements_hint)
	{
		const size_type old_n = buckets.size();
		if (num_elements_hint > old_n)
		{
			const size_type n = next_size(num_elements_hint);//��ȡ����һ������
			if (n > old_n)
			{
				vector<node*, A> tmp(n, (node*)0);//����һ���µ�vector������Ϊn
				__STL_TRY
				{
					for (size_type bucket = 0; bucket < old_n; ++bucket)//����ÿ��Ͱ
					{
						node* first = buckets[bucket];//ȡ��Ͱ���׽ڵ㣬
						while (first)//��firstָ�벻Ϊ�գ���Ͱ��δ������
						{
							size_type new_bucket = btk_num(first->val, n);//����first��val����Ӧ�ô�����hashtable���ĸ�Ͱ��
							buckets[bucket] = first->next;//buckets[bucket]ԭ��ָ��first����Ϊָ��first->next���൱�ڽ�first�ó�
							first->next = tmp[new_bucket];//first��nextָ��tmp�Ķ�ӦͰ����һ�У���Ͱ���׽ڵ�ָ��first���൱�ڽ�first���뵽Ͱ�г�Ϊ��һ���ڵ�
							tmp[new_bucket] = first;
							first = buckets[bucket];//���¶�λfirst���Ա���һ��ѭ����
						}
					}
				buckets.swap(tmp);//������Ͱ�������˺���������tmp�Զ��ͷ�
				}
			}
		}
	}

	//����insert_unique��insert_equal�ֱ�ʵ����hash_XXX��hash_multiXXX
	//����Ԫ�أ��������ֵ��ͬ,����pair<������,�Ƿ����ɹ�>
	pair<iterator, bool> insert_unique(const value_type& obj)
	{
		resize(num_elements + 1);//�˴�Ϊhashtable�ĳ�Ա����
		return insert_unique_noresize(obj);
	}
	pair<iterator, bool> insert_unique_noresize(const value_type& obj)
	{
		const size_type n = btk_num(obj);//����objӦ�����ĸ�Ͱ
		node* first = buckets[n];//��ȡ��Ͱ���׽ڵ�
		for (node* cur = first; cur; cur = cur->next)//������Ͱ������ҵ�ĳ��Ԫ�صļ�ֵ��obj��ȣ�����false
		{
			if(equals(get_key(cur->val),get_key(obj))
				return pair<iterator,bool> (iterator(cur,this),false);
		}

		//�˳�ѭ����˵�����Բ���obj����˴����½ڵ�
		node* tmp = new_node(obj);
		//�����н�tmp���뵽Ͱ��Ϊ�׽ڵ�
		tmp->next = first; /
		buckets[n] = tmp;
		//����Ԫ�ظ���
		++num_elements;
		return pair<iterator, bool>__hashtable_iterator((tmp, this), true);
	}

	//����Ԫ��,�����ֵ�ظ������ص�����������insert_unique ��ͬ���Ƿ���ֵΪ��������
	iterator insert_equal(const value_type& obj)
	{
		resize(num_elements + 1);//�˴�Ϊhashtable�ĳ�Ա����
		return insert_equal_noresize(obj);
	}
	iterator insert_equal_noresize(const value_type& obj)
	{
		const size_type n = btk_num(obj);//����objӦ�����ĸ�Ͱ
		node* first = buckets[n];//��ȡ��Ͱ���׽ڵ�
		for (node* cur = first; cur; cur = cur->next)//������Ͱ������ҵ�ĳ��Ԫ�صļ�ֵ��obj��ȣ������̲���obj
		{
			if (equals(get_key(cur->val), get_key(obj))//obj�ļ�ֵ�Լ����ڣ������̲���
			{
				node* tmp = new_node(obj)��
					tmp->next = first;
					buckets[n] = tmp;
					++num_elements;
					return tmp��
			}
		}

		//�˳�ѭ����˵�����Բ���obj����˴����½ڵ�
		node* tmp = new_node(obj);
		//�����н�tmp���뵽Ͱ��Ϊ�׽ڵ�
		tmp->next = first; /
			buckets[n] = tmp;
		//����Ԫ�ظ���
		++num_elements;
		return tmp;
	}

	//�汾1��ת���汾4
	size_type btk_num(const value_type& obj, size_t n)const
	{
		return btk_num_key(get_key(obj), n);
	}

	//�汾2��ת���汾3
	size_type btk_num(const value_type& obj)const
	{
		return btk_num_key(get_key(obj));
	}

	//�汾3��ת���汾4
	size_type btk_num_key(const value_type& key)const
	{
		return btk_num_key(key, buckets.size());
	}

	//�汾4,ת��STL��hash()�������ܹ����ݻ�����������size_t���͵�ֵ��
	size_type btk_num_key(const value_type& key, size_t n)const
	{
		return hash(key) % n;
	}

	//���
	void clear()
	{
		for (size_type i = 0; i < buckets.size(); ++i)
		{
			node* cur = buckets[i];//��ȡ��Ͱ���׽ڵ�
			while (cur)//�����ͷ�Ͱ�Ľڵ�
			{
				node* next = cur->next;
				delete_node(cur);
				cur = next;
			}
			buckets[i] = 0;//�൱��ָ���ÿ�
		}
		num_elements = 0;//��Ԫ�ؼ�����Ϊ0
	}

	//����
	void copy_from(const hashtable& ht)
	{
		buckets.clear();//�Ƚ��Լ���vector���
		buckets.reserve(ht.buckets.size());//ΪvectorԤ��Ŀ��ht��vector��size(),���ht�Ŀռ�С��������;
		buckets.insert(buckets.end(), ht.buckets.size(), (node*)0);//��vector����ht���ȵ�0ָ�룬�൱�ڽ�����Ͱ��ָ���ÿ�
		__STL_TRY
		{
			for (size_type i = 0; i < ht.buckets.size(); ++i)
			{
				if (const node* cur = ht.buckets[i])//��ȡ��ÿ��Ͱ�����Ͱ��Ϊ����
				{
					node* copy = new_node(cur->val);//�����׽ڵ�ڵ㣬�������Լ���Ͱ
					buckets[i] = copy;
					for (node* next = cur->next; next; cur = next, next = cur->next)//����Ͱ���ÿ��Ԫ��
					{
						copy->next = new_node(next->val);//����һ���½ڵ㣬�������׽ڵ��nextָ�룬�׽ڵ����
						copy = copy->next;
					}
				}
			}
		num_elements = ht.num_elements;
		}
		__STL_UNWIND(clear());
	}

	//����
	iterator find(const key_type& key)
	{
		size_type n = btk_num_key(key);//�ҵ���Ӧ��Ͱ
		node* first;
		for (first = buckets[n]; first && !equals(get_key(first), key); first = first->next)
		{}
		return iterator(first, this);
	}

	size_type count(const key_type& key)const
	{
		const size_type n = btk_num_key(key);
		size_type result = 0;
		for (const node* cur = buckets[n]; cur; cur = cur->next)
		{
			if (equals(get_key(cur->val), key))
				++result;
		}
		return result;
	}
};

/*
	STL��¼��hash function�����ڽ����������ת�����ܹ���nȡģ��ֵ��
	��ʵ�ϣ�����char��int��long��ԭ����ֵ���ͣ�hash function����ԭ�����ء�
	�������ַ���const char*���ͣ���������µ�ת��������

*/
template <class Key> struct hash {};//hash function�º����Ľṹ��������Ϊ��
inline size_t __stl_hash_string(const char* s)
{
	unsigned long h = 0;
	for (; *s; ++s)
	{
		h = 5 * h + *s;//��λ�ַ���5^n��
	}
	return size_t(h);
}

/*
	const char* ��string�ǲ�һ���ġ�string��string.h�������ĸ������ͣ�������stl�ṩ��hashfunction����ȡhashֵ��
	��hash(string("hello"))�ᱨ��
	��hash("hello")����
*/


/*
	hashtable��ʹ��

	����һ����ֵ��ʵֵԪ�ض���int��hashtable��ϣ������Ϊ50
	hashtable<int,int,hash<int>,identity<int>,equal_to<int>,alloc> iht(50,hash<int>(),equal_to<int>());

	cout<<iht.size()<<endl;   //0����Ϊ����Ϊ��
	cout<<iht.bucket_count()<<endl;  //53������50�ĵ�һ������

	iht.insert_unique(59);
	iht.insert_unique(63);
	iht.insert_unique(108);
	iht.insert_unique(2);
	iht.insert_unique(53);
	iht.insert_unique(55);
	
	����һ����Ӧ�ĵ�����
	hashtable<int,int,hash<int>,identity<int>,equal_to<int>,alloc>::iterator ite = iht.begin();

	��ӡ����Ԫ��  59  63  108  2  53  55
	for(int i=0;i<iht.size();++i��++ite)
		cout<<*ite<<"  ";
	cout<<endl;


*/