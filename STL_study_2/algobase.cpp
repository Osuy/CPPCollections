/*
	equal(first1,last1,first2)
	equal(first1,last1,first2,op)
	比较两个等长的区间 元素是否相等，相等返回true
	第二个区间只传进一个first2迭代器，所以可能会出现区间2比区间1短
	因此最好在调用equal前判断两个区间是否相等
*/

#include <iterator>
using namespace std;
template<class  InputIterator1 ,class InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
	for (; first1 != last1; ++first1, ++first2)
		if (*first1 != *first2)//迭代器解引用后比较，因此不要求迭代器类型一致，关键看迭代器如何对*号重载
			return false;
	return true;
}

template<class  InputIterator1, class InputIterator2,class BinaryPredicate>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,BinaryPredicate binary_pred) {
	for (; first1 != last1; ++first1, ++first2)
		if (!binary_pred(*first1,*first2))//用仿函数来判定
			return false;
	return true;
}

/*
	fill(first,last,value)
	fill_n(first,n,value)
	对区间用value填充，会修改原本的值
	如果只确定要填充多少个，不想去计算last迭代器，可以用fill_n，填充n个元素
	fill_n返回填充完的最后一个元素的下一个位置
*/

template<class ForwardIterator ,class T>
void fill(ForwardIterator first, ForwardIterator last, const T& value) {
	for (; first != last; ++first)//循环，对区间用value填充
		*first = value;
}


template<class OutputdIterator, class Size,class T>
OutputdIterator fill_n(OutputdIterator first,Size n , const T& value) {
	for (;n>0;--n, ++first)//循环，对区间用value填充
		*first = value;
	return first;//返回区间最后一个元素的下一个位置
}

/*
	iter_swap
*/

template<class ForwardIterator1,class ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b) {
	__iter_swap(a, b, value_type(a));
}

template <class ForwardIterator1,class ForwardIterator2,class T>
inline void __iter_swap(ForwardIterator1 a, ForwardIterator2, T*) {
	T tmp = *a;
	*a = *b;
	*b = tmp;
}

//以上通过第三章的traits技法获取了ForwardIterator1的类型，并用其定义了tmp变量
//当然也可以这么写
template<class ForwardIterator1, class ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b) {
	typename iterator_traits<ForwardIterator1>::value_type tmp = *a;
	*a = *b;
	*b = tmp;
}

/*
	lexicographical_compare(first1,last1,first2,last2)
	比较两个区间的元素是否相等
	和之前只需要第二个区间的first不同，区间长短也决定了返回结果，所以是两对完整的区间
	对第一个不匹配的元素，如果first1小，返回true，否则返回false
	如果全部都匹配，但两个区间长度不同，如果先抵达last1，返回true；先到达last2，返回false
	如果两个区间长度相同，返回false
*/

template<class InputIterator1 ,class InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (*first1 < *first2)
			return true;
		if (*first2 < *first1)
			return false;
	}
	return first1 == last1 && first2 != last2;
}

template<class InputIterator1, class InputIterator2,class Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp) {
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (comp(*first1 , *first2))
			return true;
		if (comp(*first2, *first1))
			return false;
	}
	return first1 == last1 && first2 != last2;
}

//原生指针版,用unsigned char来比较内存中的每一个bytes
inline bool lexicographical_compare(const unsigned char* first1,
									const unsigned char* last1,
									const unsigned char* first2,
									const unsigned char* last2) {

	const size_t len1 = last1 - first1;
	const size_t len2 = last2 - first2;
	const int result = memcmp(first1, first2, min(len1, len2));
	return result != 0 ? result < 0 : len1 < len2;
}

/*
	max(a,b)
	比较a、b的大小，返回较大的
*/

template <class T>
inline const T& max(const T& a, const T& b) {
	return a < b ? b : a;//复杂类型需要重载<操作符
}

template<class T ,class Compare>
inline const T& max(const T& a, const T& b, Compare comp) {
	return comp(a, b) ? b : a;
}

/*
	min(a,b)
	返回a、b中较小的
*/

template<class T>
inline const T& min(const T& a, const T& b) {
	return b < a ? b : a;
}

template<class T, class Compare>
inline const T& min(const T& a, const T& b, Compare comp) {
	return comp(b, a) ? b : a;
}

/*
	mismatch(first1,last1,first2)
	逐一匹配两个区间的元素是否相等，找到第一个不匹配点
	返回两个迭代器组成的pair，分别指向两个区间的不匹配点
*/

template <class InputIterator1,class InputIterator2>
pair< InputIterator1, InputIterator2> //返回值
mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
	while (first1 != last1 && *first == *first2) 
		++first1, ++first2;
	return pair< InputIterator1, InputIterator2>(first1, first2);
}

template <class InputIterator1, class InputIterator2,class BinaryPredicate>
pair< InputIterator1, InputIterator2> //返回值
mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,BinaryPredicate binary_pred) {
	while (first1 != last1 && binary_pred(*first, *first2))
		++first1, ++first2;
	return pair< InputIterator1, InputIterator2>(first1, first2);
}

/*
	swap
	交换两个对象的内容
*/

template <class T>
inline void swap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}

/*
	copy(first，last，result)
	将区间[first，last)的元素复制到[result,result+(last-first))区间
	最复杂的算法，针对不同的类型做了优化
*/

template<class InputIterator,class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
	return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
}

//针对原生指针的重载
inline char* copy(const char* first, const char* last, char* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}

inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
	memmove(result, first, sizeof(wchar_t)*(last - first));
	return result + (last - first);
}

//__copy_dispatch
//完全泛化版，重载()调用__copy
template <class InputIterator ,class OutputIterator>
struct __copy_dispatch {
	OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
		return __copy(first, last, result, iterator_category(first));
	}
};

//偏特化版本，两个模板参数都指定为T*
template<class T>
struct __copy_dispatch<T*, T*> {
	T* operator()(T* first, T* last, T* result) {
		typedef typename __type_traits<T>::has_trivial_assignment_operator t;
		return __copy_t(first, last, result, t());
	}
};

//偏特化版本，模板参数指定为const T* 和 T*
template <class T>
struct __copy_dispatch<const T*,T*>{
	T* operator()(const T* first, const T* last, T* result) {
		typedef typename __type_traits<T>::has_trivial_assignment_operator t;
		return __copy_t(first, last, result, t());
	}
};

//以上，调用copy函数时，传进原生指针，使用对应的原生版本
//传进复杂类型，且返回类型也一致，使用偏特化版本，跳转__copy_t
//传进复杂类型，返回类型不一致，调用完全泛化版本，跳转__copy

//__copy针对不同类型的迭代器做了不同的重载
//InputIterator版本
template<class InputIterator,class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
	for (; first != last; ++result, ++first)//m每次循环要执行first != last，速度慢
		*result = *first;
	return result;
}

//RandomAccessIterator版本
template<class RandomAccessIterator, class OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, 
							 OutputIterator result, random_access_iterator_tag) {
	return __copy_d(first, last, result, distance_type(first));
}

template<class RandomAccessIterator,class OutputIterator ,class Distance>
inline OutputIterator 
__copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*) {
	for (Distance n = last - first; n > 0; --n, ++result, ++first)//使用n进行循环，速度快
		*result = *first;
	return result;
}
