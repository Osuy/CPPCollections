#pragma once
/*
	插入排序
	从未排序区间里取第一个数，
	插入到已排序区间里
	循环len-1次
*/

void InsertionSort1(int* arr, int len)
{
	int preIndex, current;
	for (int i = 1; i < len; ++i)
	{
		preIndex = i - 1;
		current = arr[i];
		while (preIndex >= 0 && arr[preIndex] >= current)
		{
			arr[preIndex + 1] = arr[preIndex];
			--preIndex;
		}
		arr[preIndex + 1] = current;
	}
}

template <class I>
void InsertionSort(I begin, I end)
{
	I pre;
	typename I::value_type current;
	for (size_t i = 1; i < (size_t)(end - begin); ++i)
	{
		pre = begin + i;
		current = *pre;
		while (pre > begin && *(pre - 1) > current)
		{
			*pre = *(pre - 1);
			 --pre;
		}
		*pre = current;
	}
}