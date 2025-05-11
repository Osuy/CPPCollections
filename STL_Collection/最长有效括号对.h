#pragma once
#include <string>
#include <vector>
using std::vector;
using std::string;

/*
	����ֻ����'('��')'���ַ����������ű��ʽ����󳤶�
	���ű��ʽ��
	()(())��
	())())��

	��dp��¼��ÿһ���ַ�Ϊ��β�����ű��ʽ�ĳ��ȡ�
	��Ȼ����Ч�����ű��ʽ����'('��β��
	���Ƕ����ַ�i����Ϊ'('����dp[i]=0
	��Ϊ')'������i-1��
		���ַ�i-1Ϊ'('����dp[i] = dp[i-2]+2 //��һ����Ч�����ű��ʽ�������һ�����ţ�
		���ַ�i-1Ϊ')'���������ַ�i-1-dp[i-1]   //����i-1Ϊ��β�����ű��ʽ��ͷ����ǰһ���ַ���
			��Ϊ')'����ƥ�䣬��dp[i] = 0�����ڳ�ʼ��ʱΪ0�����Բ�����
			��Ϊ'('����dp[i] = 2 + dp[i-1] + dp[i-1-dp[i-1]-1] //��ǰ�ı��ʽ + ǰһ���ַ� + dp[i-1] + �ַ�i
			
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