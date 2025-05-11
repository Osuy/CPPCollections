#pragma once
#include <string.h>
#include <iterator>
using namespace std;
/*
	对外接口：
		1.函数模板
		2.针对char*的重载
		2.针对wchar_t*的重载
	
	stl采用普通函数重载来处理char*和wchar_t*的拷贝
	普通函数重载比函数模板重载和函数模板全特化要更快
*/

//函数模板copy
template<class InputIterator, class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
	//使用防函数__copy_dispatch，根据迭代器的类型派发给不同的函数
	//因为是类模板，所有声明一个匿名对象时要带模板类型参数列表
	return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
}

//char*版重载
inline char* copy(const char* first, const char* last, char* result)
{
	//直接用memmove复制
	memmove(result, first, last - first);
	return result + (last - first);
}

//wchar_t*版重载
inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
	//直接用memmove复制
	memmove(result, first, sizeof(wchar_t) * (last - first));
	return result + (last - first);
}


//----------------------------------------------------------------------------------------
/*
	__copy_dispatch
		派发器，是类模板，是仿函数
		__copy_dispatch是copy择优路径中最重要的一步
		作用有二：
		1.提取迭代器category，第四参数传给__copy，调用不同的重载版本
		2.类模板偏特化，处理原生指针

	值得注意的是偏特化版本在处理原生指针时，将指针所知类型T是否具有寻常赋值操作符作为第四参数
	假如是基本类型（int, float, char），有寻常赋值操作符，则可以用memmove来拷贝
	假如是引用类型（结构体和类），没有寻常赋值操作服，只有重载operator=，因此需要遍历区间，逐个复制

*/

//通用类模板版本
template<class InputIterator, class OutputIterator>
struct __copy_dispatch
{
	OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
	{
		//提取迭代器性别作为第四参数传给__copy
		return __copy(firts, last, result, iterator_category(first));
	}
};

//针对原生指针的偏特化
template<class T>
struct __copy_dispatch<T*, T*>
{
	T* operator()(T* first, T* last, T* result)
	{
		typedef typename __type_traits<T>::has_trivial_assignmenet_operator t;
		return __copy_t(first, last, result, t());
	}
};

//针对const原生指针的偏特化
template<class T>
struct __copy_dispatch<const T*, T*>
{
	T* operator()(const T* first, const T* last, T* result)
	{
		typedef typename __type_traits<T>::has_trivial_assignmenet_operator t;
		return __copy_t(first, last, result, t());
	}
};


//----------------------------------------------------------------------------------------
/*

	_copy是一个函数模板，有多个重载，重载的前几个未定类型都是一样的，不同的是第四参数
	根据第四参数迭代器性别的不同，调用不同的__copy

	重载版本：
		input迭代器：支持++，不支持+n、-，因此只能循环逐次++，判断first!=last来
		random_access：支持++、+n、-，可以
*/

//input_iterator版本
template<class InputIterator, class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag)
{
	for (; first != last; ++result, ++first)
		*result = *first;
	return result;
}

//random_access_iterator版本
template<class RandomAccessIterator, class OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag)
{
	return __copy_d(first, last, result, distance_type(first));
}



//----------------------------------------------------------------------------------------
/*
	__copy_d和__copy_t

	__copy_t由__copy_dispatch的偏特化调用，用于处理原生指针
	__copy_t采用重载+占位参数来进行分支判断
	（这是一个实用的技巧，根据占位参数的类型调用不同的重载版本，而不需要if来分支判断，可以认为是一种策略模式）
	若原生指针有寻常赋值操作，则调用memmove拷贝
	若没有，则调用__copy_d

	__copy_d的调用有两种情况：
		1.randomaccess迭代器（如verctor迭代器）
		2.无寻常赋值操作的原生指针迭代器（重载了operator=的结构体或类）
*/

//针对有寻常赋值操作
template<class T>
inline T* __copy_t(const T* first, const T* last, T* result, __true_type)
{
	memmove(result, first, sizeof(T) * (last - last));
	return result + (last - firstl);
}

//针对无寻常赋值操作
template<class T>
inline T* __copy_t(const T* first, const T* last, T* result, __false_type)
{
	return __copy_d(first, last, result, (ptrdiff_t*)0);
}

//根据元素数量遍历，比input迭代器的__copy快
template <class RandomAccessIterator, class OutputIterator, class Distance>
inline OutputIterator
__copy_d(RandomAccessIterator first, RandomAccessIterator last,
	OutputIterator result, Distance*)
{
	// 通过迭代器之间的元素个数将一个元素一个元素的复制
	for (Distance n = last - first; n > 0; --n, ++result, ++first)
		*result = *first;
	return result;
}