#pragma once

/*
	����������
	��������arr������һ�������������飨ԭ����ͷβȥ��0����Ԫ�أ�
	������Ԫ��֮���Ȼ�����෴����Ԫ�صĴ�С�ǰ��������µ�˳�����е�
	����������Ϊ����������

	����һ��������������������ĳ���

	����Ԫ��i������ǰ������һ����i-1Ϊ��β�����������飬��i�������Ȼ������������
	���������������ĳ���+1����������Ϊ1
	�����Ԫ����ȣ�������Ϊ1��
	���i�����ж�i-1�Ƿ�С��i-2
	���iС�����ж�i-1�Ƿ����i-2

*/

int maxTurbelenceArrayLength2(int* arr, int len)
{
	int res;
	int* increased = (int*)malloc(sizeof(int) * len);
	int* decreased = (int*)malloc(sizeof(int) * len);
	increased[0] = 1;
	decreased[0] = 1;
	for (int i = 1; i < len; i++)
	{
		if (arr[i] > arr[i - 1])
		{
			increased[i] = decreased[i - 1] + 1;
			decreased[i] = 1;
		}
		else if (arr[i] < arr[i - 1])
		{
			decreased[i] = increased[i - 1] + 1;
			increased[i] = 1;
		}
		else
		{
			increased[i] = 1;
			decreased[i] = 1;
		}
		res = max(increased[i], decreased[i]);
	}
	return res;
}