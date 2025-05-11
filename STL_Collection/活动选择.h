#pragma once

/*
	若个干具有竞争性得活动要求互斥使用某一公共资源，
	如何选择最大相容活动的集合

	假设有n个需要使用互斥某资源的活动S={a1,a2,...,an}
	每个活动ai有一个开始时间si和结束时间fi，形式为半开求区间[si,fi)
	如果活动ai和活动aj的活动区间不重叠，则ai和aj可兼容

	需要选择一系列互相兼容的活动，是资源的占据时间最大

	该问题可以用动态规划求解，但该问题同时又具有贪心选择性质
	可以使用贪心算法求解

	定义集合Sij={ak|fi<=sk<fk<=sj}
	头活动i和尾活动j不计入S
	即：Sij的任意一个活动的开始时间比头活动的结束时间晚
	结束时间比尾活动的开始时间早

	对于任意Sij，设am是第一个活动，则
	1.am在Sij的子集中
	2.Sim为空，Smj为非空，Smj可以作为子问题
*/

int OptimalSubset[100];

/*
	递归版
	s：活动开始时间表
	f：活动结束时间表
	index：当前考虑某个活动
	n：活动总数
	功能：找到第一个兼容的活动，就把活动写入，然后递归找下一个活动
*/
int RecursiveActivitySelector(int* s, int* f, int index, int n)
{
	int m = index + 1;//m表示下一个活动
	static int activity_num = 0;

	//m不超出n，且m的开始时间小于当前活动的结束时间，说明m不兼容，需自增
	while (m <= n && s[m] <= f[index])
		m++;

	//循环退出后，找到了一个兼容的m，或者越界
	if (m <= n)//若没越界
	{
		OptimalSubset[activity_num++] = m;
		RecursiveActivitySelector(s, f, m, n);
	}
}

/*
	非递归版
	找到一个兼容活动m，就写入，然后index指向m，m继续迭代
*/
int GreedyActivitySelector(int* s, int* f, int n)
{
	int activity_num = 0;
	OptimalSubset[activity_num++] = 1;
	int index = 1;
	int m;

	for (m = 2; m <= n; ++m)
	{
		if (s[m] >= f[index])
		{
			OptimalSubset[activity_num++] = m;;
			index = m;
		}
	}
	return activity_num;
}
