#pragma once
#include<vector>
using std::vector;
/*
	对于自然数1~n
	求其能组成的不同的二叉搜索树的个数


    对于n个自然数，首先考虑其孩子
    取i作为根节点，则1~i-1为其左子树，i+1~n为其右子树
    如果保存了之前求i-1和n-i的结果
    则对于以i为根的搜索树的不同的个数为res[i-1]*res[n-i]；
    于是外循环逐渐将总数量i递增，从2到n
    内循环则确定根节点j为哪个，从1到n
*/

int NumTrees(int n) {
    vector<int> res(n + 1, 0);
    res.cbegin();
    res[0] = 1;
    res[1] = 1;
    for (int i = 2; i <= n; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            res[i] += res[j - 1] * res[i - j];
        }
    }
    return res[n];
}