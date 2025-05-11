#pragma once

/*
	KMP算法的核心在于寻找模串的回溯索引
	对于模串t的每个字符t[i]，存在一个正整数k，使得
		t[0]t[1]...t[k-1] = t[i-k]t[i-k+1]...t[i-1]，即
		t[i]前的k长度的字符串与模串头开始的k长度字符串相等。
	每个t[i]，对应的k可能存在多个，需取最大的一个。
*/

void GetNext(char* t, int* next)
{
	int index=0, pre=-1;
	next[0] = -1;
	while(index)
}