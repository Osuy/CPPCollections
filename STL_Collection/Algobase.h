#pragma once


/*
	equal
	判断两区间内元素是否相等，第二个区间若较长，多出部分不考虑
	因此第二个区间的前面一部分与第一个区间相等，会返回true
*/

template <class InputInterator1, class InputIterator2>
inline bool equal(InputInterator1 first1, InputInterator1 last1, InputIterator2 first2, InputIterator2 last2)
{
	for (; first1 != last1; ++first1, ++first2)
		if (*first1 != *first2)
			return false;
	return true;
}

//自定义比较版本
template <class InputInterator1, class InputIterator2,class BinaryPredicate>
inline bool equal(InputInterator1 first1, InputInterator1 last1, 
				  InputIterator2 first2, InputIterator2 last2, 
				  BinaryPredicate binary_pred
)
{
	for (; first1 != last1; ++first1, ++first2)
		if (binary_pred(*first1, *first2))
			return false;
	return true;
}


/*
	fill
	对区间用指定元素填充
	fill_n
	对目标位置和其后(n-1)个位置用指定元素填充
*/

template <class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last, const T& value)
{
	for (; first != last; ++first)
		*first = value;
}

//接受只写迭代器
template <class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value)
{
	for (; n > 0; --n, ++first)
		*first = value;
	return first;
}

/*
	find
	在区间内寻找与指定元素相同的元素，返回找到的第一个元素的迭代器
	find_if
	根据自定义条件寻找元素
	find_end
*/

template<class InputIterator ,class T>
InputIterator find(InputIterator first, InputIterator last, const T& value)
{
	while (first != last && *first != value) ++first;
	return first;
}

template<class InputIterator,class Predicate>
InputIterator find_if(InputIterator first, InputIterator last, Predicate pred)
{
	while (first != last && !pred(*first))++first;
	return first;
}

template <class ForwardIterator1,class ForwardIterator2>
inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
	ForwardIterator2 first2, ForwardIterator2 last2
)
{
	typedef typename iterator_traits<ForwardIterator1>::iterator_category category1;
	typedef typename iterator_traits<ForwardIterator2>::iterator_category category2;

	return __find_end(first1, last1, first2, last2, category1, category2);
}

 
/*
	copy
	拷贝算法，将[first,last)区间的元素拷贝到[result,result + (last-first))区间上
	copy对迭代器的要求几乎没有，要求[first,last)区间至少是Input迭代器，[result,result + (last-first))至少是Output迭代器
	因此，几乎可以用于任何容器间的拷贝。

	copy的具体细节是使用memmove或循环来复制元素。
	copy仍然有一些需要注意的情况：
		1.如果输入区间和输出区间是不重叠的，那当然没有问题
		2.如果输入区间的起点在输出区间内，由于输入区间覆盖时已经读取，因此也没有问题
		3.但是，如果输出区间在输入区间内，那么当输入区间迭代器到输出区间起点时，原本应该拷贝的值已经被之前的拷贝覆盖了。
	以上的第3点需要视情况而定，任何使用memmove都不会出错，而使用循环则一定会出错。
	例如，
	如果在3的情况下对vector拷贝，则正确；而对deque拷贝则错误
	因为vector的迭代器是原生指针T*，会调用memmove；而deque的迭代器是结构体，会使用循环
*/

//泛化copy函数，对外的唯一接口
template<class InputIterator,class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
	return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);//直接内部调转，利用偏特化区分路径
}

//针对const char* 的全特化，直接使用memmove
inline char* copy(const char* first, const char* last, char* result)
{
	memmove(result, first, last - first);//直接使用memmove移动内存数据
	return result + (last - first);//返回输出区间的末尾
}

//针对const wchar_t* 的全特化，直接使用memmove
inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
	memmove(result, first, last - first);
	return result + (last - first);
}

/*
	泛化的copy内部仅调用了__copy_dispatch（仿函数）
	__copy_dispatch有一个完全泛化版和两个偏特化版

	泛化版：调用__copy，除了原封不动地传入输入输出区间外，还传入了输入区间地迭代器category
	因为__copy根据参数的不同有许多重载版本。传入迭代器category，然后调用合适的重载版本，以提高效率

	偏特化版：调用__copy_t，是针对原生指针（即迭代器就是元素指针）的优化，判断有无trivial_assignment_operator（平凡的赋值操作）
	也就是原生的 = 操作，而不是重载的 = 。因为如果
*/
template <class InputIterator, class OutputIterator>
struct __copy_dispatch
{
	//()操作符重载
	OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
	{
		return __copy(first, last, result, iterator_category(first));//调用__copy，传入输入迭代器的category，目的是路径区分
	}
};

//偏特化版1，调用__copy_t
//__copy_t
template <class T>
struct __copy_dispatch<T*, T*>
{
	T* operator()(T* first, T* last, T* result)
	{
		typedef typename __type_traits<T>::has_trivial_assignment_operator t;
		return __copy_t(first, last, result, t());//调用__copy_t
	}
};

//偏特化版2，调用__copy_t，相交版本1，唯一区别是输入区间的迭代器被const修饰
template <class T>
struct __copy_dispatch<const T*, T*>
{
	T* operator()(const T* first, const T* last, T* result)
	{
		typedef typename __type_traits<T>::has_trivial_assignment_operator t;
		return __copy_t(first, last, result, t());//调用__copy_t
	}
};

//__copy，InputIterator版本
template <class InputIterator, class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag)
{
	for (; first != last; ++result, ++first)//循环条件：迭代器是否不等，速度慢
		*result = *first;
	return result;
}

//__copy，RandomAccessIterator版本，内部调用__copy_d，这个封装是为了提高复用，不是分出路径
template <class RandomAccessIterator, class OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag)
{
	return __copy_d(first, last, result, distance_type(first));
}

//__copy_d，用于__copy的RandomAccessIterator版本和__copy_t的has_trivial_assignment_operator版本
template <class RandomAccessIterator, class OutputIterator, class Distance>
inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*)
{
	for (Distance n = last - first; n > 0; --n, ++result, ++first)//循环条件：n>0，速度快
		*result = *first;
	return result;
}

//__copy_t，原生=操作版本
template <class T>
inline T* __copy_t(const T* first, const T* last, T* result, __true_type)
{
	memmove(result, first, sizeof(T) * (last - first));
	return result + (last - first);
}

//__copy_t，重载=操作版本
template <class T>
inline T* __copy_t(const T* first, const T* last, T* result, __false_type)
{
	return __copy_d(first, last, result, (ptrdiff_t*)0);
}


/*
	以上就是copy的全部细节。
	根据输入区间的迭代器类型，copy会选择调用合适的函数完成拷贝。
	以下的copy路径的整理：
	——char*：调用copy全特化版1，使用memmove
	——wchar_t*：调用全特化版2，使用memmove
	——其他迭代器：调转__copy_dispatch
		——原生指针：调用__copy_dispatch偏特化版1
			——转调__copy_t，额外参数has_trivial_assignment_operator
				——未重载 = ：使用memmove
				——重载了 = ：使用__copy_d
					——__copy_d：使用n>0循环
		——const原生指针：调用__copy_dispatch偏特化版2
			——转调__copy_t，额外参数has_trivial_assignment_operator
				——未重载 = ：使用memmove
				——重载了 = ：使用__copy_d
		——智能指针:调用__copy_dispatch完全特化版
			——转调__copy，额外参数iterator_category
				——input_iterator_tag：使用first!=last循环
				——random_access_iterator_tag：使用__copy_d
					——__copy_d：使用n>0循环


	任何使用memmove的copy，第3中情况不会出错
	任何使用循环的copy，第3中情况会出错
*/

