#pragma once

/*
	��������������ȥ������Ԫ�أ���һ���������õ�������
	��AC��ABC��һ��������

	������X������Y�Ĺ���������Z

	1.�����ӽṹ������X=x1x2...xm��Y=y1y2...yn��Z=z1z2...zk
		1.xm=yn����zk=xm=yn��Z(k-1)��X(m-1)��Y(n-1)�Ĺ���������
		2.xm!=yn����zk!=xm��Z��X(m-1)��Y�Ĺ���������
		3.xm!=yn����zk!=yn��Z��X��Y(n-1)�Ĺ���������
		ע�⣬xm!=yn����ֱ�ӽ�m��n��ǰ�ƣ���Ϊ����X�ĵ����ڶ���Ԫ��=Y�����һ��Ԫ��

	2.�ݹ鶨�����Ž��ֵ
		��l[i,j]��ʾ����Xi��Yj������������еĳ̶ȣ��������¶���
			l[i,j] = 1. 0����i=0��j=0
				     2. l[i-1,j-1]+1����xi=yj
					 3. max(l[i-1,j],l[i,j-1])����xi!=yj

*/

int** Lcs_Length(const char* str1, const char* str2, int str1_length, int str2_length)
{
	int i, j;//i��ʾstr1��ǰi���ַ���j��ʾstr2��ǰj���ַ�
	int** l = (int**)malloc(sizeof(int*) * (str1_length + 1));
	//l���str1ǰi���ַ���str2ǰj���ַ�����󹫹������еĳ̶�
	int** b = (int**)malloc(sizeof(int*) * (str1_length + 1));
	//b��������0��ʾstr1[i]==str2[j]��������

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
		for (j = 1; j < str2_length; ++j)//˫ѭ�����������ַ���ͷ��ʼ���
		{
			if (str2[i - 1] == str2[j - 1])//�����ַ����
			{
				l[i][j] = l[i - 1][j - 1] + 1;//���������l[i - 1][j - 1]��1
				b[i][j] = 0;//ָ�������Ϸ�
			}
			else if(l[i-1][j]>=l[i][j-1])
			{
				l[i][j] = l[i - 1][j];
				b[i][j] = 1;//ָ�����Ϸ�
			}
			else
			{
				l[i][j] = l[i][j-1];
				b[i][j] = 2;//ָ������
			}
		}
	}
	return b;
}