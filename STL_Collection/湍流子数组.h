#pragma once

/*
	湍流子数组
	对于数组arr，存在一个连续的子数组（原数组头尾去掉0或多个元素）
	其相邻元素之差必然符号相反，即元素的大小是按上下上下的顺序排列的
	则称这个数组为湍流子数组

	返回一个数组的最大湍流子数组的长度

	对于元素i，假若前方存在一个以i-1为结尾的湍流子数组，且i加入后仍然是湍流子数组
	则最大湍流子数组的长度+1，否则重置为1
	如果两元素相等，则重置为1，
	如果i大，则判断i-1是否小于i-2
	如果i小，则判断i-1是否大于i-2

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