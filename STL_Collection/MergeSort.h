//#include <vector>
//
//
///*
//	arr分为两个区间
//	(l, mid)和(mid + 1, r)
//*/
//void merge(int* arr, int l, int mid, int r)
//{
//	//新建一个数组，存入所有arr的元素
//	std::vector<int> v;
//	for (int m = l; m <= r; m++)
//	{
//		v.push_back(arr[m]);
//	}
//
//	//i和j分别指向左右两个数组的起点，用于遍历
//	int i = l, j = mid + 1;
//
//	for (int k = l; k <= r; ++k)
//	{
//		if (i > mid)//当左数组遍历完，继续遍历右数组
//		{
//			arr[k] = v[j - l];
//			++j;
//		}
//		else if (j > r)//当右数组遍历完，继续遍历左数组
//		{
//			arr[k] = v[i - l];
//			++i;
//		}
//		else//两个数组都没遍历完，比较i和j指向的元素，谁小取谁，然后i/j自加
//		{
//			if (v[i - l] < v[j - l])
//			{
//				arr[k] = v[i - l];
//				++i;
//			}
//			else
//			{
//				arr[k] = v[j - l];
//				++j;
//			}
//		}
//	}
//}
//
//void merge2(int* arr, int l, int mid, int r)
//{
//	if (l >= r)return;
//	int i,j;
//	std::vector<int> v;
//	for (i = l; i <= r; i++)
//		v.push_back(arr[i]);
//
//	i = l, j = mid + 1;
//	for (int k = l; k <= r; k++)
//	{
//		if (l > mid)
//			arr[k] = v[i++ - l];
//		else if (j > r)
//			arr[k] = v[j++ - l];
//		else
//		{
//			if (v[i] > v[j])
//				arr[k] = v[j++ - l];
//			else
//				arr[k] = v[i++ - l];
//		}
//	}
//}
//
////递归版
//void mergesort1(int* arr, int l, int r)
//{
//	if (l >= r)
//		return;
//	 
//	int mid = (l + r) / 2;
//	//return merge(mergesort(arr, middle), mergesort(arr + middle, len - middle));
//	mergesort1(arr, l, mid);
//	mergesort1(arr, mid + 1, r);
//	merge(arr, l, mid, r);
//}
//
///*
//	非递归版本
//	s为分组长度，从2开始，每次翻倍
//	对数组分成大小为s的若干组，然后对其merge
//	数组尾端可能出现不足s的情况，需要在循环题外处理
//
//*/
//void mergesort1(int* arr, int len)
//{
//	int s = 2, i;
//	while (s <= len)//外循环控制s，起始为2，每次倍增。将数组分为len/s个小组，每组s个元素进行merge
//	{
//		i = 0;
//		while (i + s <= len)//内循环控制i，起始为0，每次加上s，即以步长s遍历数组
//		{
//			merge(arr, i, i + (s - 1) / 2, i + s - 1);
//			i += s;
//		}
//		
//		//可能会有n-s<i<n-1的情况，此时还剩余部分为归并，但却退出了循环
//		//手动归并
//		merge(arr, i + 1, (i + len) / 2, len - 1);
//
//		s *= 2;//s倍增
//	}
//
//	//退出循环后，得到的数组尚未排序好，而是两个排序好的子数组，需要再对整个数组手动归并一次
//	merge(arr, 0, s / 2 - 1, len - 1);
//}
//
//void mergesort2(int* arr, int len)
//{
//	int s = 2, i;
//	while (s <= len)
//	{
//		i = 0;
//		while (i + s < len)
//		{
//			merge(arr, i, i + (s - 1) / 2, i + s - 1);
//			i += s;
//		}
//		merge(arr, i, (i + len - 1) / 2, len - 1);
//		s *= 2;
//	}
//
//	merge(arr, 0, s / 2 - 1, len - 1);
//}
