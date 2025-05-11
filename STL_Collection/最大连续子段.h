#pragma once

/*
	��һ�������У�Ԫ��Ϊ��������0��������
	��һ�����У�ʹ������Ԫ�غ������Ԫ�ظ������

	���÷��η�
	���Ӷη�Ϊ���ݣ��ݹ��������Ӷκ�

	1.ȷ���ݹ�ı߽�����
		��Ȼ�����Ӷγ��ȵ���1ʱ�����ٷ֣���ʱ��
		�����Ԫ��Ϊ�������ظ�Ԫ��
		�����Ԫ��Ϊ����0������0����ʾ��Ԫ�ز���������Ӷ�

	2.��Ƶݹ�ģʽ
		����Ӷε������3��
		1.����Ӷ������
		2.����Ӷ����Ҳ�
		3.����ֶ�i~j������0 <= i < n/2��n/2 <= j <n
	
		����1��2���ݹ����Ӷε�����Ӷκͼ���
		����3�������S1 = max(��a[i]) �� S2 = max(��a[j])������i������n/2��j��n/2��ʼ����

	3.�ϲ�
		����һ�㷵�ش˴εݹ���õ�����Ӷκ�
		������һ�����У�����������Ӷε�����Ӷκ�ls��rs��
		Ȼ�����sum = s1+s2��ȡsum��ls��rs�����ֵ����ֵ��������

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
			if (lefts > s1)//���Ԫ��Ϊ������ͱ�ԭ��С�������޸�s1
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
