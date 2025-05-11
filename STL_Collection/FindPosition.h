#pragma once

int n, k, ax, ay;
int a[5000][5000];

/*
	在二维数组中找到k的位置，
	对于任意元素，其右边和下边的元素大于它
*/


void findPosition()
{
	int i = n, j = n;
	while (j >= 0)
	{
		if (a[n][j] < k)break;
		j--;
	}
	j++;//定位到第一个大于k的数

	while (a[i][j] != k)
	{
		while (a[i][j] > k)i--;
		while (a[i][j] < k)j++;

	}
	ax = i, ay = j;
}