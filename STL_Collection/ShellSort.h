#pragma once

/*
	希尔排序
	若要深刻理解希尔排序，需要理解插入排序
	插入排序可以比喻成抽牌，起初自己手里有一张牌，循环遍历就像抽牌，不断从未排序的数组里抽出新元素
	然后内循环从后往前遍历已排序的数组

	希尔排序将整个数组分为gap份，所有下标相隔gap的元素属于同一组，
	对每组进行插入排序，然后将gap缩小

	外循环，每次gap减半，缩小到1；
	内循环：前gap个元素是每组的第一个元素，是有序区，因此i从gap迭代到len-1


*/

void ShellSort1(int* arr, int len)
{
	for (int gap = len / 2; gap > 0; gap /= 2)
	{
		for (int i = gap; i < len; ++i)
		{
			int j = i;
			int current = arr[i];
			while (j - gap >= 0 && arr[j - gap] > current)
			{
				arr[j] = arr[j - gap];
				j -= gap;
			}
			arr[j] = current;
		}
	}
}

void ShellSort2(int* arr, int len)
{
	for (int gap = len / 2; gap > 0; gap /= 2)
	{
		for (int i = gap; i < len; ++i)
		{
			int current = arr[i];
			int j = i;
			while (j - gap >= 0 && arr[j - gap] > current)
			{
				arr[j] = arr[j - gap];
				j -= gap;

			}
			arr[j] = current;
		}
	}
}

void ShellSort(int* arr, int len)
{
	for (int gap = len / 2; gap > 0; gap /= 2)
	{
		for (int i = gap; i < len; i++)
		{
			int cur = arr[i];
			int j = i;
			while (j - gap >= 0 && arr[j - gap] > cur)
			{
				arr[j] = arr[j - gap];
				j -= gap;
			}
			arr[j] = cur;
		}
	}
}