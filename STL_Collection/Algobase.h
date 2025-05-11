#pragma once


/*
	equal
	�ж���������Ԫ���Ƿ���ȣ��ڶ����������ϳ���������ֲ�����
	��˵ڶ��������ǰ��һ�������һ��������ȣ��᷵��true
*/

template <class InputInterator1, class InputIterator2>
inline bool equal(InputInterator1 first1, InputInterator1 last1, InputIterator2 first2, InputIterator2 last2)
{
	for (; first1 != last1; ++first1, ++first2)
		if (*first1 != *first2)
			return false;
	return true;
}

//�Զ���Ƚϰ汾
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
	��������ָ��Ԫ�����
	fill_n
	��Ŀ��λ�ú����(n-1)��λ����ָ��Ԫ�����
*/

template <class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last, const T& value)
{
	for (; first != last; ++first)
		*first = value;
}

//����ֻд������
template <class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value)
{
	for (; n > 0; --n, ++first)
		*first = value;
	return first;
}

/*
	find
	��������Ѱ����ָ��Ԫ����ͬ��Ԫ�أ������ҵ��ĵ�һ��Ԫ�صĵ�����
	find_if
	�����Զ�������Ѱ��Ԫ��
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
	�����㷨����[first,last)�����Ԫ�ؿ�����[result,result + (last-first))������
	copy�Ե�������Ҫ�󼸺�û�У�Ҫ��[first,last)����������Input��������[result,result + (last-first))������Output������
	��ˣ��������������κ�������Ŀ�����

	copy�ľ���ϸ����ʹ��memmove��ѭ��������Ԫ�ء�
	copy��Ȼ��һЩ��Ҫע��������
		1.��������������������ǲ��ص��ģ��ǵ�Ȼû������
		2.�������������������������ڣ������������串��ʱ�Ѿ���ȡ�����Ҳû������
		3.���ǣ����������������������ڣ���ô���������������������������ʱ��ԭ��Ӧ�ÿ�����ֵ�Ѿ���֮ǰ�Ŀ��������ˡ�
	���ϵĵ�3����Ҫ������������κ�ʹ��memmove�����������ʹ��ѭ����һ�������
	���磬
	�����3������¶�vector����������ȷ������deque���������
	��Ϊvector�ĵ�������ԭ��ָ��T*�������memmove����deque�ĵ������ǽṹ�壬��ʹ��ѭ��
*/

//����copy�����������Ψһ�ӿ�
template<class InputIterator,class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
	return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);//ֱ���ڲ���ת������ƫ�ػ�����·��
}

//���const char* ��ȫ�ػ���ֱ��ʹ��memmove
inline char* copy(const char* first, const char* last, char* result)
{
	memmove(result, first, last - first);//ֱ��ʹ��memmove�ƶ��ڴ�����
	return result + (last - first);//������������ĩβ
}

//���const wchar_t* ��ȫ�ػ���ֱ��ʹ��memmove
inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
	memmove(result, first, last - first);
	return result + (last - first);
}

/*
	������copy�ڲ���������__copy_dispatch���º�����
	__copy_dispatch��һ����ȫ�����������ƫ�ػ���

	�����棺����__copy������ԭ�ⲻ���ش���������������⣬����������������ص�����category
	��Ϊ__copy���ݲ����Ĳ�ͬ��������ذ汾�����������category��Ȼ����ú��ʵ����ذ汾�������Ч��

	ƫ�ػ��棺����__copy_t�������ԭ��ָ�루������������Ԫ��ָ�룩���Ż����ж�����trivial_assignment_operator��ƽ���ĸ�ֵ������
	Ҳ����ԭ���� = ���������������ص� = ����Ϊ���
*/
template <class InputIterator, class OutputIterator>
struct __copy_dispatch
{
	//()����������
	OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
	{
		return __copy(first, last, result, iterator_category(first));//����__copy�����������������category��Ŀ����·������
	}
};

//ƫ�ػ���1������__copy_t
//__copy_t
template <class T>
struct __copy_dispatch<T*, T*>
{
	T* operator()(T* first, T* last, T* result)
	{
		typedef typename __type_traits<T>::has_trivial_assignment_operator t;
		return __copy_t(first, last, result, t());//����__copy_t
	}
};

//ƫ�ػ���2������__copy_t���ཻ�汾1��Ψһ��������������ĵ�������const����
template <class T>
struct __copy_dispatch<const T*, T*>
{
	T* operator()(const T* first, const T* last, T* result)
	{
		typedef typename __type_traits<T>::has_trivial_assignment_operator t;
		return __copy_t(first, last, result, t());//����__copy_t
	}
};

//__copy��InputIterator�汾
template <class InputIterator, class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag)
{
	for (; first != last; ++result, ++first)//ѭ���������������Ƿ񲻵ȣ��ٶ���
		*result = *first;
	return result;
}

//__copy��RandomAccessIterator�汾���ڲ�����__copy_d�������װ��Ϊ����߸��ã����Ƿֳ�·��
template <class RandomAccessIterator, class OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag)
{
	return __copy_d(first, last, result, distance_type(first));
}

//__copy_d������__copy��RandomAccessIterator�汾��__copy_t��has_trivial_assignment_operator�汾
template <class RandomAccessIterator, class OutputIterator, class Distance>
inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*)
{
	for (Distance n = last - first; n > 0; --n, ++result, ++first)//ѭ��������n>0���ٶȿ�
		*result = *first;
	return result;
}

//__copy_t��ԭ��=�����汾
template <class T>
inline T* __copy_t(const T* first, const T* last, T* result, __true_type)
{
	memmove(result, first, sizeof(T) * (last - first));
	return result + (last - first);
}

//__copy_t������=�����汾
template <class T>
inline T* __copy_t(const T* first, const T* last, T* result, __false_type)
{
	return __copy_d(first, last, result, (ptrdiff_t*)0);
}


/*
	���Ͼ���copy��ȫ��ϸ�ڡ�
	������������ĵ��������ͣ�copy��ѡ����ú��ʵĺ�����ɿ�����
	���µ�copy·��������
	����char*������copyȫ�ػ���1��ʹ��memmove
	����wchar_t*������ȫ�ػ���2��ʹ��memmove
	������������������ת__copy_dispatch
		����ԭ��ָ�룺����__copy_dispatchƫ�ػ���1
			����ת��__copy_t���������has_trivial_assignment_operator
				����δ���� = ��ʹ��memmove
				���������� = ��ʹ��__copy_d
					����__copy_d��ʹ��n>0ѭ��
		����constԭ��ָ�룺����__copy_dispatchƫ�ػ���2
			����ת��__copy_t���������has_trivial_assignment_operator
				����δ���� = ��ʹ��memmove
				���������� = ��ʹ��__copy_d
		��������ָ��:����__copy_dispatch��ȫ�ػ���
			����ת��__copy���������iterator_category
				����input_iterator_tag��ʹ��first!=lastѭ��
				����random_access_iterator_tag��ʹ��__copy_d
					����__copy_d��ʹ��n>0ѭ��


	�κ�ʹ��memmove��copy����3������������
	�κ�ʹ��ѭ����copy����3����������
*/

