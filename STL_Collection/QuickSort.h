#pragma once

//�������+swap
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

//˫�����+ֱ�Ӹ�ֵ��Ч�ʸ���
int Partition2(int* arr, int l, int r)
{
	//��������ĵ�һ��Ԫ��
	int tmp = arr[l];
	//��ʱ��l��ָ�Ķ�����Ա�����
	while (l < r)
	{
		//���ұ���һ����tmpС����������arr[l]
		while (l < r && arr[r] >= tmp)
		{
			--r;
		}
		arr[l] = arr[r];
		//��ʱr��ָ��Ԫ�ؿ��Ա�����
		//�������һ����tmp���������arr[r]
		while (l < r && arr[l] <= tmp)
		{
			++l;
		}
		arr[r] = arr[l];
	}

	//�˳�ѭ��ʱ��l==r����ʱ��tmp����arr[l]����
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
		//����һ�η���ѡ��󣬷����м�ֵ��Ȼ�����м�ֵΪ�ֽ����ٶ�����������ѡ��
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