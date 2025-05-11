#pragma once

/*
	STL的HashTable是hashmap和hashset的底层容器。
	hashtable的寻址方法为开链法，中控vector的每个结点维护一个链表（而非STL的list，因为不需要那么多功能）
	注意：
		hashtable虽然支持键值：实值的元素形式，但本质上只存储了实值，键值是根据ExtractKey函数从实值计算得出的。
		因此，如果两个不同的实值根据ExtractKey函数得到的键值是相同的，则不能插入这两个元素。
		hashSet的键值就是实值，因此只存储实值，ExtractKey函数就是直接返回实值
		hashMap的键值是Key,实值是pair<const Key,Value>，因此键值和实值都存储，ExtractKey函数返回select1st<pair<...>>。
*/

//hash_table的节点，包含此节点的值和一个指向下一个节点的指针
template <class Value>
struct __hashtable_node
{
	__hashtable_node* next;
	Value val;
};

//hash_table的迭代器，注意：只支持++，不支持--
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
			size_type bucket = ht->bkt_num(old->val);//btk_num是hashtable的函数，根据val得到所在桶的索引
			while (!cur && ++bucket < ht->buckets.size())//如果下一个桶是空的，就去下下个桶，直到到达vector的尾部
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


//hashtable的数据结构
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
		const size_type n_buckets = next_size(n);//获取到大于等于n的第一个质数，（来自STL内定的28个质数数组）
		buckets.reserve(n_buckets);//使用vector的reserve来预留上面那个质数的空间，但vector的首尾元素没变，此处vector为空。
		buckets.insert(buckets.end(), n_buckets, (node*)0);//在vector的尾部补充0，此处即将vector的元素全部设为0
		num_elements = 0;
	}

	//重新设定hashtable的桶的大小
	void resize(size_type num_elements_hint)
	{
		const size_type old_n = buckets.size();
		if (num_elements_hint > old_n)
		{
			const size_type n = next_size(num_elements_hint);//获取到下一个质数
			if (n > old_n)
			{
				vector<node*, A> tmp(n, (node*)0);//构造一个新的vector，容量为n
				__STL_TRY
				{
					for (size_type bucket = 0; bucket < old_n; ++bucket)//遍历每个桶
					{
						node* first = buckets[bucket];//取出桶的首节点，
						while (first)//当first指针不为空，即桶尚未遍历完
						{
							size_type new_bucket = btk_num(first->val, n);//根据first的val计算应该处于新hashtable的哪个桶中
							buckets[bucket] = first->next;//buckets[bucket]原本指向first，改为指向first->next，相当于将first拿出
							first->next = tmp[new_bucket];//first的next指向tmp的对应桶，下一行，让桶的首节点指向first，相当于将first插入到桶中成为第一个节点
							tmp[new_bucket] = first;
							first = buckets[bucket];//重新定位first，以便下一次循环。
						}
					}
				buckets.swap(tmp);//将两个桶互换，此函数结束后tmp自动释放
				}
			}
		}
	}

	//以下insert_unique和insert_equal分别实现了hash_XXX和hash_multiXXX
	//插入元素，不允许键值相同,返回pair<迭代器,是否插入成功>
	pair<iterator, bool> insert_unique(const value_type& obj)
	{
		resize(num_elements + 1);//此处为hashtable的成员函数
		return insert_unique_noresize(obj);
	}
	pair<iterator, bool> insert_unique_noresize(const value_type& obj)
	{
		const size_type n = btk_num(obj);//计算obj应该在哪个桶
		node* first = buckets[n];//获取该桶的首节点
		for (node* cur = first; cur; cur = cur->next)//遍历该桶，如果找到某个元素的键值与obj相等，返回false
		{
			if(equals(get_key(cur->val),get_key(obj))
				return pair<iterator,bool> (iterator(cur,this),false);
		}

		//退出循环，说明可以插入obj，因此创建新节点
		node* tmp = new_node(obj);
		//此两行将tmp插入到桶做为首节点
		tmp->next = first; /
		buckets[n] = tmp;
		//增加元素个数
		++num_elements;
		return pair<iterator, bool>__hashtable_iterator((tmp, this), true);
	}

	//插入元素,允许键值重复。返回迭代器。（与insert_unique 不同的是返回值为迭代器）
	iterator insert_equal(const value_type& obj)
	{
		resize(num_elements + 1);//此处为hashtable的成员函数
		return insert_equal_noresize(obj);
	}
	iterator insert_equal_noresize(const value_type& obj)
	{
		const size_type n = btk_num(obj);//计算obj应该在哪个桶
		node* first = buckets[n];//获取该桶的首节点
		for (node* cur = first; cur; cur = cur->next)//遍历该桶，如果找到某个元素的键值与obj相等，则立刻插入obj
		{
			if (equals(get_key(cur->val), get_key(obj))//obj的键值以及存在，则立刻插入
			{
				node* tmp = new_node(obj)；
					tmp->next = first;
					buckets[n] = tmp;
					++num_elements;
					return tmp；
			}
		}

		//退出循环，说明可以插入obj，因此创建新节点
		node* tmp = new_node(obj);
		//此两行将tmp插入到桶做为首节点
		tmp->next = first; /
			buckets[n] = tmp;
		//增加元素个数
		++num_elements;
		return tmp;
	}

	//版本1，转调版本4
	size_type btk_num(const value_type& obj, size_t n)const
	{
		return btk_num_key(get_key(obj), n);
	}

	//版本2，转调版本3
	size_type btk_num(const value_type& obj)const
	{
		return btk_num_key(get_key(obj));
	}

	//版本3，转调版本4
	size_type btk_num_key(const value_type& key)const
	{
		return btk_num_key(key, buckets.size());
	}

	//版本4,转调STL的hash()函数，能够根据基本类型生成size_t类型的值。
	size_type btk_num_key(const value_type& key, size_t n)const
	{
		return hash(key) % n;
	}

	//清除
	void clear()
	{
		for (size_type i = 0; i < buckets.size(); ++i)
		{
			node* cur = buckets[i];//获取到桶的首节点
			while (cur)//遍历释放桶的节点
			{
				node* next = cur->next;
				delete_node(cur);
				cur = next;
			}
			buckets[i] = 0;//相当于指针置空
		}
		num_elements = 0;//将元素计数设为0
	}

	//复制
	void copy_from(const hashtable& ht)
	{
		buckets.clear();//先将自己的vector清空
		buckets.reserve(ht.buckets.size());//为vector预留目标ht的vector的size(),如果ht的空间小，则不做事;
		buckets.insert(buckets.end(), ht.buckets.size(), (node*)0);//在vector插入ht长度的0指针，相当于将所有桶的指针置空
		__STL_TRY
		{
			for (size_type i = 0; i < ht.buckets.size(); ++i)
			{
				if (const node* cur = ht.buckets[i])//获取到每个桶，如果桶不为空则
				{
					node* copy = new_node(cur->val);//创建首节点节点，并赋给自己的桶
					buckets[i] = copy;
					for (node* next = cur->next; next; cur = next, next = cur->next)//遍历桶里的每个元素
					{
						copy->next = new_node(next->val);//创建一个新节点，并赋给首节点的next指针，首节点后移
						copy = copy->next;
					}
				}
			}
		num_elements = ht.num_elements;
		}
		__STL_UNWIND(clear());
	}

	//查找
	iterator find(const key_type& key)
	{
		size_type n = btk_num_key(key);//找到对应的桶
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
	STL收录的hash function，用于将传入的数据转化成能够被n取模的值。
	事实上，对于char、int、long等原生数值类型，hash function几乎原样返回。
	而对于字符串const char*类型，设计了如下的转换函数：

*/
template <class Key> struct hash {};//hash function仿函数的结构体声明，为空
inline size_t __stl_hash_string(const char* s)
{
	unsigned long h = 0;
	for (; *s; ++s)
	{
		h = 5 * h + *s;//高位字符乘5^n，
	}
	return size_t(h);
}

/*
	const char* 和string是不一样的。string是string.h内声明的复杂类型，不能用stl提供的hashfunction来获取hash值。
	如hash(string("hello"))会报错
	但hash("hello")不会
*/


/*
	hashtable的使用

	创建一个键值和实值元素都是int的hashtable，希望长度为50
	hashtable<int,int,hash<int>,identity<int>,equal_to<int>,alloc> iht(50,hash<int>(),equal_to<int>());

	cout<<iht.size()<<endl;   //0，因为容器为空
	cout<<iht.bucket_count()<<endl;  //53，大于50的第一个质数

	iht.insert_unique(59);
	iht.insert_unique(63);
	iht.insert_unique(108);
	iht.insert_unique(2);
	iht.insert_unique(53);
	iht.insert_unique(55);
	
	创建一个对应的迭代器
	hashtable<int,int,hash<int>,identity<int>,equal_to<int>,alloc>::iterator ite = iht.begin();

	打印所有元素  59  63  108  2  53  55
	for(int i=0;i<iht.size();++i，++ite)
		cout<<*ite<<"  ";
	cout<<endl;


*/