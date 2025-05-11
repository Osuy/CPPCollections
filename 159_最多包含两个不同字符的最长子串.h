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

int maxSubStringLengthWithTwoChar2(const string& str)
{
	int freq[26];
	for (int i = 0; i < 26; ++i)
		freq[i] = 0;

	int left = 0,
		right = 0,
		count = 0;

	while (right < str.length())
	{
		if (freq[str[right++] - 'a']++ == 0)
			count++;

		if (count > 2)
		{
			if (--freq[str[left++] - 'a'] == 0)
				count--;
		}

	}
	return right - left;
}

