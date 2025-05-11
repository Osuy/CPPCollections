/*
	Set概述：
	Set的元素实值即是键值，并且不允许有多个键值相同。Set能根据元素的大小自动排序。
	本质上：
		Set存放的元素是Key（接就是Value），通过将Key的类型传给红黑树的Key和Value，来实现实值键值相同。
		set的迭代器类型为const_iterator。即不能通过迭代器来修改元素的值。因为元素的值关系到其所处位置，因此改变元素的值会打乱原有的组织。

	而Set是支持插入和删除的，并会在插入和删除后对其他元素进行调整，以保持底层是一颗二叉搜索树。
	Set的插入和删除不会改变其他迭代器的有效性。
*/

template <class Key,class Compare = less<Key>,class Alloc = alloc>
class set 
{
public:
	//模板参数声明
	typedef Key key_type;
	typedef Key value_type;//Key既是键值也是实值
	typedef Compare key_compare;
	typedef Compare value_compare;
	
private:
	
	//以下使底层红黑树的类型声明，其中，Key = key_type = Key，Value = value_type = Key，所以Set的底层红黑树的键值和实值使一样的
	//此外，KeyOfValue=identity<value_type>，indentity<T>为stl提供的一元函数，直接返回自身，定义如下：
	/*
	template <class T>
	struct identity :public unary_function<T, T>
	{
		const T& operator()(const T& x)const { return x; }
	};*/
	typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;
	
public:
	//内嵌类型声明
	typedef typename rep_type::const_pointer pointer;//指针类型都是常量指针，表示不可通过指针修改set的元素
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::const_reference reference;//常量引用，不可通过引用修改set的元素
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::const_iterator iterator;//迭代器为常量迭代器，不可通过迭代器修改set元素
	typedef typename rep_type::const_iterator const_iterator;
	typedef typename rep_type::const_reverse_iterator reverse_iterator;
	typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
};

/*
	Map概述：
	map的元素具有实值和键值，元素会根据键值自动排序。前提是提供了相应的Comparea
	map不能通过迭代器修改元素的键值，但可以通过迭代器修改元素的实值。
	本质上：
		map存放的元素是pair<const Key,T>，其中Key用const来修饰，因此不能修改Key，但是可以修改T（也就是Value）
		map的指针、引用和迭代器并不像set一样全是const_修饰的。
		map的指针、引用和迭代器能修改所指的元素，只不过元素是pair<const Key,T>
		因此通过KeyOfValue = select1st<value_type>从元素获取获取键值，这个键值是const Key，因此不能修改键值。

	而map是支持插入和删除的，并会在插入和删除后对其他元素进行调整，以保持底层是一颗二叉搜索树。
	map的插入和删除不会改变其他迭代器的有效性。
*/

template <class Key, class T, class Compare = less<T>, class Alloc = alloc>
class map
{
public:
	typedef Key key_type;
	typedef T data_type;
	typedef T mapped_type;
	typedef pair<const Key, T> value_type;
	typedef Compare key_compare;

	//以下是一个内部的functor类，作用是单纯地调用comp来比较元素
	class value_compare : public binary_function<value_type, value_type, bool>
	{
		friend class map <Key, T Compare, Alloc>;
	protected:
		Compare comp;
		value_compare(Compare c) :comp(c) {}
	public:
		bool operator()(const value_type& x, const value_type& y)const
		{
			return comp.(x.first, y.first);
		}
	};

private:
	//以下是底层红黑树，Key=key_type=Key,Value=value_type=pair<const Key, T>
	//KeyOfValue=select1st<value_type>=Key，表示从实值中获取键值的方法。
	typedef rb_tree<key_type, value_type, select1st<value_type>, key_compare, Alloc> rep_type;

public:
	//map是允许修改实值的，所以不是所有指针、引用和迭代器都用const_声明
	typedef typename rep_type::pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::reference reference;
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::iterator iterator;
	typedef typename rep_type::const_iterator const_iterator;
	typedef typename rep_type::reverse_iterator reverse_iterator;
	typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
};