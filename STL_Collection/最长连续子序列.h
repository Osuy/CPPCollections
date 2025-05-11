#pragma once
#include <vector>
#include <unordered_set>
#include <unordered_map>
using std::vector;
using std::unordered_set;
using std::unordered_map;
/*
	对于一个数组，存在自序列满足
	子序列的元素来自数组，每个元素之间差1，从小到达
	返回连续子序列的最大长度
*/

int longestConsecutive(const vector<int>& nums)
{

	//将数组的元素插入到hash_set中，取出重复元素，并方便查找
	unordered_set<int> num_set;
	for (const int& num : nums)
	{
		num_set.insert(num);
	}

	int longestStreak = 0;

	for (const int& num : num_set)
	{
		//如果num-1存在，则没必要以num为起点去寻找序列
		if (!num_set.count(num - 1))
		{
			int currentNum = num;
			int currentStreak = 1;
			while (num_set.count(currentNum + 1))
			{
				currentNum++;
				currentStreak++;
			}

			longestStreak = currentStreak > longestStreak ? currentStreak : longestStreak;
		}
	}
	return longestStreak;
}