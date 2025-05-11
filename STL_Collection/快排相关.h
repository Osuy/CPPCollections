#pragma once


/*
	利用类似快排的方法，找到一串无序数中第k大的数

	快排进行一次分区后，就将基准放在了正确的位置，前面都比基准小，后面都比基准大
	这是知道基准的下标为i，
	若i>k，就在前面找，
	若i<k，就在后面找，

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