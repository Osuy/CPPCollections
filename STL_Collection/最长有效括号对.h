#pragma once
#include <string>
#include <vector>
using std::vector;
using std::string;

/*
	给出只包含'('和')'的字符串，求括号表达式的最大长度
	括号表达式：
	()(())√
	())())×

	用dp记录以每一个字符为结尾的括号表达式的长度。
	显然，有效的括号表达式不以'('结尾。
	于是对于字符i，若为'('，则dp[i]=0
	若为')'，则考虑i-1：
		若字符i-1为'('，则dp[i] = dp[i-2]+2 //（一个有效的括号表达式后面跟着一对括号）
		若字符i-1为')'，若考虑字符i-1-dp[i-1]   //（以i-1为结尾的括号表达式的头部的前一个字符）
			若为')'，不匹配，则dp[i] = 0。由于初始化时为0，所以不做事
			若为'('，则dp[i] = 2 + dp[i-1] + dp[i-1-dp[i-1]-1] //更前的表达式 + 前一个字符 + dp[i-1] + 字符i
			
*/

int GetMaxBracketsString(const string& s)
{
	int res = 0;
	vector<int> dp(s.length(), 0);

	for (int i = 0; i < s.length(); ++i)
	{
		if (s[i] == ')')
		{
			if (s[i - 1] == '(')
				dp[i] = dp[i - 2] + 2;
			else if (i - dp[i - 1] > 0 && s[i - 1 - dp[i - 1]] == '(')
				dp[i] = 2 + dp[i - 1] + ((i - dp[i - 1]) >= 2 ? dp[i - dp[i - 1] - 2] : 0);

			res = max(res, dp[i]);
		}
	}
}
int CountBracketsExpression(int n)
{
	if (n == 0)return 0;
	return __CountBracketsExpression(n);
}


int __CountBracketsExpression(int n)
{
	if (n == 1||n==0)return 1;

	int res = 0;
	for (int i = 0; i < n; ++i)
	{
		res += CountBracketsExpression(i) * CountBracketsExpression(n - 1 - i);
	}

	return res;
}