/*
 *
 *STL�Ĺ�ϣ��ʹ�ÿ����ṹ��������
 *�п�����vector�������ÿһ��Ԫ�ض���list��ָ�롣
 *����һ��Ԫ��(������ֵ��ʵֵ)�����ü�ֵ��vector��sizeȡ��
 *Ȼ����������ҵ���Ӧ������
 *
 *��ϣ���ģ�������
 *Value��ʵֵ
 *Key:��ֵ
 *HashFcn:ɢ�к���
 *ExtractKey:��ȡ��ֵ�ķº���
 *EqualKey:�жϼ�ֵ�ķº���
 *Alloc:�ռ�������
 */


//���ڵ�
template<class Value>
struct __hashtable_node {
	__hashtable_node* next;
	Value val;
};

//��ϣ�������
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
			size_type bucket = ht->bkt_num(old->val);//��������������±�
			while (!cur && ++bucket < ht->bucket.size())//�±�++�����û����
				cur = ht->buckets[bucket];
		}
		return *this;
	}

	iterator operator++(int){
		iterator tmp = *this;
		++* this;
		return tmp;
	}
	//��ϣ�������û��--����
};

/*
 *��ϣ����п���vector�ĳ���ͨ��������������ϣ����̬��չ����Ȼ������
 *STLԤ���ṩ��28��������ÿһ��������һ����Լ����
 *
 */
static const int __stl_num_primes = 28;
static const unsigned long __stl_prime_list[__stl_num_primes] =
{
	53,97,193,389,769//����ʡ�ԣ����Ϊ4294967291ul
};

inline unsigned long __stl_next_prime(unsigned long n) {
	const unsigned long* first = __stl_prime_list;
	const unsigned long* last = __stl_prime_list + __stl_num_primes;
	const unsigned long* pos = lower_bound(first, last, n);//lower_bound�Ƿ����㷨�����������в�С��n��Ԫ�صĵ�����

	return pos == last ? *(last - 1) : *pos;
}



//��ϣ�����ݽṹ
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

	//���ýڵ�ռ�
	node* new_node(const value_type& obj) {
		noden n = node_allocator::allocate();
		n->next = 0;
		__STL_TRY{
			construct(&n->val,obj);
		return n;
		}
		__STL_UNWIND(node_allocator::deallocate(n));
	}

	//�ͷŽڵ�
	void delete_node(node* n) {
		destroy(&n->val);
		node_allocator::deallocate(n);
	}

	//���캯���������ֱ��ǣ�Ԫ�ظ�����ɢ�к������ȽϺ���
	hashtable(size_type n, const HashFcn& hf, const EqualKey& eql) :
		hash(hf), equals(eql, get_key(ExtractKey()), num_elements(0)) {

		initialize_buckets(n);
	}

	void initialize_buckets(size_type n) {
		const size_type n_buckets = next_size(n);
		buckets.reserve(n_buckets);
		buckets.insert(buckets.end(), n_buckets, (node*)0);
	}

	//�ж��Ƿ���Ҫ�ؽ�����������Ҫ���̷��أ���Ҫ��ִ��
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
	//��ʼ����һ��ӵ��50���ڵ��hashtable
	hashtable<int, int, hash<int>, identity<int>, equal_to<int>, alloc> iht(50, hash<int>(), equal_to<int>);


}