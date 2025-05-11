/*
	概述：
	STL的List是一个双向循环链表，附带头节点位于首节点前一个位置。换言之：头节点就是end()
	list不能使用stl的sort，只能使用成员函数sort。因为stl的sort只支持randomAccessIterator
	优点：
		任意位置快速插入和删除，无需移动其他元素
	确定：
		访问需要遍历，效率不及Vector
*/

//List的节点
template<class T>
struct __list_node {
	typedef void* void_pointer;
	void_pointer prev;
	void_pointer next;
	T data;
};

//List的迭代器，是结构体
template<class T, class Ref, class Ptr>
struct __list_iterator {

	//内嵌类型声明
	typedef bidirectional_iterator_tag iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef Ptr pointer;
	typedef Ref reference;

	typedef __list_iterator<T, T&, T*>   iterator;
	typedef __list_iterator<T, Ref, Ptr> self;
	typedef __list_node<T>* link_type;
	typedef size_t size_type;
	

	//节点指针，一个迭代器内部当然有一个普通指针，然后根据这个指针重载迭代器的操作符
	link_type node;

	//构造
	__list_iterator(link_type x) :node(x) {}
	__list_iterator() {}
	__list_iterator(const iterator& x) :node(x.node) {}


	//操作符重载
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

//List的数据结构
template<class T, class Alloc = alloc>
class list {
protected:
	typedef __list_node<T>           list_node;

public:
	typedef list_node* link_type;
	typedef __list_iterator<T, T&, T*> iterator;
	typedef simple_alloc<list_node, Alloc> list_node_allocator;

protected:
	link_type node;//附加头节点，用于获取到链表

public:
	//构造
	list() { empty_initialize(); }

	//工具
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

	iterator insert(iterator position, const T& x) {
		link_type tmp = create_node(x);
		tmp->next = position.node;
		tmp->prev = position.node->prev;
		(link_type(position.node->prev))->next = tmp;

	}

protected:
	//内部工具
	link_type get_node() { return list_node_allocator::allocate(); }

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

	void empty_initialize() {
		node = get_node();
		node->next = node;
		node->prev = node;
	}

};