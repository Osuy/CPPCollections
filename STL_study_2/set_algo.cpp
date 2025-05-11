/*
	set的算法至少都包含两个区间的首尾迭代器
*/


/*
	set_union(first1,last1,first2,last2，result)
	此外还可以有第六个参数，仿函数
	set_union构造两个区间的并集,赋给result所指的区间，并返回result。
*/

template <class InputInterator1,class InputInterator2,class OutputIterator>
OutputIterator set_union(InputInterator1 first1, InputInterator1 last1,
					 	 InputInterator2 first2, InputInterator2 last2,
						 OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
		}
		else if (*first1 > * first2) {
			*result = *first2;
			++first2;
		}
		else {
			*result = *first1;
			++first1;
			++first2;
		}
		++result;
	}
	//以上在两个区间任意一个到达尾端后结束
	//于是多出的部分用copy的方式衔接上去
	return copy(first2, last2, copy(first1, last1, result));
}

/*
	set_intersection（first1,last1,first2,last2，result）
	向result所指的内存构造两个区间的交集
	相比与set_union，去掉了*result=*first的操作
*/

template <class InputIterator1 ,class InputIterator2,class OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
								InputIterator2 first2, InputIterator2 last2,
								OutputIterator result) {
	while (first1 != last1 && first2 != last2)
		if (&first1 < *first2)
			++first1;
		else if (*first2 < *first1)
			++first2;
		else {
			*result = *first1;
			++first1;
			++first2;
			++result;
		}
	return result;
}

/*
	set_difference
	向result所指的内存处构造两个区间的差集
*/

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_defference(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2,
	OutputIterator result) {
	while (first != last1 && first2 != last2)
		if (&first1 < *first2) {
			*result = *first1;
			++first1;
			++result;
		}

		else if (*first2 < *first1)
			++first2;
		else {
			*result = *first1;
			++first1;
			++first2;
		}
	//如果区间2提前结束，就把区间1剩下的元素拷贝给result，
	//如果区间2提前结束，下面操作什么都没发生
	return copy(first1, last1, result);
}

/*
	set_symmetric_difference
	并集减去交集。构造一个包含两个区间的元素，但不包含两个区间都有的元素
*/