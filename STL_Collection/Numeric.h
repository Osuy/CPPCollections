#pragma once

/*
	accumulate
	累加算法，将区间的元素和Init全部相加
*/

template <class InputIterator ,class T>
T accumulate(InputIterator first, InputIterator last, T init)
{
	for (; first != last; ++first)
		init += *first;
	return init;
}

//自定义运算版本
template <class InputIterator, class T,class BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op)
{
	for (; first != last; ++first)
		init = binary_op(init, *first);//运用自定义运算
	return init;
}

/*
	adjacent_difference
	计算相邻元素的差,赋给目标地址
*/

template <class InputIterator,class OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result)
{
	if (first == last) return result;
	*result = *first;
	
	iterator_traits<InputIterator>::value_type value = *first;
	while (++first != last)
	{
		*++result = *first - value;
		value = *first;
	}
}

//自定义运算版本
template <class InputIterator, class OutputIterator,class Binary_Operation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, Binary_Operation binary_op)
{
	if (first == last) return result;
	*result = *first;
	iterator_traits<InputIterator>::value_type value = *first;
	while (++first != last)
	{
		*++result = binary_op(*first, value);
		value = *first;
	}
}

/*
	inner_product
	内积计算：(a,b,c)*(i,j,k) = a*i + b*j + c*k	
	将区间1和起点2的元素进行内积，初始值init = 0
*/

template <class InputIterator1,class InputIterator2,class T>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init)
{
	for (; first1 != last1; ++first1, ++first2)
		init = init + (*first1 * *first2);
	return init;
}

//自定义运算版本
template <class InputIterator, class InputIterator2, class T, class Binary_Operation1, class Binary_Operation2>
T inner_product(InputIterator first1, InputIterator last1, 
	InputIterator2 first2, T init, 
	Binary_Operation1 binary_op1,
	Binary_Operation2 binary_op2
)
{
	for (; first1 != last1; ++first1, ++first2)
		init = binary_op1(init, binary_op2(*first1, *first2));
	return init;
}

/*
	partial_sum
	部分和，将[*first + *(first + 1) + ... + *(first + i)] 赋值给 *result
*/

template <class InputIterator, class OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result)
{
	if (first == last) return result;
	*result = *first;
	iterator_traits<InputIterator>::value_type value = *first;
	while (++first != last)
	{
		value += *first;
		*++result = value;
	}
}

//自定义运算版本
template <class InputIterator, class OutputIterator, class Binary_Operation>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result, Binary_Operation binary_op)
{
	if (first == last) return result;
	*result = *first;
	iterator_traits<InputIterator>::value_type value = *first;
	while (++first != last)
	{
		value = binary_op(value, *first);
		*++result = value;
	}
}







