#pragma once

/*
	子序列是主序列去掉若干元素（不一定连续）得到的序列
	例AC是ABC的一个子序列

	求序列X和序列Y的公共子序列Z

	1.最优子结构。对于X=x1x2...xm，Y=y1y2...yn，Z=z1z2...zk
		1.xm=yn，则zk=xm=yn，Z(k-1)是X(m-1)和Y(n-1)的公共子序列
		2.xm!=yn，则zk!=xm，Z是X(m-1)和Y的公共子序列
		3.xm!=yn，则zk!=yn，Z是X和Y(n-1)的公共子序列
		注意，xm!=yn不能直接将m和n都前移，因为可能X的倒数第二个元素=Y的最后一个元素

	2.递归定义最优解的值
		设l[i,j]表示序列Xi和Yj的最长公共子序列的程度，则有以下定义
			l[i,j] = 1. 0，当i=0或j=0
				     2. l[i-1,j-1]+1，当xi=yj
					 3. max(l[i-1,j],l[i,j-1])，当xi!=yj

*/

int** Lcs_Length(const char* str1, const char* str2, int str1_length, int str2_length)
{
	int i, j;//i表示str1的前i个字符，j表示str2的前j个字符
	int** l = (int**)malloc(sizeof(int*) * (str1_length + 1));
	//l存放str1前i个字符于str2前j个字符的最大公共子序列的程度
	int** b = (int**)malloc(sizeof(int*) * (str1_length + 1));
	//b存放情况，0表示str1[i]==str2[j]。。。。

	for (i = 0; i <= str1_length; ++i)
	{
		l[i] = (int*)malloc(sizeof(int) * (str2_length + 1));
		b[i] = (int*)malloc(sizeof(int) * (str2_length + 1));
	}

	for (i = 0; i <= str1_length; ++i)
		l[i][0] = 0;

	for (j = 0; j <= str2_length; ++j)
		l[0][j] = 0;

	for (i = 1; i < str1_length; ++i)
	{
		for (j = 1; j < str2_length; ++j)//双循环，从两个字符串头开始向后
		{
			if (str2[i - 1] == str2[j - 1])//若两字符相等
			{
				l[i][j] = l[i - 1][j - 1] + 1;//长度相比于l[i - 1][j - 1]多1
				b[i][j] = 0;//指向表的左上方
			}
			else if(l[i-1][j]>=l[i][j-1])
			{
				l[i][j] = l[i - 1][j];
				b[i][j] = 1;//指向表的上方
			}
			else
			{
				l[i][j] = l[i][j-1];
				b[i][j] = 2;//指向表的左方
			}
		}
	}
	return b;
}