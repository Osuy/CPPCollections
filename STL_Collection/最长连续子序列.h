#pragma once
#include <vector>
#include <unordered_set>
#include <unordered_map>
using std::vector;
using std::unordered_set;
using std::unordered_map;
/*
	����һ�����飬��������������
	�����е�Ԫ���������飬ÿ��Ԫ��֮���1����С����
	�������������е���󳤶�
*/

int longestConsecutive(const vector<int>& nums)
{

	//�������Ԫ�ز��뵽hash_set�У�ȡ���ظ�Ԫ�أ����������
	unordered_set<int> num_set;
	for (const int& num : nums)
	{
		num_set.insert(num);
	}

	int longestStreak = 0;

	for (const int& num : num_set)
	{
		//���num-1���ڣ���û��Ҫ��numΪ���ȥѰ������
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