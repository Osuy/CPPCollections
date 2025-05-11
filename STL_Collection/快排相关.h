#pragma once


/*
	�������ƿ��ŵķ������ҵ�һ���������е�k�����

	���Ž���һ�η����󣬾ͽ���׼��������ȷ��λ�ã�ǰ�涼�Ȼ�׼С�����涼�Ȼ�׼��
	����֪����׼���±�Ϊi��
	��i>k������ǰ���ң�
	��i<k�����ں����ң�

*/

int partition(int* arr, int left, int right)
{
	if (left >= right)return left;

	int tmp = arr[left];
	while (left < right)
	{
		while (left<right && arr[right]>tmp)
			right--;
		arr[left] = arr[right];

		while (left < right && arr[left] < tmp)
			left--;
		arr[right] = arr[left];
	}
	arr[left] = tmp;
	return left;
}

int findKth(int* arr,int left,int right, int n)
{
	if (right - left < n)return -1;

	int partitionIndex = partition(arr, left, right);

	if (partitionIndex < n) return findKth(arr,partitionIndex+1,right,n);
	if (partitionIndex < n) return findKth(arr,left,partitionIndex-1,n);
	return partitionIndex;
}