/*
	adjacent_find(first,last)
	找出第一组满足条件的相邻元素
*/

//相等条件
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

//自定义条件
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
	对区间内与value相等的元素的个数计数
*/

template<class InputIterator, class T>
typename iterator_traits<InputIterator>::difference_type//这一整行是返回类型,运用了traits技法
count(InputIterator first, InputIterator last, const T& value) {
	iterator_traits<InputIterator>::difference_type n = 0;
	for (; first != last; ++first)
		if (*first == value)
			++n;
	return n;
}

/*
	count_if(first,last,pred)
	对区间内满足条件的元素进行计数，返回数量
*/

template<class InputIterator, class Predicate>
typename iterator_traits<InputIterator>::difference_type//这一整行是返回类型,运用了traits技法
count(InputIterator first, InputIterator last, Predicate pred) {
	iterator_traits<InputIterator>::difference_type n = 0;
	for (; first != last; ++first)
		if (pred(*first))//只有这一步和count不同
			++n;
	return n;
}

/*
	find(first ,last value)
	寻找区间内与value相等的元素，返回第一个找到的元素的迭代器
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
	找出最后一个出现的子序列
	在区间1中寻找序列2，并返回最后一次出现的位置的迭代器
*/

template<class ForwardIterator1,class ForwardIterator2>
inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
	ForwardIterator2 first2, ForwardIterator2 last2) {//因为使用了traits技法，所以转交给内函数处理
	typedef typename iterator_traits<ForwardIterator1>::iterator_category category1;

	typedef typename iterator_traits<ForwardIterator2>::iterator_category category2;

	return __find_end(first1, last1, first2, last2, category1(), category2());
}

//正向查找
template<class ForwardIterator1,class ForwardIterator2>
ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1,
							ForwardIterator2 first2, ForwardIterator2 last2,
	forward_iterator_tag,forward_iterator_tag) {
	if (first2 == last2)//如果查找目标是空，返回区间1的末尾
		return last1;
	else {
		ForwardIterator1 result = last1;
		while (1) {//自己判断是否是最后一个，寻找目标转交给子函数实现
			ForwardIterator1 new_result = search(first1, last1, first2, last2);//寻找到一个目标，并记录地址
			if (new_result == last1)//如果已经定位到区间1 的末尾，可以返回
				return result;
			else {//将找到的地址交给result，把first1改动，以进行下一次循环，
				result = new_result;
				first1 = new_result;
				++first1;
			}
		}
	}
}

//逆向查找
template <class BidirectionalIterator1,class BidirectionalIterator2>
BidirectionalIterator1 __find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
								  BidirectionalIterator2 first2, BidirectionalIterator2 last2,
								  bidirectional_iterator_tag, bidirectional_iterator_tag) {
	typedef reverse_iterator<BidirectionalIterator1> reviter1;
	typedef reverse_iterator<BidirectionalIterator2> reviter2;

	//定义两个变量
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
	寻找区间2任一元素在区间1首次出现的地址
	如helloworld和aeiou，第一个出现的来自区间2的元素是'e'，返回指向它的迭代器
*/

template <class InputIterator,class ForwardIterator>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
							ForwardIterator first2, ForwardIterator last2) {
	for (; first1 != last1; ++first1)
		for (ForwardIterator iter = first2; iter != last2; ++iter)//m每一次都要扫描区间2中的所有元素，所以用迭代器遍历
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
	用func去遍历区间
*/

template <class IputIterator,class Function>
Function for_each(IputIterator first, IputIterator last, Function f) {
	for (; first != last; ++first)
		f(*first);
	return f;
}

/*
	generate(first,last,gen)
	将仿函数gen的结果填写在区间上
*/

template <class ForwardIterator,class Generator>
void generate(ForwardIterator first, ForwardIterator last, Generator gen) {
	for (; first1 = last; ++first)
		*first = gen();
}

//如果已经直到到底要修改多少个元素，使用generate_n更高效
template <class OutputIterator ,class Size ,class Generator>
OutputIterator generate_n(OutputIterator first, Size n, Generator gen) {
	for (; n > 0; --n, ++firsts)
		*first = gen();
	return first;
}

/*
	includes(first1,last1,first2,last2)
	判断区间2是否包含于区间1中，即区间2的所有元素都在区间1中出现过
	两个区间必须是有序的
*/

template <class InputIterator1,class InputIterator2>
bool includes(InputIterator1 first1, InputIterator1 last1,
			  InputIterator2 first2, InputIterator2 last2) {
	while (first1 != last1 && first2 != last2)
		if (*first2 < *first1)//比区间1最小元素还小，说明不inlcludes
			return false;
		else if (*first1 < *first2)//当前元素1小于元素2，当前元素1++
			++first1;
		else                        //此项元素1==元素2，
			++first1, ++first2;
	return first2 == last2;//循环结束后，至少有一个区间走到末尾，如果是区间2走到末尾，说明includes，返回true
}

template <class InputIterator1 ,class InputIterator2,class Compare>
bool includes(InputIterator1 first1, InputIterator1 last1,
			  InputIterator2 first2, InputIterator2 last2,
			  Compare comp) {
	while (first1 != last1 && first2 != last2)
		if (comp(*first2 , *first1))//比区间1最小元素还小，说明不inlcludes
			return false;
		else if (comp(*first1 , *first2))//当前元素1小于元素2，当前元素1++
			++first1;
		else                        //此项元素1==元素2，
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
	将两个区间的内容按照顺序合并，置于result所指的容器
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
	返回最小元素的迭代器
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
	将区间满足pred的元素放前面，不满足的放后面
*/

template<class BidirectionlaIterator,class Predicate>
BidirectionlaIterator partion(BidirectionlaIterator first, BidirectionlaIterator last, Predicate pred) {
	while (true) {
		while (true)//正序找到第一个不合要求的first
			if (first == last)return first;
			else if (pred(*first))++first;
			else break;
		--last;//last向前，如果是第一次循环，指向最后一个元素，否则，指向的是已经swap后的前一个元素
		while (true)//从后向前找到第一个符合要求的last
			if (first == last)return first;
			else if (!pred(*last))--last;
			else break;
		iter_swap(first, last);//交换first和last
		++first;//调整first满足下一次循环
	}
}

/*
	remove(first,last value)
	移除并非删除，而是将那些与value相等的元素的地址存入其他不相等的元素
*/

template<class ForwardIterator ,class T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value) {
	first = find(first, last, value);//找到第一个与value相等的元素
	ForwardIterator next = first;
	return first == last ? first:remove_copy(++next, last, first, value);
}

//遍历一个容器，把与value不相等的元素赋给新容器，返回新容器的最后元素的下一位置
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
	将区间中的所有旧值用新值代替
*/

template <class ForwardIterator,class T>
void replace(ForwardIterator first, ForwardIterator last, const T& old_value, const T& new_value) {
	for (; first != last; ++first)
		if (*first == old_value)*first = new_value;
}

//将区间的所有元素赋给新的容器，其中，与old_value相等的元素用new_value替代
template <class InputIterator,class OutputIterator ,class T>
OutputIterator replace_copy(InputIterator first, InputIterator last,
	OutputIterator result, const T& old_value, const T& new_value) {
	for (; first != last; ++first, ++result)
		*result = *first == old_value ? : new_value : *first;
}

/*
	reverse(first,last)
	颠倒容器中所有元素
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