#include <cstddef>

//节录自SGI STL <stl_iterator.h>
//五种迭代器类型
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag {};
struct bidirectional_iterator_tag : public input_iterator_tag{};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

//自行开发迭代器最好继承std::iterator
template<class Category, class T, class Distance = ptrdiff_t,
		 class Pointer = T*, class Reference = T&>
struct iterator {
	typedef Category  iterator_category;
	typedef T         value_type;
	typedef Distance  difference_type;
	typedef Pointer   pointer;
	typedef Reference reference;
};

//特性提取机
template<class Iterator>
struct iterator_traits {
	typedef typename  Iterator::iterator_category  iterator_category;
	typedef typename  Iterator::value_type         value_type;
	typedef typename  Iterator::difference_type    difference_type;
	typedef typename  Iterator::pointer            pointer;
	typedef typename  Iterator::reference          reference;
};

//针对原生指针设计的偏特化traits
template<class T>
struct iterator_traits<T*> {
	typedef random_access_iterator_tag  iterator_category;
	typedef T                           value_type;
	typedef ptrdiff_t                   difference_type;
	typedef T*                          pointer;
	typedef T&                          reference;
};

//针对原生常量指针设计的偏特化traits
template<class T>
struct iterator_traits<const T*> {
	typedef random_access_iterator_tag  iterator_category;
	typedef T                           value_type;
	typedef ptrdiff_t                   difference_type;
	typedef const T*                    pointer;
	typedef const T&                    reference;
};

//选择迭代器的category
template<class Iterator>
inline typename iterator_traits<Iterator>::iterator_category  //这一行是返回类型
iterator_category(const Iterator&) {
	typedef typename  iterator_traits<Iterator>::iterator_category category;
	return category();
}

//选择迭代器的distance type
template<class Iterator>
inline typename iterator_traits<Iterator>::difference_type*  //这一行是返回类型
difference_type(const Iterator&) {
	return static_cast<typename  iterator_traits<Iterator>::difference_type>(0);
}