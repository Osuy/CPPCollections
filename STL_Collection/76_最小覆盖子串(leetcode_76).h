#pragma once
#include<string>
#include<unordered_map>
using std::string;
using std::unordered_map;
/*
	对于主串s和模串t
	求s中包含所有t的字母的最小子串
	重复的字母需要包含相同的个数

	循环体：left移到s尾部停止
	如果右边界未到达尾部且计数小于t长度，则右边界右移（说明区间还未包含t的全部字符）
		右移遇到的元素频度--，如果还有余量则计数++
	否则左边界右移（区间已经包含t的全部字符，但最左边可能有无效字符，因此左边界右移）
		左移后，离开的元素频度++，如果==0，则计数--
	如果计数count==t的长度，并且right-left的值比上一次记录的区间长度len更小，
	则len取更小的区间


	s中存在多个子串，满足包含t的全部字符
	滑动窗口右边界负责包含所有字符，左边界负责缩小区间，
	当区间包含了全部字符后，左边界继续左移，并count--
	以便下一个循环，右边界的右移判断为真
	这样，区间就能在找到一个子串后，继续寻找下一个子串
	每找到一个，就比较区间长度，如果比之前的更短，则更新len保存更短的区间，
	并更新res字符串取新值
*/

string minSubContainString(const string& s, const string& t)
{
	unordered_map<char, int> freq;
	for (int i = 0; i < t.length(); i++)
		freq[t[i]]++;

	int left = 0;
	int right = 0;
	int count = 0;
	string res;
	int len = s.length();

	while (right < s.length())
	{
		if (freq[s[right]] > 0)
			count++;
		freq[s[right]]--;
		right++;

		while (count >= t.length())
		{
			if (right - left < len)
			{
				len = right - left;
				res = s.substr(left, right - left);
			}

			if (freq[s[left]]==0)
				count--;

			freq[s[left]]++;
			left++;
		}
	}
	return res;
}

string minSubContainString2(const string& s, const string& t)
{
	int freq[26];
	for (int i = 0; i < 26; ++i)
		freq[i] = 0;

	for (int i = 0; i < t.length(); ++i)
		freq[t[i] - 'a']++;


	int left = 0,
		right = 0,
		count = 0,
		len = t.length();

	string res;


	while (right < s.length())
	{
		if (freq[s[right++]]-- > 0)++count;
		
		while (count >= t.length())
		{
			if (right - left < len)
			{
				len = right - left;
				res = s.substr(left, right);
			}

			if (freq[s[left++]]-- == 0)--count;
		}

	}
	return 
}