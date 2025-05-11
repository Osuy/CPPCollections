#pragma once

/*
	���������0-1�����������
	0-1���������У���Ʒ����װ�뱳�����ǲ�װ�룬��xi = 0��1
	�����������У���Ʒ���Բ���װ�뱳������ 0 =< xi <= 1
*/

/*
	Weights����Ʒ������Ҫ���С��������
*/

float* GreedyKnapsack(int n, int W, int* Weights, float* Values, float* VW)
{
	int i;
	float* x = (float*)malloc(sizeof(float) * n);
	for (i = 0; i < n; ++i)
		x[i] = 0;
	for (i = 0; i < n; ++i)
	{
		if (Weights[i] <= W)
		{
			x[i] = 1;
			W = W - Weights[i];
		}
			break;
	}
	if (i < n)
		x[i] = W / (float)Weights[i];
	return x;
}