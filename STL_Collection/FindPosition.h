#pragma once

int n, k, ax, ay;
int a[5000][5000];

/*
	�ڶ�ά�������ҵ�k��λ�ã�
	��������Ԫ�أ����ұߺ��±ߵ�Ԫ�ش�����
*/


void findPosition()
{
	int i = n, j = n;
	while (j >= 0)
	{
		if (a[n][j] < k)break;
		j--;
	}
	j++;//��λ����һ������k����

	while (a[i][j] != k)
	{
		while (a[i][j] > k)i--;
		while (a[i][j] < k)j++;

	}
	ax = i, ay = j;
}