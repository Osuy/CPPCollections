#pragma once
/*
	STL�ṩ��������Set��ص��㷨���ֱ��ǲ���union������intersection���difference���ԳƲsymmetric difference
	�����������㣬����ѧ������Ԫ���ظ������򣬵�STL�е�Set����Ҫ��Ԫ������multiSet����Ԫ���ظ�
	�ڱ��ڼ���ĽǶ��Ͽ��ǣ����������Ǿ޴�ı����ġ���ˣ��������㷨����Set/MultiSetΪ�������䡣
	������hashtableʵ�ֵ�hashSetԪ�ز���������ģ���Ȼ����ʵ�����������㷨�����Ǽ�Ϊ���㡣���STLû��ʵ�ֻ���hashtable����������

	������Ҫע�⣺
		��������Set����1����ô������������1��������ѧ��ֻ��һ��1��
			һ��Set������1����һ����һ��1����ô���һ��1������ѧ��û��1��
		������Ϊ�ظ���Ԫ��������ʱ��Ȼ�Ƕ�����Ԫ�أ�������Ϊ�ظ���һ������
*/

/*
	set_union
	����S1��S2�Ĳ�����������result��λ���ϡ�����result+S1.size+S2.size��Ҫ���������䶼������ġ�
	���һ��Ԫ�ص�S1�г���m�Σ���S2�г���n�Σ���ô�ڲ����л����max(m,n)��
*/

template <class InputIterator1,class InputIterator2,class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
						 InputIterator2 first2, InputIterator2 last2,
						 OutputIterator result
)
{
	while (first1 != last1 && first2 != last2)
	{	
		//���first1С�����first1���Լ�
		//���first2С�����first2���Լ�
		//���first1 == first2��ֻ���һ����Ȼ���������Լ�
		//���result�Լ�
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
	//�˳�ѭ����S1��S2������һ������ʣ�࣬ʹ��copy��ʣ��Ŀ�����result
	return copy(first2, last2, copy(first1, last1, result));
}


/*
	set_intersection
	����S1��S2�Ľ������ŵ�result����������ÿһ���ȴ�����S1Ҳ������S2��Ԫ�ء�
	���ĳ��Ԫ����S1�ظ�m�Σ���S2�ظ�n�Σ��򽻼��ظ�min(m,n)��
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
		else//ֻ��*first2 = *first1ʱ�������result��ȫ���Լ�
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
	��S1 - S2����S1ȥ��S2�ڳ��ֵ�Ԫ�غ��ʣ��Ԫ�ء���S2 - S1��ͬ
	���ĳ��Ԫ����S1����m�Σ�S2����n�Σ���ô�����max(m-n,0)��
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
	��S1��S2�ĶԳƲ����ֻ������S1��S2������ͬʱ�������������Ԫ��
	���ĳԪ����S1����m�Σ���S2����n�Σ���ô����������ڳ���abs(m-n)��
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





