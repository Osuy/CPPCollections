#pragma once
/*
	STL提供了四种与Set相关的算法，分别是并集union、交集intersection、差集difference、对称差集symmetric difference
	以上四种运算，在数学上允许元素重复和无序，但STL中的Set至少要求元素有序，multiSet允许元素重复
	在便于计算的角度上考虑，有序无疑是巨大的便利的。因此，这四种算法接受Set/MultiSet为输入区间。
	而基于hashtable实现的hashSet元素并不是有序的，虽然可以实现以上四种算法，但是极为不便。因此STL没有实现基于hashtable的四种运算

	另外需要注意：
		例如两个Set都有1，那么并集就有两个1，但是数学上只有一个1；
			一个Set有两个1，另一个有一个1，那么差集有一个1，但数学上没有1。
		这是因为重复的元素在运算时仍然是独立的元素，不会因为重复而一并并差
*/

/*
	set_union
	构造S1和S2的并集，并放在result的位置上。返回result+S1.size+S2.size。要求两个区间都是有序的。
	如果一个元素的S1中出现m次，在S2中出现n次，那么在并集中会出现max(m,n)次
*/

template <class InputIterator1,class InputIterator2,class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
						 InputIterator2 first2, InputIterator2 last2,
						 OutputIterator result
)
{
	while (first1 != last1 && first2 != last2)
	{	
		//如果first1小，输出first1再自加
		//如果first2小，输出first2再自加
		//如果first1 == first2，只输出一个，然后两个都自加
		//最后result自加
		if (*first1 < *first2)
		{
			*result = *first1;
			++first1;
		}
		else if (*first2 < *first1)
		{
			*result = *first2;
			++first2;
		}
		else
		{
			*result = *first1;
			++first1;
			++first2;
		}
		++result;
	}
	//退出循环后，S1和S2至多有一个还有剩余，使用copy将剩余的拷贝给result
	return copy(first2, last2, copy(first1, last1, result));
}


/*
	set_intersection
	构造S1和S2的交集，放到result。交集包含每一个既存在于S1也存在于S2的元素。
	如果某个元素在S1重复m次，在S2重复n次，则交集重复min(m,n)次
*/

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2,
	OutputIterator result
)
{
	while (first1 != last1 && first2 != last2)
	{
		if (*first1 < *first2)
		{
			++first1;
		}
		else if (*first2 < *first1)
		{
			++first2;
		}
		else//只在*first2 = *first1时，输出给result并全部自加
		{
			*result = *first1;
			++first1;
			++first2;
			++result;
		}

	}
	return result;
}


/*
	set_difference
	求S1 - S2，即S1去除S2内出现的元素后的剩余元素。与S2 - S1不同
	如果某个元素在S1出现m次，S2出现n次，那么差集出现max(m-n,0)次
*/

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
							  InputIterator2 first2, InputIterator2 last2,
							  OutputIterator result
)
{
	while (first1 != last1 && first2 != last2)
	{
		if (*first1 < *first2)
		{
			*result = *first1;
			++first1;
			++result;
		}
		else if (*first2 < *first1)
		{
			++first2;
		}
		else
		{
			++first2;
			++result;
		}
	}
	return copy(first1, last1, result);
}


/*
	set_symmetric_difference
	求S1和S2的对称差集，即只出现在S1或S2，而不同时存在两个区间的元素
	如果某元素在S1出现m次，在S2出现n次，那么在输出区间内出现abs(m-n)次
*/

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
										InputIterator2 first2, InputIterator2 last2,
										OutputIterator result
)
{
	while (first1 != last1 && first2 != last2)
	{
		if (*first1 < *first2)
		{
			*result = *first1;
			++first1;
			++result;
		}
		else if (*first2 < *first1)
		{
			*result = *first2;
			++first2;
			++result;
		}
		else
		{
			++first2;
			++result;
		}
	}
	return copy(first2, last2, copy(first1, last1, result));
}





