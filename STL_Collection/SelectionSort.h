#pragma once
#include <vector>
using namespace std;
/*
	选择排序
	从未排序区间里找到最小的元素放在数组前面，
	内循环找到未排序数组里最小元素所在的下标
	找到后和外循环的i所在的元素互换。
	外循环从0至len-1。
*/

void SelectionSort1(int* arr, int len)
{
	int minIndex, temp;
	for (int i = 0; i < len - 1; ++i)
	{
		minIndex = i;
		for (int j = i + 1; j < len; ++j)//在未排序的数组里循环，找到最小元素的下标
		{
			if (arr[j] < arr[minIndex])//这一步很妙，因为需要互换的是i，minIndex的任务是找到最小元素
									   //因此arr[j]总是和更小的元素比较
			{
				minIndex = j;//不必存储最小值，只需存储最小值的下标
			}
		}
		//将最小元素的下标和此次外循环的下标所指元素互换
		temp = arr[i];
		arr[i] = arr[minIndex];
		arr[minIndex] = temp;
	}
}

void SelectionSort2(int* arr, int len)
{
	int min,tmp;
	for (int i = 0; i < len - 1; i++)
	{
		min = i;
		for (int j = i + 1; j < len; j++)
		{
			if (arr[j] < arr[min])
				min = j;
		}
		tmp = arr[min];
		arr[min] = arr[i];
		arr[i] = tmp;
	}
}



//以下是对STL迭代器泛化的版本
template <class I>
void SelectionSort(I begin,I end)
{
	typename I::value_type tmp;
	I mini;
	for (I i = begin;i != end - 1;++i)
	{
		mini = i;
		for (I j = i + 1; j != end; ++j)
		{
			if (*j < *mini)
				mini = j;
		}
		if (mini != i)
		{
			tmp = *i;
			*i = *mini;
			*mini = tmp;
		}
	}
};













