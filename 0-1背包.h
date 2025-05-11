#pragma once
#include <vector>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
using std::vector;
/*
	0-1背包问题
	有n个物品，第i个物品的价值为vi，重为wi，其中vi和wi均为非负数
	背包容量为W
	选择物品放入背包，使背包内的物品的总价值最大

	设xi表示物品i是否放入背包，取0或1
		maxW=Σ(vi * xi)
	约束条件为：
		Σ(wi * xi) <= W

	1.刻画最优解结构
		对于某个解，物品n放在背包里，则x1,x2,..,xn-1是一个子问题
		这个子问题是：对于W-wn，从1,2,...,n-1物品重，选择放入背包的物品是总价值最大

	2.递归定义最优解的值
		设c[i,w]表示背包容量为w时，i个物品导致最优解的总价值
		即有：
			c[i,w]=1. 0，当i=0或w=0
				   2. c[i-1,w]，当wi>W
				   3. max(c[i-1,w-wi]+vi,c[i-1,w])，当i>0且wi<=w

		对于第3种情况：比较i放进去还是不放进去的背包总价值

	3.计算最优解的值
*/

/*
	n：物品个数
	W：背包最大容量
	Weights：物品重量数组
	Value：物品价值数组
*/
void KnapsackDP(int n, int W, int* Weights, float* Values)
{
	int i, w;
	vector<vector<int>> c(n + 1, vector<int>(W + 1, 0));
	//第一个维度：考虑前i个物品
	//第二个维度：背包的可容纳重量w
	//值：表示前i个物品在容量为w时的最大价值


	for (i = 1; i <= n; ++i)//试图放进第i个物品，已经考虑了前i-1个物品
	{
		c[i][0] = 0;        //背包可容纳重量为0，当然放不进去东西，置0
		for (w = 1; w <= W; ++w)//逐渐增加背包的可容纳重量，直到W
		{
			if (Weights[i - 1] <= w)//如果物品i的重量可以放进背包
			{
				//能放进去，但是有两种情况，一个是放进去，一个是不放进去
				//放不放进去取决于到底那种情况得到的价值最大
				//即前i-1个物品在w - Weights[i - 1]的最大价值 加上 Value[i] 是否大于 前i-1个物品在w的最大价值
				if (Values[i - 1] + c[i - 1][w - Weights[i - 1]] > c[i - 1][w])
					c[i][w] = Values[i - 1] + c[i - 1][w - Weights[i - 1]];
				else
					c[i][w] = c[i - 1][w];
			}
			else
				//如果放不进去，则直接等于前i-1个物品在重量为w的解
				c[i][w] = c[i - 1][w];
		}
	}
}


/*
	变体的背包问题
	对价格为10的整数倍的物品，有price、importance和depend
	对于现有M的金额，求最大的满意度
*/
struct stuff
{
	int price;
	int importance;
	int depend;
	stuff() :price(0), importance(0), depend(0) {}

	stuff(int p, int i, int d) :price(p), importance(i), depend(d) {}

	int satif()const { return price * importance; }
};

void satifPack()
{
	int M, N;
	cin >> M >> N;
	cout << M << endl << N << endl;
	N++;
	M = 1 + M / 10;
	vector<stuff> v(1);
	for (int n = 1; n < N; ++n)
	{
		int p, i, d;
		cin >> p >> i >> d;
		v.emplace_back(p, i, d);
	}
	
	vector<int>sel(N, 0);
	sel[0] = 1;

	for (int n = 1; n < N; ++n)
	{
		for (int m = 1; m < M; m++)
		{
			if(v[n].price<=m&&
				sel[v[n]]
				)
		}
	}
}