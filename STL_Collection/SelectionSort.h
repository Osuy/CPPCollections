#pragma once
#include <vector>
using namespace std;
/*
	ѡ������
	��δ�����������ҵ���С��Ԫ�ط�������ǰ�棬
	��ѭ���ҵ�δ������������СԪ�����ڵ��±�
	�ҵ������ѭ����i���ڵ�Ԫ�ػ�����
	��ѭ����0��len-1��
*/

void SelectionSort1(int* arr, int len)
{
	int minIndex, temp;
	for (int i = 0; i < len - 1; ++i)
	{
		minIndex = i;
		for (int j = i + 1; j < len; ++j)//��δ�����������ѭ�����ҵ���СԪ�ص��±�
		{
			if (arr[j] < arr[minIndex])//��һ�������Ϊ��Ҫ��������i��minIndex���������ҵ���СԪ��
									   //���arr[j]���Ǻ͸�С��Ԫ�رȽ�
			{
				minIndex = j;//���ش洢��Сֵ��ֻ��洢��Сֵ���±�
			}
		}
		//����СԪ�ص��±�ʹ˴���ѭ�����±���ָԪ�ػ���
		temp = arr[i];
		arr[i] = arr[minIndex];
		arr[minIndex] = temp;
	}
}

void SelectionSort2(int* arr, int len)
{
	int min,tmp;
	for (int i = 0; i < len - 1; i++)
	{
		min = i;
		for (int j = i + 1; j < len; j++)
		{
			if (arr[j] < arr[min])
				min = j;
		}
		tmp = arr[min];
		arr[min] = arr[i];
		arr[i] = tmp;
	}
}



//�����Ƕ�STL�����������İ汾
template <class I>
void SelectionSort(I begin,I end)
{
	typename I::value_type tmp;
	I mini;
	for (I i = begin;i != end - 1;++i)
	{
		mini = i;
		for (I j = i + 1; j != end; ++j)
		{
			if (*j < *mini)
				mini = j;
		}
		if (mini != i)
		{
			tmp = *i;
			*i = *mini;
			*mini = tmp;
		}
	}
};













