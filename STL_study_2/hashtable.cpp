/*
 *
 *STL的哈希表使用开链结构，像珠帘
 *中控器是vector，里面的每一个元素都是list的指针。
 *插入一个元素(包括键值和实值)，先用键值对vector的size取余
 *然后根据余数找到对应的链表
 *
 *哈希表的模板参数：
 *Value：实值
 *Key:键值
 *HashFcn:散列函数
 *ExtractKey:提取键值的仿函数
 *EqualKey:判断键值的仿函数
 *Alloc:空间配置器
 */


//链节点
template<class Value>
struct __hashtable_node {
	__hashtable_node* next;
	Value val;
};

//哈希表迭代器
template <class Value ,class Key,class HashFcn,class ExtractKey,class EqualKey,class Alloc>
struct __hashtable_iterator {
	typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> hashtable;
	typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
	typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;
	typedef __hashtable_node<Value> node;

	typedef forward_iterator_tag iterator_category;
	typedef Value value_type;
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;
	typedef Value& reference;
	typedef Value pointer;

	node* cur;
	hashtable* ht;

	__hashtable_iterator(node* n,hashtable* tab):cur(n),ht(tab){}
	__hashtable_iterator() {}

	reference operator*()const { return cur->val; }
	pointer operator->()const { return &(operator*()); }
	bool operator==(const iterator& it)const { return cur == it.cur; }
	bool operator!=(const iterator& it)const { return cur != it.cur; }

	iterator& operator++(){
		const node* old = cur;
		cur = cur->next;
		if (!cur) {
			size_type bucket = ht->bkt_num(old->val);//返回所在链表的下标
			while (!cur && ++bucket < ht->bucket.size())//下标++，如果没出界
				cur = ht->buckets[bucket];
		}
		return *this;
	}

	iterator operator++(int){
		iterator tmp = *this;
		++* this;
		return tmp;
	}
	//哈希表迭代器没有--操作
};

/*
 *哈希表的中控器vector的长度通常是质数，并且希望动态扩展后依然是质数
 *STL预先提供了28个质数，每一个都是上一个大约两倍
 *
 */
static const int __stl_num_primes = 28;
static const unsigned long __stl_prime_list[__stl_num_primes] =
{
	53,97,193,389,769//后面省略，最大为4294967291ul
};

inline unsigned long __stl_next_prime(unsigned long n) {
	const unsigned long* first = __stl_prime_list;
	const unsigned long* last = __stl_prime_list + __stl_num_primes;
	const unsigned long* pos = lower_bound(first, last, n);//lower_bound是泛型算法，返回区间中不小于n的元素的迭代器

	return pos == last ? *(last - 1) : *pos;
}



//哈希表数据结构
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc=alloc>
class hashtable {
public:
	typedef HashFcn hasher;
	typedef EqualKey key_equal;
	typedef size_t size_type;

private:
	hasher hash;
	key_equal equals;
	ExtractKey get_key;

	typedef __hashtable_node<Value> node;
	typedef simple_alloc<node, Alloc> node_allocator;

	vector<node*, Alloc> buckets;
	size_type num_elements;

public:
	size_type bucket_count()const { return buckets.size(); }

	//配置节点空间
	node* new_node(const value_type& obj) {
		noden n = node_allocator::allocate();
		n->next = 0;
		__STL_TRY{
			construct(&n->val,obj);
		return n;
		}
		__STL_UNWIND(node_allocator::deallocate(n));
	}

	//释放节点
	void delete_node(node* n) {
		destroy(&n->val);
		node_allocator::deallocate(n);
	}

	//构造函数，参数分别是：元素个数，散列函数，比较函数
	hashtable(size_type n, const HashFcn& hf, const EqualKey& eql) :
		hash(hf), equals(eql, get_key(ExtractKey()), num_elements(0)) {

		initialize_buckets(n);
	}

	void initialize_buckets(size_type n) {
		const size_type n_buckets = next_size(n);
		buckets.reserve(n_buckets);
		buckets.insert(buckets.end(), n_buckets, (node*)0);
	}

	//判断是否需要重建表格，如果不需要立刻返回；需要就执行
	void resize(size_type num_elements_hint) {
		const size_type old_n = buckets.size();
		if (num_elements_hint > old_n) {
			const size_type n = next_size(num_elements_hint);
			if (n > old_n) {

			}
		}
	}


};

void text() {
	//初始构造一个拥有50个节点的hashtable
	hashtable<int, int, hash<int>, identity<int>, equal_to<int>, alloc> iht(50, hash<int>(), equal_to<int>);


}