#pragma once
/*
	dp[i][j] ��ʾi~j���ִ��Ƿ񹹳ɻ��Ĵ�
	���str[i]==str[j]����dp[i][j] = dp[i+1][j-1]
		����dp[i][j]=false

	���⣬���j-i<2��˵���ִ�Ϊ1���ַ��������ַ���һ���ַ�Ϊtrue�������ַ�Ϊstr[i]==str[j]��true��false

*/