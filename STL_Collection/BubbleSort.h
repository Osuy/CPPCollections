#pragma once

/*
	ð������
	��ѭ����0��len-2��
	ÿһ�������ѭ�����ͻ��δ���������ҵ�������
	����δ�������������棬Ȼ��������ǰ��С1

	��ѭ����0��len-i-1��
	�����Ƚϣ�ǰ�߽ϴ�ͻ������Ӷ�ʵ��������������������
*/	

void BubbleSort(int* arr, int len)
{
	for (int i = 0; i < len - 1; ++i)//��ѭ��ָ��len-1��
	{
		for (int j = 0; j < len - 1 - i; ++j)//��ѭ����0��len-1-i
		{
			if (arr[j] > arr[j + 1])//ÿ����һ��Ԫ�رȵ�ǰԪ�ش󣬾ͻ���
			{
				int temp = arr[j + 1];
				arr[j + 1]= arr[j];
				arr[j] = temp;
			}
		}
	}
}

void BubbleSort2(int* arr, int len)
{
	for (int i = 0; i < len - 1; i++)
	{
		for (int j = 0; i < len -1 - i; ++j)
		{
			if (arr[j] > arr[j + 1])
			{
				int tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
		}
	}
}


//��STL�������ķ����汾
template<class I>
void BubbleSort(I begin,I end)
{
	typename I::value_type tmp;
	I _end = end - 1;
	for (size_t i = 0; i < (size_t)(end - begin - 1); ++i)
	{
		for (I j = begin; j != _end; ++j)
		{
			if (*j > *(j + 1))
			{
				tmp = *j;
				*j = *(j + 1);
				*(j + 1) = tmp;
			}
		}
	}
}
