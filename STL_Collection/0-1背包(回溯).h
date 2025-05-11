#pragma once

/*
	Values：物品价值表
	Weights：物品重量表
	VW：价值和质量比值表
	n：物品数量
	W：背包容量
	Profit_Gained：已拿物品的价值
	Weight_Used：已拿物品的重量
	k：已经考虑了K个物品
	功能：试图将剩于n-k个物品放入背包。逐个取物品，判断是否能放进去
	能：继续放下一个
	不能：放一部分，然后rerturn 背包内物品总价值
	循环结束：return 背包内物品总价值
*/
float Bound(float* Values,
			int* Weights,
			float* VW,
			int n,
			int W,
			float Profit_Gained,
			int Weight_Used,
			int k)
{
	int i;
	for (i = k + 1; i < n; ++i)
	{
		if (Weight_Used + Weights[i] <= W)
		{
			Profit_Gained += Values[i];
			Weight_Used += Weights[i];
		}
		else
		{
			Profit_Gained += VW[i] * (W - Weight_Used);
			Weight_Used = W;
			return Profit_Gained;
		}
	}
	return Profit_Gained;
}


/*
	
*/
int* Knapsack(float* Values, int* Weights, float* VW, int n, int W)
{
	int current_weight = 0;
	float current_profit = 0;
	int Weight;
	float Profit = -1;
	int index = 0;

	int* X = (int*)malloc(sizeof(int) * n);
	int* Y = (int*)malloc(sizeof(int) * n);

	while (1)
	{
		//逐个选择物品，如果能放进背包，则放进去，然后更新背包内的质量和价值
		//如果放不进去，退出循环，此时index指向不能放进去的物品
		while (index < n && current_weight + Weights[index] <= W)
		{
			current_profit += Values[index];
			current_weight += Weights[index];
			Y[index] = 1;
			index++;
		}

		//如果index指向n，说明最后一个物品放进了背包
		//直接记录累计重量和价值
		if (index >= n)
		{
			Weight = current_weight;
			Profit = current_profit;
			index = n;
			int i;
			for (i = 0; i < n; ++i)
				X[i] = Y[i];
		}
		else//否则index的物品放不进去，置0
			Y[index] = 0;

		//判断index后面的物品放进去是否小于Profit，是则循环
		while (Bound(Values, Weights, VW, n, W, current_profit, current_weight, index) <= Profit)
		{
			//index向前回溯，直到遇到一个放进去的物品
			while (index != 0 && Y[index] != 1)
				index--;

			if (index == 0)
				return X;
			Y[index] = 0;
			current_profit -= Values[index];
			current_weight -= Weights[index];
		}
		index++;
	}
}