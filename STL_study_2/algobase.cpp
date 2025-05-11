/*
	equal(first1,last1,first2)
	equal(first1,last1,first2,op)
	�Ƚ������ȳ������� Ԫ���Ƿ���ȣ���ȷ���true
	�ڶ�������ֻ����һ��first2�����������Կ��ܻ��������2������1��
	�������ڵ���equalǰ�ж����������Ƿ����
*/

#include <iterator>
using namespace std;
template<class  InputIterator1 ,class InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
	for (; first1 != last1; ++first1, ++first2)
		if (*first1 != *first2)//�����������ú�Ƚϣ���˲�Ҫ�����������һ�£��ؼ�����������ζ�*������
			return false;
	return true;
}

template<class  InputIterator1, class InputIterator2,class BinaryPredicate>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,BinaryPredicate binary_pred) {
	for (; first1 != last1; ++first1, ++first2)
		if (!binary_pred(*first1,*first2))//�÷º������ж�
			return false;
	return true;
}

/*
	fill(first,last,value)
	fill_n(first,n,value)
	��������value��䣬���޸�ԭ����ֵ
	���ֻȷ��Ҫ�����ٸ�������ȥ����last��������������fill_n�����n��Ԫ��
	fill_n�������������һ��Ԫ�ص���һ��λ��
*/

template<class ForwardIterator ,class T>
void fill(ForwardIterator first, ForwardIterator last, const T& value) {
	for (; first != last; ++first)//ѭ������������value���
		*first = value;
}


template<class OutputdIterator, class Size,class T>
OutputdIterator fill_n(OutputdIterator first,Size n , const T& value) {
	for (;n>0;--n, ++first)//ѭ������������value���
		*first = value;
	return first;//�����������һ��Ԫ�ص���һ��λ��
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

//����ͨ�������µ�traits������ȡ��ForwardIterator1�����ͣ������䶨����tmp����
//��ȻҲ������ôд
template<class ForwardIterator1, class ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b) {
	typename iterator_traits<ForwardIterator1>::value_type tmp = *a;
	*a = *b;
	*b = tmp;
}

/*
	lexicographical_compare(first1,last1,first2,last2)
	�Ƚ����������Ԫ���Ƿ����
	��֮ǰֻ��Ҫ�ڶ��������first��ͬ�����䳤��Ҳ�����˷��ؽ������������������������
	�Ե�һ����ƥ���Ԫ�أ����first1С������true�����򷵻�false
	���ȫ����ƥ�䣬���������䳤�Ȳ�ͬ������ȵִ�last1������true���ȵ���last2������false
	����������䳤����ͬ������false
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

//ԭ��ָ���,��unsigned char���Ƚ��ڴ��е�ÿһ��bytes
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
	�Ƚ�a��b�Ĵ�С�����ؽϴ��
*/

template <class T>
inline const T& max(const T& a, const T& b) {
	return a < b ? b : a;//����������Ҫ����<������
}

template<class T ,class Compare>
inline const T& max(const T& a, const T& b, Compare comp) {
	return comp(a, b) ? b : a;
}

/*
	min(a,b)
	����a��b�н�С��
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
	��һƥ�����������Ԫ���Ƿ���ȣ��ҵ���һ����ƥ���
	����������������ɵ�pair���ֱ�ָ����������Ĳ�ƥ���
*/

template <class InputIterator1,class InputIterator2>
pair< InputIterator1, InputIterator2> //����ֵ
mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
	while (first1 != last1 && *first == *first2) 
		++first1, ++first2;
	return pair< InputIterator1, InputIterator2>(first1, first2);
}

template <class InputIterator1, class InputIterator2,class BinaryPredicate>
pair< InputIterator1, InputIterator2> //����ֵ
mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,BinaryPredicate binary_pred) {
	while (first1 != last1 && binary_pred(*first, *first2))
		++first1, ++first2;
	return pair< InputIterator1, InputIterator2>(first1, first2);
}

/*
	swap
	�����������������
*/

template <class T>
inline void swap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}

/*
	copy(first��last��result)
	������[first��last)��Ԫ�ظ��Ƶ�[result,result+(last-first))����
	��ӵ��㷨����Բ�ͬ�����������Ż�
*/

template<class InputIterator,class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
	return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
}

//���ԭ��ָ�������
inline char* copy(const char* first, const char* last, char* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}

inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
	memmove(result, first, sizeof(wchar_t)*(last - first));
	return result + (last - first);
}

//__copy_dispatch
//��ȫ�����棬����()����__copy
template <class InputIterator ,class OutputIterator>
struct __copy_dispatch {
	OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
		return __copy(first, last, result, iterator_category(first));
	}
};

//ƫ�ػ��汾������ģ�������ָ��ΪT*
template<class T>
struct __copy_dispatch<T*, T*> {
	T* operator()(T* first, T* last, T* result) {
		typedef typename __type_traits<T>::has_trivial_assignment_operator t;
		return __copy_t(first, last, result, t());
	}
};

//ƫ�ػ��汾��ģ�����ָ��Ϊconst T* �� T*
template <class T>
struct __copy_dispatch<const T*,T*>{
	T* operator()(const T* first, const T* last, T* result) {
		typedef typename __type_traits<T>::has_trivial_assignment_operator t;
		return __copy_t(first, last, result, t());
	}
};

//���ϣ�����copy����ʱ������ԭ��ָ�룬ʹ�ö�Ӧ��ԭ���汾
//�����������ͣ��ҷ�������Ҳһ�£�ʹ��ƫ�ػ��汾����ת__copy_t
//�����������ͣ��������Ͳ�һ�£�������ȫ�����汾����ת__copy

//__copy��Բ�ͬ���͵ĵ��������˲�ͬ������
//InputIterator�汾
template<class InputIterator,class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
	for (; first != last; ++result, ++first)//mÿ��ѭ��Ҫִ��first != last���ٶ���
		*result = *first;
	return result;
}

//RandomAccessIterator�汾
template<class RandomAccessIterator, class OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, 
							 OutputIterator result, random_access_iterator_tag) {
	return __copy_d(first, last, result, distance_type(first));
}

template<class RandomAccessIterator,class OutputIterator ,class Distance>
inline OutputIterator 
__copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*) {
	for (Distance n = last - first; n > 0; --n, ++result, ++first)//ʹ��n����ѭ�����ٶȿ�
		*result = *first;
	return result;
}
