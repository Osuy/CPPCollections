/*
	
*/

//list�ڵ�,������һ��T�������ݺ�����ָ��
//��Ϊָ��ָ����������ڵ㣬������void*������ָ���κ�����
//void*һ��������ĳ�����ͣ��Ͳ�����ת������
template<class T>
struct __list_node{
	typedef void* void_pointer;
	void_pointer prev;
	void_pointer next;
	T data;
};

//list������
template<class T,class Ref,class Ptr>
struct __list_iterator {

	//һ���������������һ���ģ��������͵ļ�д
	typedef __list_iterator<T, T&, T*>   iterator;
	typedef __list_iterator<T, Ref, Ptr> self;

	typedef bidirectional_iterator_tag iterator_category;
	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef __list_node<T>* link_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	//link_type��ָ�룬���Ե������ڲ�����һ��nodeָ�룬ָ��Ԫ��
	link_type node;

	//���캯����list�Ĺ��캯���Ǵ���һ�����û����ã�����x���Ϊ��Ա
	__list_iterator(link_type x) :node(x){}
	__list_iterator() {}
	__list_iterator(const iterator& x):node(x.node){}


	//���������������
	bool operator==(const self& x)const { return node == x.node; }

	bool operator!=(const self& x)const { return node != x.node; }

	reference operator*()const { return (*node).data; }

	pointer operator->()const { return &(operator*()); }

	self& operator++() {
		node = (link_type)((*node).next);
	}

	self operator++(int) {
		self tmp = *this;
		++* this;
		return tmp;
	}

	self& operator--() {
		node = (link_type)((*node).prev);
	}

	self operato--(int) {
		self tmp = *this;
		--* this;
		return tmp;
	}
};

/*
stl��list�ǻ�״˫�������Ϊ���ܹ�ʹ��stl�ṩ�ĵ���������Ҫʵ��ǰ�պ�
��ν����������list<sometype> ilist; ʵ������һ��ָ��ĩβԪ�أ�������node����ָ��
ĩβԪ�ز����������У�����.nextָ���һ��Ԫ�أ���һ��Ԫ��.prevָ��node
������begin()����node.next��Ҳ���ǵ�һ��Ԫ��
end()����node,ʵ����ǰ�պ󿪵�Ч��
*/

template<class T,class Alloc=alloc>
class list {
protected:
	typedef __list_node<T>           list_node;
	typedef __list_iterator<T,T&,T*> iterator;

public:
	typedef list_node* link_type;
	typedef simple_alloc<lisi_node, Alloc> list_node_allocator;

	list() { empty_initialize(); }

protected:
	link_type node;//node�����Ƿ�װ��ǰ��ָ�������

protected:
	link_type get_node() { return list_node_allocator::allocate();}

	void put_node(link_type p) { list_node_allocator::deallocate(); }

	link_type create_node(const T& x) {
		link_type p = get_node();
		construct(&p->data, x);
		return p;
	}

	void destroy_node(link_type p) {
		destroy(&p->data);
		put_node(p);
	}

	iterator begin() { return (link_type)((*node).next); }

	iterator end() { return node; }

	bool empty()const { return node->next == node; }

	size_type size()const {
		size_type result = 0;
		distance(begin(), end(), result);//��������ľ���
		return result;
	}

	reference front() { return *begin(); }

	reference back() { return *(--end()); }

	void empty_initialize() {
		node = get_node();
		node->next = node;
		node->prev = node;
	}

	//���뺯������ν�Ĳ���ʱָ����...֮ǰ���롱
	iterator insert(iterator position, const T& x) {
		link_type tmp = create_node(x);
		tmp->next = position.node;
		tmp->prev = position.node->prev;
		(link_type(position.node->prev))->next = tmp;

	}
};
