#pragma once
#include <vector>
#include <string>
#include <unordered_set>
using std::vector;
using std::string;
using std::unordered_set;

/*
	单次拆分
	对于给定的字符串和字典（单词的集合）
	判断字符串是否由字典中的单词组成

	深度优先遍历和动态规划，可解


    动态规划思路：
        对于整个s，如果尾部的若干字符是一个单词，
        则需要保证前面的子串也是一个由单词组成的串。
        这就是子问题的拆分。
        用i取遍历s，dp[i]代表s0~1是否可以拆分

*/


bool IsWordCompose(string s, vector<string>& wordDict) {
    unordered_set<string> words;
    for (auto word : wordDict)
        words.insert(word);

    //数组的长度是s.length+1，0为初始化为true。表示传入的字符串为空也算true
    vector<bool> dp(s.length() + 1, false);
    dp[0] = true;

    //逐个增加并考虑，直到i增加到整个s
    //对于每个i，考虑它是否由一个前半部分的子串和后半部分的单词组成
    //如果是，则dp[i]=true
    for (int i = 1; i <= s.length(); ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            if (dp[j] && words.find(s.substr(j, i - j)) != words.end())
            {
                dp[i] = true;
                break;
            }
        }
    }

    return dp[s.length()];
}