#pragma once

/*
	共n种不同面值的邮票，一封信最多能贴m张
	设计邮票的面值，
	使其能在一封信上，凑出连续的{1，2，3，...，max}
	并且max尽可能大
*/

int n, m;
int maxStamp;
int r;//前cur种邮票能支付的最大邮资
const int MM = 500;
const int MN = 50;
int x[MN];//递增数列，存放邮票面值
int y[MM];//y[j]=k，表示邮资为j的最小邮票张数为k
int ans[MN];
const int INF = 1000000;//表示不可达
void backtrace(int cur)
{
	//cur=n，说明已有0~cur-1，共n张邮票，
	//且r大于当前的最大面值和
	//保存到ans里，不需要寻找，return回上一个递归
	if (cur >= n && r > maxStamp)
	{
		maxStamp = r;
		for (int i = 0; i < n; ++i)
			ans[i] = x[i];

		return;
	}

	//备份r和y
	int backup_r = r;
	int backup_y[MM];
	for (int i = 0; i < MM; i++)
	{
		backup_y[i] = y[i];
	}
	
	//新面值至少为前一张邮票面值+1，才有意义
	//至多为r+1，因为
	//前n-1种面值m张最多凑r，凑不到r+1
	//新面值如果大于r+1，那么凑r+1仍然是前n-1种去凑，还是凑不出
	//所以新面值最多r+1
	for (int i = x[cur - 1] + 1; i <= r + 1; ++i)
	{
		x[cur] = i;//循环使x[cur]取每一个i，然后实验是否能满足
		//k从0到前一种邮票面值*(m-1)
		//表示遍历连续的面值和
		for (int k = 0; k <= x[cur - 1] * (m - 1); ++k)
		{
			if (y[k] >= m)continue;//若所需张数大于m，说明凑出k的组合达到n张，不能再加
			//如果凑出k的组合不满n张，就遍历差的张数：1~m-y[k]
			for (int num = 1; num <= m - y[k]; ++num)
			{
				if (k + x[cur] * num < MN)//加上num张新面值没出界
				{
					//则将对应面值和的张数更新
					y[k + x[cur] * num] = min(y[k + x[cur] * num], y[k] + num);
				}
			}
		}

		//经历上面的循环后，部分y数组的值更新
		//找到第一个不连续的面值和，作为r
		//说明，前n-1种面值和新面值i的组合最大能凑出r
		while (y[r + 1] < INF)r++;

		//然后递归下一张面值
		backtrace(cur + 1);

		//恢复r和y
		r = backup_r;
		for (int j = 0; j < MM; j++)y[j] = backup_y[j];
	}
	free(backup_y);
}

void backtrace2(int cur)
{
	if (cur >= m && r > maxStamp)
	{
		maxStamp = r;
		for (int i = 0; i < cur; ++i)
			ans[i] = x[i];
	}

	int backup_r = r;
	int backup_y[MN];
	for (int i = 0; i < MN; ++i)
		backup_y[i] = y[i];

	for (int i = x[cur - 1] + 1; i <= r + 1; ++i)
	{
		x[cur] = i;
		for (int k = 0; k < x[cur - 1] * (m - 1); k++)
		{
			for (int num = 1; num < m - y[k]; num++)
			{
				if (k + num * i < MM)
					y[k + num * i] = min(y[k] + num, y[k + num * i]);
			}

		}

		while (y[r + 1] < INF)r++;

		backtrace2(cur + 1);

		r = backup_r;
		for (int j = 0; j < MN; ++j)
			y[j] = backup_y[j];
	}

	free(backup_y);
}

/*
	这道题和n皇后很像，改进成用栈和循环的解决方式
	设数组stamp[n]表示邮票的面值，是递增数组
	acc[]
*/

