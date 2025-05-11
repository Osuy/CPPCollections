#pragma once
#include<string>
using std::string;

//ʹ����ֻ����Сд��ĸ���ַ���
int lengthOfLongestSubstring(string s) {

    int left = 0;
    int right = 0;
    int freq[26];
    for (int i = 0; i < 26; i++)
        freq[i] = 0;

    while (right < s.length())
    {
        freq[s[right] - 'a']++;

        if (freq[s[right] - 'a'] != 1)
        {
            freq[s[left] - 'a']--;
            left++;
        }
        right++;
    }
    return right - left;
}