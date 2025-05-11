/*
	set的底层是rbtree实现的
	set的特性是所有元素会自动排序。（set的实值就是键值）
	set的迭代器不能对元素修改，不过可以插入和删除，因此是const_iterator
*/
#include <functional>
using namespace std;

template <class Key,class Compare=less<Key>,Alloc=alloc>
class set {
public:
	typedef Key key_type;
	typedef Key value_type;
	typedef Compare key_compare;
	typedef Compare value_compare;

private:
	typedef rb_tree<Key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;

	rep_type t;

public:
	typedef typename rep_type::const_pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::const_reference reference;
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::const_iterator iterator;
	typedef typename rep_type::const_iterator const_iterator;
	typedef typename rep_type::const_reverse_iterator reverse_iterator;
	typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type;

	//构造函数，用默认或指定的比较函数传给t，然后什么都不做
	set() : t(Compare()) {}
	explicit set(const Compare& comp) :t(comp) {}

	//拷贝构造函数
	set(const set<Key, Compare, Alloc>& x) :t(x.t) {}

	//泛化的构造函数，用泛化的迭代器传递一个区间给t赋值，使用rbtree的insert_unique
	template<class InputIterator>
	set(InputIterator first, InputIterator last) : t(Compare()) { t.insert_unique(first, last); }

	//用区间赋值的同时指定比较函数
	template<class InputIterator>
	set(InputIterator first, InputIterator last, const Compare& comp) : t(comp) { t.insert_unique(first, last); }

	//运算符重载。其中==和<是泛化的友元.set后面的模板参数可以省略，函数的空模板参数不能省略
	set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc>& x) { t = x.operator=; return *this; }
	friend bool operator==<>(const set&, const set&);
	friend bool operator<<> (const set&, const set&);

	//读取函数，调用自rbtree
	key_compare key_comp()const { return t.key_comp(); }
	value_compare value_comp()const { return t.key_comp(); }//实值即键值
	iterator begin()const { return t.begin(); }
	iterator end()const { return t.end(); }
	reverse_iterator rbegin()const { return t.rbegin(); }
	reverse_iterator rend()const { return t.rend(); }
	bool empty()const { return t.empty(); }
	size_type size()const { return t.size(); }
	size_type max_size()const { return t.max_size(); }
	void swap(set<Key, Compare, Alloc>& x) { t.swap(x.t); }

	//修改函数，插入和删除
	typedef pair<iterator, bool> pair_iterator_bool;
	pair<iterator, bool> insert(const value_type& x) {
		pair<typename rep_type::iterator, bool>p = t.insert_unique(x);//插入后返回的是一个对组
		return pair<iterator, bool>(p.first, p.second);//将上面的rep_type::iterator转化为iterator
	}

	iterator insert(iterator position, const value_type& x) {
		typedef typename rep_type::iterator rep_iterator;
		return t.insert_unique((rep_iterator&)position, x);
	}

	template<class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		t.insert_unique(first, last);
	}

	void erase(iterator position) {
		typedef typename rep_type::iterator rep_iterator;
		t.erase((rep_iterator&)position);
	}
	
	size_type erase(const key_type& x) {
		return t.erase(x);
	}

	void erase(iterator first, iterator last) {
		typedef typename rep_type::iterator rep_iterator;
		t.erase((rep_iterator&)first, (rep_iterator&)last);
	}

	void clear() { t.clear(); }

	//set提供的函数
	iterator find(const key_type& x)const { return t.find(x); }

	size_type count(const key_type& x)const { return t.count(x); }

	iterator lower_bound(const key_type& x)const { return t.lower_bound(x); }

	iterator upper_bound(const key_type& x)const { return t.upper_bound(x); }

	pair<iterator, iterator> equal_range(const key_type& x)const { return t.equal_range(x); }

};

//两个友元重载
template<class Key,class Compare ,class Alloc>
inline bool operator==(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
	return x.t == y.t;//rbtree的==重载
}

template<class Key, class Compare, class Alloc>
inline bool operator==(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
	return x.t < y.t;
}

