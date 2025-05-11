/*
	adjacent_find(first,last)
	�ҳ���һ����������������Ԫ��
*/

//�������
template <class ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
	if (first == last)return last;
	ForwardIterator next = first;
	while (++next != last) {
		if (*first == *next)return first;
		first = next;
	}
	return last;
}

//�Զ�������
template <class ForwardIterator ,class BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last,BinaryPredicate binary_pred) {
	if (first == last)return last;
	ForwardIterator next = first;
	while (++next != last) {
		if (binary_pred(*first ,*next)) return first;
		first = next;
	}
	return last;
}

/*
	count(first,last value)
	����������value��ȵ�Ԫ�صĸ�������
*/

template<class InputIterator, class T>
typename iterator_traits<InputIterator>::difference_type//��һ�����Ƿ�������,������traits����
count(InputIterator first, InputIterator last, const T& value) {
	iterator_traits<InputIterator>::difference_type n = 0;
	for (; first != last; ++first)
		if (*first == value)
			++n;
	return n;
}

/*
	count_if(first,last,pred)
	������������������Ԫ�ؽ��м�������������
*/

template<class InputIterator, class Predicate>
typename iterator_traits<InputIterator>::difference_type//��һ�����Ƿ�������,������traits����
count(InputIterator first, InputIterator last, Predicate pred) {
	iterator_traits<InputIterator>::difference_type n = 0;
	for (; first != last; ++first)
		if (pred(*first))//ֻ����һ����count��ͬ
			++n;
	return n;
}

/*
	find(first ,last value)
	Ѱ����������value��ȵ�Ԫ�أ����ص�һ���ҵ���Ԫ�صĵ�����
*/

template <class InputIterator,class T>
InputIterator find(InputIterator first, InputIterator last, const T& value) {
	while (first != last && *first != value)++first;
}

/*
	find_if(first,last ,pred)
*/

template <class InputIterator, class Predicate>
InputIterator find(InputIterator first, InputIterator last, Predicate pred) {
	while (first != last && !pred(*first))++first;
}

/*
	find_end(first1,last1,first2,last2)
	�ҳ����һ�����ֵ�������
	������1��Ѱ������2�����������һ�γ��ֵ�λ�õĵ�����
*/

template<class ForwardIterator1,class ForwardIterator2>
inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
	ForwardIterator2 first2, ForwardIterator2 last2) {//��Ϊʹ����traits����������ת�����ں�������
	typedef typename iterator_traits<ForwardIterator1>::iterator_category category1;

	typedef typename iterator_traits<ForwardIterator2>::iterator_category category2;

	return __find_end(first1, last1, first2, last2, category1(), category2());
}

//�������
template<class ForwardIterator1,class ForwardIterator2>
ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1,
							ForwardIterator2 first2, ForwardIterator2 last2,
	forward_iterator_tag,forward_iterator_tag) {
	if (first2 == last2)//�������Ŀ���ǿգ���������1��ĩβ
		return last1;
	else {
		ForwardIterator1 result = last1;
		while (1) {//�Լ��ж��Ƿ������һ����Ѱ��Ŀ��ת�����Ӻ���ʵ��
			ForwardIterator1 new_result = search(first1, last1, first2, last2);//Ѱ�ҵ�һ��Ŀ�꣬����¼��ַ
			if (new_result == last1)//����Ѿ���λ������1 ��ĩβ�����Է���
				return result;
			else {//���ҵ��ĵ�ַ����result����first1�Ķ����Խ�����һ��ѭ����
				result = new_result;
				first1 = new_result;
				++first1;
			}
		}
	}
}

//�������
template <class BidirectionalIterator1,class BidirectionalIterator2>
BidirectionalIterator1 __find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
								  BidirectionalIterator2 first2, BidirectionalIterator2 last2,
								  bidirectional_iterator_tag, bidirectional_iterator_tag) {
	typedef reverse_iterator<BidirectionalIterator1> reviter1;
	typedef reverse_iterator<BidirectionalIterator2> reviter2;

	//������������
	reviter1 rlast1(first1);
	reviter2 rlast2(first2);

	reviter1 rresult = search(reviter1(last1), rlast1, reciter2(last2), rlast2);

	if (rresult == rlast1)
		return last1;
	else {
		BidirectionalIterator1 result = rresult.base();
		advance(result, -distance(first2, last2));
		return result;
	}
}


/*
	find_first_of(first1,last1,first2,last2)
	Ѱ������2��һԪ��������1�״γ��ֵĵ�ַ
	��helloworld��aeiou����һ�����ֵ���������2��Ԫ����'e'������ָ�����ĵ�����
*/

template <class InputIterator,class ForwardIterator>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
							ForwardIterator first2, ForwardIterator last2) {
	for (; first1 != last1; ++first1)
		for (ForwardIterator iter = first2; iter != last2; ++iter)//mÿһ�ζ�Ҫɨ������2�е�����Ԫ�أ������õ���������
			if (*first1 == *iter)
				return first1;
	return last1;
}

template <class InputIterator ,class ForwardIterator,class BinaryRredicate>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
							ForwardIterator first2, ForwardIterator last2,
							BinaryRredicate comp) {
	for (; firt1 != last1; ++first1)
		for (ForwardIterator iter = first2; iter != last2; ++iter)
			if (comp(*first1, *iter))
				return first1;
	return last1;
}

/*
	for_each(first,last,func)
	��funcȥ��������
*/

template <class IputIterator,class Function>
Function for_each(IputIterator first, IputIterator last, Function f) {
	for (; first != last; ++first)
		f(*first);
	return f;
}

/*
	generate(first,last,gen)
	���º���gen�Ľ����д��������
*/

template <class ForwardIterator,class Generator>
void generate(ForwardIterator first, ForwardIterator last, Generator gen) {
	for (; first1 = last; ++first)
		*first = gen();
}

//����Ѿ�ֱ������Ҫ�޸Ķ��ٸ�Ԫ�أ�ʹ��generate_n����Ч
template <class OutputIterator ,class Size ,class Generator>
OutputIterator generate_n(OutputIterator first, Size n, Generator gen) {
	for (; n > 0; --n, ++firsts)
		*first = gen();
	return first;
}

/*
	includes(first1,last1,first2,last2)
	�ж�����2�Ƿ����������1�У�������2������Ԫ�ض�������1�г��ֹ�
	������������������
*/

template <class InputIterator1,class InputIterator2>
bool includes(InputIterator1 first1, InputIterator1 last1,
			  InputIterator2 first2, InputIterator2 last2) {
	while (first1 != last1 && first2 != last2)
		if (*first2 < *first1)//������1��СԪ�ػ�С��˵����inlcludes
			return false;
		else if (*first1 < *first2)//��ǰԪ��1С��Ԫ��2����ǰԪ��1++
			++first1;
		else                        //����Ԫ��1==Ԫ��2��
			++first1, ++first2;
	return first2 == last2;//ѭ��������������һ�������ߵ�ĩβ�����������2�ߵ�ĩβ��˵��includes������true
}

template <class InputIterator1 ,class InputIterator2,class Compare>
bool includes(InputIterator1 first1, InputIterator1 last1,
			  InputIterator2 first2, InputIterator2 last2,
			  Compare comp) {
	while (first1 != last1 && first2 != last2)
		if (comp(*first2 , *first1))//������1��СԪ�ػ�С��˵����inlcludes
			return false;
		else if (comp(*first1 , *first2))//��ǰԪ��1С��Ԫ��2����ǰԪ��1++
			++first1;
		else                        //����Ԫ��1==Ԫ��2��
			++first1, ++first2;
	return first2 == last2;
}

/*
	max_element(first,last)
*/

template <class ForwardIterator>
ForwardIterator max_elememt(ForwardIterator first, ForwardIterator last) {
	if (first == last)return first;
	ForwardIterator result = first;
	while (++first != last)
		if (*result < *first)result = first;
	return result;
}

template <class ForwardIterator ,class Compare>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp) {
	if (first == last)return first;
	ForwardIterator result = first;
	while (++first != last)
		if (comp(*result, *first))result = first;
	return result;
}

/*
	merge(first1,last1,first2,last2,result)
	��������������ݰ���˳��ϲ�������result��ָ������
*/

template <class InputIterator1,class InputIterator2,class OutputIterator>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
					 InputIterator2 firsts2, InputIterator2 last2,
					 OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first2 < *first1) {
			*result = *first2;
			++first2;
		}
		else {
			*result = first1;
			++first1;
		}
		++result;
	}
	return copy(first12, last2, copy(first1, last1, result));
}

template <class InputIterator1, class InputIterator2, class OutputIterator,class Compare>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 firsts2, InputIterator2 last2,
	OutputIterator result,Compare comp) {
	while (first1 != last1 && first2 != last2) {
		if (comp(*first2 , *first1)) {
			*result = *first2;
			++first2;
		}
		else {
			*result = first1;
			++first1;
		}
		++result;
	}
	return copy(first12, last2, copy(first1, last1, result));
}


/*
	min_element
	������СԪ�صĵ�����
*/

template<class ForwardIterator>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last) {
	if (first == last)return first;
	ForwardIterator result = first;
	while (++first != last)
		if (*first < *result)result = first;
	return result;
}

template<class ForwardIterator,class Compare>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last,Compare comp) {
	if (first == last)return first;
	ForwardIterator result = first;
	while (++first != last)
		if (comp(*first , *result))result = first;
	return result;
}

/*
	partition(first,last,pred)
	����������pred��Ԫ�ط�ǰ�棬������ķź���
*/

template<class BidirectionlaIterator,class Predicate>
BidirectionlaIterator partion(BidirectionlaIterator first, BidirectionlaIterator last, Predicate pred) {
	while (true) {
		while (true)//�����ҵ���һ������Ҫ���first
			if (first == last)return first;
			else if (pred(*first))++first;
			else break;
		--last;//last��ǰ������ǵ�һ��ѭ����ָ�����һ��Ԫ�أ�����ָ������Ѿ�swap���ǰһ��Ԫ��
		while (true)//�Ӻ���ǰ�ҵ���һ������Ҫ���last
			if (first == last)return first;
			else if (!pred(*last))--last;
			else break;
		iter_swap(first, last);//����first��last
		++first;//����first������һ��ѭ��
	}
}

/*
	remove(first,last value)
	�Ƴ�����ɾ�������ǽ���Щ��value��ȵ�Ԫ�صĵ�ַ������������ȵ�Ԫ��
*/

template<class ForwardIterator ,class T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value) {
	first = find(first, last, value);//�ҵ���һ����value��ȵ�Ԫ��
	ForwardIterator next = first;
	return first == last ? first:remove_copy(++next, last, first, value);
}

//����һ������������value����ȵ�Ԫ�ظ��������������������������Ԫ�ص���һλ��
template <class InputIterator ,class OutputIterator,class T>
OutputIterator remove_copy(InputIterator first, InputIterator last, OutputIterator result, const T& value) {
	for (; first != last;++first)
		if (*first != value) {
			*result = *first;
			++result;
		}
}

template <class ForwardIterator ,class Predicate>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, Predicate pred) {
	first = find_if(first, last, pred);
	ForwardIterator next = first;
	return first == last ? first : remove_copy_if(++next, last, first, pred);
}


/*
	replace(first,last,old_value,new_value)
	�������е����о�ֵ����ֵ����
*/

template <class ForwardIterator,class T>
void replace(ForwardIterator first, ForwardIterator last, const T& old_value, const T& new_value) {
	for (; first != last; ++first)
		if (*first == old_value)*first = new_value;
}

//�����������Ԫ�ظ����µ����������У���old_value��ȵ�Ԫ����new_value���
template <class InputIterator,class OutputIterator ,class T>
OutputIterator replace_copy(InputIterator first, InputIterator last,
	OutputIterator result, const T& old_value, const T& new_value) {
	for (; first != last; ++first, ++result)
		*result = *first == old_value ? : new_value : *first;
}

/*
	reverse(first,last)
	�ߵ�����������Ԫ��
*/

template<class BidirectionalIterator>
inline void reverse(BidirectionalIterator first, BidirectionalIterator last) {
	__reverse(first, last, iterator_category(first));
}

template <class BidirectionalIterator>
void __reverse(BidirectionalIterator first, BidirectionalIterator last, bidirectional_iterator_tag) {
	while (true)
		if (first == last | first == --last)
			return;
		else
			iter_swap(first++, last);
}