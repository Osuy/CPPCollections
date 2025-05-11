#pragma once

//单向遍历+swap
int Partition1(int* arr, int left, int right)
{
	int tmp,pivot = left;
	int swapIndex = pivot + 1;
	for (int i = swapIndex; i < right; ++i)
	{
		if (arr[i] < arr[pivot])
		{
			tmp = arr[i];
			arr[i] = arr[swapIndex];
			arr[swapIndex] = tmp;
			++swapIndex;
		}
	}

	tmp = arr[pivot];
	arr[pivot] = arr[swapIndex-1];
	arr[swapIndex-1] = tmp;
	return swapIndex - 1;
}

//双向遍历+直接赋值，效率更好
int Partition2(int* arr, int l, int r)
{
	//保存区间的第一个元素
	int tmp = arr[l];
	//此时，l所指的对象可以被覆盖
	while (l < r)
	{
		//从右边找一个比tmp小的数，赋给arr[l]
		while (l < r && arr[r] >= tmp)
		{
			--r;
		}
		arr[l] = arr[r];
		//此时r所指的元素可以被覆盖
		//从左边找一个比tmp大的数赋给arr[r]
		while (l < r && arr[l] <= tmp)
		{
			++l;
		}
		arr[r] = arr[l];
	}

	//退出循环时，l==r，此时将tmp赋给arr[l]即可
	arr[l] = tmp;
	return l;
}

int partation2(int* arr, int l, int r)
{
	if (l >= r)return r;

	int tmp = arr[l];
	while (l < r)
	{
		while (l < r && arr[r] > tmp)
			r--;
		arr[l] = arr[r];
		while (l<r && arr[l]>tmp)
			l++;
		arr[r] = arr[l];
	}
	arr[l] = tmp;
	return l;
}

void QuickSort1(int* arr, int left, int right)
{
	if (right>left)
	{
		//进行一次分区选捡后，返回中间值，然后以中间值为分界线再对左右两部分选捡
		int partitionIndex = Partition1(arr, left,right);
		QuickSort1(arr, 0, partitionIndex - 1);
		QuickSort1(arr, partitionIndex + 1, right);
	}
}

template<class I>
I Partition(I left, I right)
{
	typename I::value_type tmp;
	I index = left + 1;
	for (I i = index; i != right; ++i)
	{
		if (*i < *left)
		{
			tmp = *i;
			*i = *index;
			*index = tmp;
			++index;
		}
	}
	--index;
	tmp = *index;
	*index = *left;
	*left = tmp;
	return index;
}

template<class I>
void QuickSort(I left, I right)
{
	if (right - left > 0)
	{
		I partIndex = Partition(left, right);
		QuickSort(left, partIndex - 1);
		QuickSort(partIndex + 1, right);
	}
}