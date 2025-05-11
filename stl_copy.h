#pragma once
#include <string.h>
#include <iterator>
using namespace std;
/*
	����ӿڣ�
		1.����ģ��
		2.���char*������
		2.���wchar_t*������
	
	stl������ͨ��������������char*��wchar_t*�Ŀ���
	��ͨ�������رȺ���ģ�����غͺ���ģ��ȫ�ػ�Ҫ����
*/

//����ģ��copy
template<class InputIterator, class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
	//ʹ�÷�����__copy_dispatch�����ݵ������������ɷ�����ͬ�ĺ���
	//��Ϊ����ģ�壬��������һ����������ʱҪ��ģ�����Ͳ����б�
	return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
}

//char*������
inline char* copy(const char* first, const char* last, char* result)
{
	//ֱ����memmove����
	memmove(result, first, last - first);
	return result + (last - first);
}

//wchar_t*������
inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
	//ֱ����memmove����
	memmove(result, first, sizeof(wchar_t) * (last - first));
	return result + (last - first);
}


//----------------------------------------------------------------------------------------
/*
	__copy_dispatch
		�ɷ���������ģ�壬�Ƿº���
		__copy_dispatch��copy����·��������Ҫ��һ��
		�����ж���
		1.��ȡ������category�����Ĳ�������__copy�����ò�ͬ�����ذ汾
		2.��ģ��ƫ�ػ�������ԭ��ָ��

	ֵ��ע�����ƫ�ػ��汾�ڴ���ԭ��ָ��ʱ����ָ����֪����T�Ƿ����Ѱ����ֵ��������Ϊ���Ĳ���
	�����ǻ������ͣ�int, float, char������Ѱ����ֵ���������������memmove������
	�������������ͣ��ṹ����ࣩ��û��Ѱ����ֵ��������ֻ������operator=�������Ҫ�������䣬�������

*/

//ͨ����ģ��汾
template<class InputIterator, class OutputIterator>
struct __copy_dispatch
{
	OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
	{
		//��ȡ�������Ա���Ϊ���Ĳ�������__copy
		return __copy(firts, last, result, iterator_category(first));
	}
};

//���ԭ��ָ���ƫ�ػ�
template<class T>
struct __copy_dispatch<T*, T*>
{
	T* operator()(T* first, T* last, T* result)
	{
		typedef typename __type_traits<T>::has_trivial_assignmenet_operator t;
		return __copy_t(first, last, result, t());
	}
};

//���constԭ��ָ���ƫ�ػ�
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

	_copy��һ������ģ�壬�ж�����أ����ص�ǰ����δ�����Ͷ���һ���ģ���ͬ���ǵ��Ĳ���
	���ݵ��Ĳ����������Ա�Ĳ�ͬ�����ò�ͬ��__copy

	���ذ汾��
		input��������֧��++����֧��+n��-�����ֻ��ѭ�����++���ж�first!=last��
		random_access��֧��++��+n��-������
*/

//input_iterator�汾
template<class InputIterator, class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag)
{
	for (; first != last; ++result, ++first)
		*result = *first;
	return result;
}

//random_access_iterator�汾
template<class RandomAccessIterator, class OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag)
{
	return __copy_d(first, last, result, distance_type(first));
}



//----------------------------------------------------------------------------------------
/*
	__copy_d��__copy_t

	__copy_t��__copy_dispatch��ƫ�ػ����ã����ڴ���ԭ��ָ��
	__copy_t��������+ռλ���������з�֧�ж�
	������һ��ʵ�õļ��ɣ�����ռλ���������͵��ò�ͬ�����ذ汾��������Ҫif����֧�жϣ�������Ϊ��һ�ֲ���ģʽ��
	��ԭ��ָ����Ѱ����ֵ�����������memmove����
	��û�У������__copy_d

	__copy_d�ĵ��������������
		1.randomaccess����������verctor��������
		2.��Ѱ����ֵ������ԭ��ָ���������������operator=�Ľṹ����ࣩ
*/

//�����Ѱ����ֵ����
template<class T>
inline T* __copy_t(const T* first, const T* last, T* result, __true_type)
{
	memmove(result, first, sizeof(T) * (last - last));
	return result + (last - firstl);
}

//�����Ѱ����ֵ����
template<class T>
inline T* __copy_t(const T* first, const T* last, T* result, __false_type)
{
	return __copy_d(first, last, result, (ptrdiff_t*)0);
}

//����Ԫ��������������input��������__copy��
template <class RandomAccessIterator, class OutputIterator, class Distance>
inline OutputIterator
__copy_d(RandomAccessIterator first, RandomAccessIterator last,
	OutputIterator result, Distance*)
{
	// ͨ��������֮���Ԫ�ظ�����һ��Ԫ��һ��Ԫ�صĸ���
	for (Distance n = last - first; n > 0; --n, ++result, ++first)
		*result = *first;
	return result;
}