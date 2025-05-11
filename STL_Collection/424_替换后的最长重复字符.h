#pragma once
#include <string>
using std::string;

/*
	对于一个字符串，任意替换其中的字符为其他字符
	使出现只包含一个字符的子串，称为重复字符
	求替换K次的情况下，重复字符的最大长度

	设置滑动窗口，起始都在左
	右边界向右移，记录所有出现的字符串的频度
	如果最大频度 >= 窗口长度 — K，说明可以替换为只有一个字符的子串，于是记录长度
	如果最大频度 < 窗口长度 — K，说明无论怎么替换，都不能替换为只有一个字符
	于是左边界右移，然后减去频度
*/

int MaxRepeatcharSubStringLenght(const string& str, int K)
{
	int left = 0;
	int right = 0;
	int len = str.length();
	int maxCount = 0;
	int freq[26];
	for (int i = 0; i < 26; i++)
		freq[i] = 0;

	while (right < len)
	{
		freq[str[right] - 'A']++;
		if (freq[str[right] - 'A'] > maxCount)
			maxCount = freq[str[right] - 'A'];
		right++;

		//maxCount只变大不变小，因此left右移，但是不需要重新计算最多重复元素的次数
		//因为找到一个更大的窗口，就没必要缩小窗口。毕竟求的是最长重复串
		if (right - left > maxCount + K)
		{
			freq[str[left] - 'A']--;
			left++;
		}
	}
	//虽然right遍历到len时，可能找不到更大的重复子串，但是窗口的长度不变滑动到遍历结束
	//窗口只会在找到更到的重复子串时变大，因此退出循环后right-left就是窗口长度，也就是最大重复子串的长度
	return right - left;
}

int MaxRepeatcharSubStringLenght2(const string& str, int K)
{
	int left = 0,
		right = 0,
		len = str.length(),
		max_freq = 0;

	int freq[26];
	for (int i = 0; i < len; ++i)
		freq[i] = 0;

	while (right < len)
	{
		if (++freq[str[right++] - 'a'] > max_freq)
			max_freq = freq[str[right] - 'a'];

		if (max_freq < len - k)
		{
			--freq[str[left++] - 'a'];
		}
	}
	return right - left;
}
