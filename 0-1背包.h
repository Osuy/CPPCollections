#pragma once
#include <vector>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
using std::vector;
/*
	0-1��������
	��n����Ʒ����i����Ʒ�ļ�ֵΪvi����Ϊwi������vi��wi��Ϊ�Ǹ���
	��������ΪW
	ѡ����Ʒ���뱳����ʹ�����ڵ���Ʒ���ܼ�ֵ���

	��xi��ʾ��Ʒi�Ƿ���뱳����ȡ0��1
		maxW=��(vi * xi)
	Լ������Ϊ��
		��(wi * xi) <= W

	1.�̻����Ž�ṹ
		����ĳ���⣬��Ʒn���ڱ������x1,x2,..,xn-1��һ��������
		����������ǣ�����W-wn����1,2,...,n-1��Ʒ�أ�ѡ����뱳������Ʒ���ܼ�ֵ���

	2.�ݹ鶨�����Ž��ֵ
		��c[i,w]��ʾ��������Ϊwʱ��i����Ʒ�������Ž���ܼ�ֵ
		���У�
			c[i,w]=1. 0����i=0��w=0
				   2. c[i-1,w]����wi>W
				   3. max(c[i-1,w-wi]+vi,c[i-1,w])����i>0��wi<=w

		���ڵ�3��������Ƚ�i�Ž�ȥ���ǲ��Ž�ȥ�ı����ܼ�ֵ

	3.�������Ž��ֵ
*/

/*
	n����Ʒ����
	W�������������
	Weights����Ʒ��������
	Value����Ʒ��ֵ����
*/
void KnapsackDP(int n, int W, int* Weights, float* Values)
{
	int i, w;
	vector<vector<int>> c(n + 1, vector<int>(W + 1, 0));
	//��һ��ά�ȣ�����ǰi����Ʒ
	//�ڶ���ά�ȣ������Ŀ���������w
	//ֵ����ʾǰi����Ʒ������Ϊwʱ������ֵ


	for (i = 1; i <= n; ++i)//��ͼ�Ž���i����Ʒ���Ѿ�������ǰi-1����Ʒ
	{
		c[i][0] = 0;        //��������������Ϊ0����Ȼ�Ų���ȥ��������0
		for (w = 1; w <= W; ++w)//�����ӱ����Ŀ�����������ֱ��W
		{
			if (Weights[i - 1] <= w)//�����Ʒi���������ԷŽ�����
			{
				//�ܷŽ�ȥ�����������������һ���ǷŽ�ȥ��һ���ǲ��Ž�ȥ
				//�Ų��Ž�ȥȡ���ڵ�����������õ��ļ�ֵ���
				//��ǰi-1����Ʒ��w - Weights[i - 1]������ֵ ���� Value[i] �Ƿ���� ǰi-1����Ʒ��w������ֵ
				if (Values[i - 1] + c[i - 1][w - Weights[i - 1]] > c[i - 1][w])
					c[i][w] = Values[i - 1] + c[i - 1][w - Weights[i - 1]];
				else
					c[i][w] = c[i - 1][w];
			}
			else
				//����Ų���ȥ����ֱ�ӵ���ǰi-1����Ʒ������Ϊw�Ľ�
				c[i][w] = c[i - 1][w];
		}
	}
}


/*
	����ı�������
	�Լ۸�Ϊ10������������Ʒ����price��importance��depend
	��������M�Ľ������������
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