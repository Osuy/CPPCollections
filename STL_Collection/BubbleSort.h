#pragma once

/*
	冒泡排序
	外循环从0至len-2。
	每一次完成外循环，就会从未排序区间找到最大的数
	放在未排序区间的最后面，然后区间向前缩小1

	内循环从0至len-i-1。
	两两比较，前者较大就互换。从而实现最大数换到区间最后面
*/	

void BubbleSort(int* arr, int len)
{
	for (int i = 0; i < len - 1; ++i)//外循环指向len-1次
	{
		for (int j = 0; j < len - 1 - i; ++j)//内循环从0至len-1-i
		{
			if (arr[j] > arr[j + 1])//每当后一个元素比当前元素大，就互换
			{
				int temp = arr[j + 1];
				arr[j + 1]= arr[j];
				arr[j] = temp;
			}
		}
	}
}

void BubbleSort2(int* arr, int len)
{
	for (int i = 0; i < len - 1; i++)
	{
		for (int j = 0; i < len -1 - i; ++j)
		{
			if (arr[j] > arr[j + 1])
			{
				int tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
		}
	}
}


//对STL迭代器的泛化版本
template<class I>
void BubbleSort(I begin,I end)
{
	typename I::value_type tmp;
	I _end = end - 1;
	for (size_t i = 0; i < (size_t)(end - begin - 1); ++i)
	{
		for (I j = begin; j != _end; ++j)
		{
			if (*j > *(j + 1))
			{
				tmp = *j;
				*j = *(j + 1);
				*(j + 1) = tmp;
			}
		}
	}
}
