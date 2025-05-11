#pragma once
#include <vector>
#include <string>
#include <unordered_set>
using std::vector;
using std::string;
using std::unordered_set;

/*
	���β��
	���ڸ������ַ������ֵ䣨���ʵļ��ϣ�
	�ж��ַ����Ƿ����ֵ��еĵ������

	������ȱ����Ͷ�̬�滮���ɽ�


    ��̬�滮˼·��
        ��������s�����β���������ַ���һ�����ʣ�
        ����Ҫ��֤ǰ����Ӵ�Ҳ��һ���ɵ�����ɵĴ���
        �����������Ĳ�֡�
        ��iȡ����s��dp[i]����s0~1�Ƿ���Բ��

*/


bool IsWordCompose(string s, vector<string>& wordDict) {
    unordered_set<string> words;
    for (auto word : wordDict)
        words.insert(word);

    //����ĳ�����s.length+1��0Ϊ��ʼ��Ϊtrue����ʾ������ַ���Ϊ��Ҳ��true
    vector<bool> dp(s.length() + 1, false);
    dp[0] = true;

    //������Ӳ����ǣ�ֱ��i���ӵ�����s
    //����ÿ��i���������Ƿ���һ��ǰ�벿�ֵ��Ӵ��ͺ�벿�ֵĵ������
    //����ǣ���dp[i]=true
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