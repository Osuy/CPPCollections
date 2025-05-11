#pragma once

/*
	ϣ������
	��Ҫ������ϣ��������Ҫ����������
	����������Ա����ɳ��ƣ�����Լ�������һ���ƣ�ѭ������������ƣ����ϴ�δ���������������Ԫ��
	Ȼ����ѭ���Ӻ���ǰ���������������

	ϣ���������������Ϊgap�ݣ������±����gap��Ԫ������ͬһ�飬
	��ÿ����в�������Ȼ��gap��С

	��ѭ����ÿ��gap���룬��С��1��
	��ѭ����ǰgap��Ԫ����ÿ��ĵ�һ��Ԫ�أ��������������i��gap������len-1


*/

void ShellSort1(int* arr, int len)
{
	for (int gap = len / 2; gap > 0; gap /= 2)
	{
		for (int i = gap; i < len; ++i)
		{
			int j = i;
			int current = arr[i];
			while (j - gap >= 0 && arr[j - gap] > current)
			{
				arr[j] = arr[j - gap];
				j -= gap;
			}
			arr[j] = current;
		}
	}
}

void ShellSort2(int* arr, int len)
{
	for (int gap = len / 2; gap > 0; gap /= 2)
	{
		for (int i = gap; i < len; ++i)
		{
			int current = arr[i];
			int j = i;
			while (j - gap >= 0 && arr[j - gap] > current)
			{
				arr[j] = arr[j - gap];
				j -= gap;

			}
			arr[j] = current;
		}
	}
}

void ShellSort(int* arr, int len)
{
	for (int gap = len / 2; gap > 0; gap /= 2)
	{
		for (int i = gap; i < len; i++)
		{
			int cur = arr[i];
			int j = i;
			while (j - gap >= 0 && arr[j - gap] > cur)
			{
				arr[j] = arr[j - gap];
				j -= gap;
			}
			arr[j] = cur;
		}
	}
}