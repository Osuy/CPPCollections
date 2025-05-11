#pragma once
#include<vector>
using std::vector;
/*
	������Ȼ��1~n
	��������ɵĲ�ͬ�Ķ����������ĸ���


    ����n����Ȼ�������ȿ����亢��
    ȡi��Ϊ���ڵ㣬��1~i-1Ϊ����������i+1~nΪ��������
    ���������֮ǰ��i-1��n-i�Ľ��
    �������iΪ�����������Ĳ�ͬ�ĸ���Ϊres[i-1]*res[n-i]��
    ������ѭ���𽥽�������i��������2��n
    ��ѭ����ȷ�����ڵ�jΪ�ĸ�����1��n
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