#pragma once

/*
	对一无序序列，元素为负整数、0、正整数
	求一子序列，使子序列元素和最大，且元素个数最多

	采用分治法
	将子段分为两份，递归求得最大子段和

	1.确定递归的边界条件
		显然，当子段长度等于1时无需再分，这时，
		如果该元素为正，返回该元素
		如果该元素为负或0，返回0，表示该元素不记入最大子段

	2.设计递归模式
		最大子段的情况有3种
		1.最大子段在左侧
		2.最大子段在右侧
		3.最大字段i~j，其中0 <= i < n/2，n/2 <= j <n
	
		对于1和2，递归求子段的最大子段和即可
		对于3，需计算S1 = max(Σa[i]) 和 S2 = max(Σa[j])，其中i迭代到n/2，j从n/2开始迭代

	3.合并
		向上一层返回此次递归求得的最大子段和
		对于上一层序列，求得其两个子段的最大子段和ls和rs，
		然后求得sum = s1+s2，取sum，ls，rs三者种的最大值，并返回

*/

int MaxSubSum(int* Array, int l, int r)
{
	int sum = 0;
	int i;
	if (l == r)
	{
		if (Array[l] > 0)
			sum = Array[l];
		else
			sum = 0;
	}
	else
	{
		int center = (l + r) / 2;
		int leftsum = MaxSubSum(Array, l, center);
		int rightsum = MaxSubSum(Array, center + 1, r);

		int s1 = 0;
		int lefts = 0;
		for (i = center; i >= l; --i)
		{
			lefts += Array[i];
			if (lefts > s1)//如果元素为负，则和比原来小，不会修改s1
				s1 = lefts;
		}

		int s2 = 0;
		int rights = 0;
		for (i = center + 1; i <= r; ++i)
		{
			rights += Array[i];
			if (rights > s2)
				s2 = rights;
		}
		sum = s1 + s2;

		if (sum < leftsum)
			sum = leftsum;
		if (sum < rightsum)
			sum = rightsum;
	}
	return sum;
}
