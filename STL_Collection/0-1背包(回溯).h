#pragma once

/*
	Values����Ʒ��ֵ��
	Weights����Ʒ������
	VW����ֵ��������ֵ��
	n����Ʒ����
	W����������
	Profit_Gained��������Ʒ�ļ�ֵ
	Weight_Used��������Ʒ������
	k���Ѿ�������K����Ʒ
	���ܣ���ͼ��ʣ��n-k����Ʒ���뱳�������ȡ��Ʒ���ж��Ƿ��ܷŽ�ȥ
	�ܣ���������һ��
	���ܣ���һ���֣�Ȼ��rerturn ��������Ʒ�ܼ�ֵ
	ѭ��������return ��������Ʒ�ܼ�ֵ
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
		//���ѡ����Ʒ������ܷŽ���������Ž�ȥ��Ȼ����±����ڵ������ͼ�ֵ
		//����Ų���ȥ���˳�ѭ������ʱindexָ���ܷŽ�ȥ����Ʒ
		while (index < n && current_weight + Weights[index] <= W)
		{
			current_profit += Values[index];
			current_weight += Weights[index];
			Y[index] = 1;
			index++;
		}

		//���indexָ��n��˵�����һ����Ʒ�Ž��˱���
		//ֱ�Ӽ�¼�ۼ������ͼ�ֵ
		if (index >= n)
		{
			Weight = current_weight;
			Profit = current_profit;
			index = n;
			int i;
			for (i = 0; i < n; ++i)
				X[i] = Y[i];
		}
		else//����index����Ʒ�Ų���ȥ����0
			Y[index] = 0;

		//�ж�index�������Ʒ�Ž�ȥ�Ƿ�С��Profit������ѭ��
		while (Bound(Values, Weights, VW, n, W, current_profit, current_weight, index) <= Profit)
		{
			//index��ǰ���ݣ�ֱ������һ���Ž�ȥ����Ʒ
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