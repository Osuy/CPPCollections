/*
	set���㷨���ٶ����������������β������
*/


/*
	set_union(first1,last1,first2,last2��result)
	���⻹�����е������������º���
	set_union������������Ĳ���,����result��ָ�����䣬������result��
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
	//������������������һ������β�˺����
	//���Ƕ���Ĳ�����copy�ķ�ʽ�ν���ȥ
	return copy(first2, last2, copy(first1, last1, result));
}

/*
	set_intersection��first1,last1,first2,last2��result��
	��result��ָ���ڴ湹����������Ľ���
	�����set_union��ȥ����*result=*first�Ĳ���
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
	��result��ָ���ڴ洦������������Ĳ
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
	//�������2��ǰ�������Ͱ�����1ʣ�µ�Ԫ�ؿ�����result��
	//�������2��ǰ�������������ʲô��û����
	return copy(first1, last1, result);
}

/*
	set_symmetric_difference
	������ȥ����������һ���������������Ԫ�أ����������������䶼�е�Ԫ��
*/