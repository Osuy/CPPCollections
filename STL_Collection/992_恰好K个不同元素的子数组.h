#include<vector>
using std::vector;
/*
	对于一个正整数组和正整数K
	存在子数组（仅原数组的头尾删去0个以上的元素），其恰好有K个不同的元素
	返回这样的子数组存在的个数

	题干是恰好包含K个不同元素的子数组
	如果是最多包含K个不同元素，非常好解
	但是转念一想
	恰好K个不同元素 = 最多K个不同元素 - 最多(K-1)个不同元素
	因此题目也还是很简单

	非常重要的点是，求最多包含K个元素的子数组，首先找到包含K个元素的子数组，再求它的所有组合的个数
	但是，求出这个子数组的所有组合，可能会与下一个区间的所有组合重复计数
	因此需要固定住右边界。因为下一个区间的右边界必然是不同的。
	那么，需要求的是以右边界为数组末端的子数组的个数，显然数目=right-left
*/



int subArrayWithMaxKDistinct(int* arr, int len,int K)
{
	//双指针，左闭右开，right指向下一个未遍历的元素
	int left = 0;
	int right = 0;
	//记录不同元素的个数
	int count = 0;
	//记录元素出现的频度
	vector<int> freq(len, 0);
	//记录子数组的个数
	int res = 0;

	while (left < right)
	{
		if (freq[arr[right]] == 0)
		{
			count++;
		}
		freq[arr[right]]++;
		right++;

		//当count <= K时退出，满足了最多包含K个元素的子数组
		while (count > K)
		{
			freq[arr[left]]--;
			if (freq[arr[left]] == 0)
			{
				count--;
			}
			left++;
		}

		//此时left和right所圈定的区间包含K个不同元素
		//那么子数组的个数是所有组合
		//但是如果计算所有组合，对于下一个区间
		//可能会重复，因此只计算固定左边界的子数组
		//即索引之差
		res += right - left;
	}
	return res;
}
int subArrayWithKDistinct(int* arr, int len, int K)
{
	return subArrayWithMaxKDistinct(arr, len, K) - subArrayWithMaxKDistinct(arr, len, K - 1);
}