#pragma once

/*
	����һ������������arr��һ��������s
	Ѱ����С�������飬ʹ������ĺ�>=s
	������С������ĳ���


	ѭ����ֱ����߽絽��β���˳�
	��������ڵĺ�С��s�����ұ߽�δ����β�����ұ߽�����
	���򣬼�¼��С���䳤�ȣ���߽�����
*/

int minSumSubArrayLength(int* arr, int len, int s)
{
	int sum = 0;
	int left = 0;
	int right = 0;
	int minLen = len;

	while (right < len)
	{
		sum += arr[right];
		right++;
		
		while (sum >= s)
		{
			minLen = minLen > right - left ? right - left : minLen;
			sum -= arr[left];
			left++;
		}
	}

	return minLen;
}