#pragma once

/*
	背包问题和0-1背包问题很像。
	0-1背包问题中，物品不是装入背包就是不装入，即xi = 0或1
	而背包问题中，物品可以部分装入背包，即 0 =< xi <= 1
*/

/*
	Weights：物品重量表，要求从小到达排序
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