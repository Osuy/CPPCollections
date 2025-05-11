#pragma once
#include <unordered_map>
#include <string>
using std::unordered_map;
using std::string;

int maxSubStringLengthWithTwoChar(const string& str)
{
	int left = 0;
	int right = 0;
	int count = 0;
	unordered_map<char, int> freq;

	while (right < str.length())
	{
		if (freq[str[right]] == 0)
			count++;
		freq[str[right]]++;
		right++;

		if (count > 2)
		{
			if (freq[str[left]] == 1)
				count--;
			freq[str[left]]--;
			left++;
		}
	}

	return right - left;
}

