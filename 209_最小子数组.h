#pragma once

/*
	对于一个正整数数组arr和一个正整数s
	寻找最小的子数组，使子数组的和>=s
	返回最小子数组的长度


	循环，直到左边界到达尾部退出
	如果区间内的和小于s，且右边界未到达尾部，右边界右移
	否则，记录最小区间长度，左边界右移
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