#pragma once
/*
	dp[i][j] 表示i~j的字串是否构成回文串
	如果str[i]==str[j]，则dp[i][j] = dp[i+1][j-1]
		否则，dp[i][j]=false

	另外，如果j-i<2，说明字串为1个字符或两个字符，一个字符为true，两个字符为str[i]==str[j]？true：false

*/