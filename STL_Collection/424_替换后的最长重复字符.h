#pragma once
#include <string>
using std::string;

/*
	����һ���ַ����������滻���е��ַ�Ϊ�����ַ�
	ʹ����ֻ����һ���ַ����Ӵ�����Ϊ�ظ��ַ�
	���滻K�ε�����£��ظ��ַ�����󳤶�

	���û������ڣ���ʼ������
	�ұ߽������ƣ���¼���г��ֵ��ַ�����Ƶ��
	������Ƶ�� >= ���ڳ��� �� K��˵�������滻Ϊֻ��һ���ַ����Ӵ������Ǽ�¼����
	������Ƶ�� < ���ڳ��� �� K��˵��������ô�滻���������滻Ϊֻ��һ���ַ�
	������߽����ƣ�Ȼ���ȥƵ��
*/

int MaxRepeatcharSubStringLenght(const string& str, int K)
{
	int left = 0;
	int right = 0;
	int len = str.length();
	int maxCount = 0;
	int freq[26];
	for (int i = 0; i < 26; i++)
		freq[i] = 0;

	while (right < len)
	{
		freq[str[right] - 'A']++;
		if (freq[str[right] - 'A'] > maxCount)
			maxCount = freq[str[right] - 'A'];
		right++;

		//maxCountֻ��󲻱�С�����left���ƣ����ǲ���Ҫ���¼�������ظ�Ԫ�صĴ���
		//��Ϊ�ҵ�һ������Ĵ��ڣ���û��Ҫ��С���ڡ��Ͼ��������ظ���
		if (right - left > maxCount + K)
		{
			freq[str[left] - 'A']--;
			left++;
		}
	}
	//��Ȼright������lenʱ�������Ҳ���������ظ��Ӵ������Ǵ��ڵĳ��Ȳ��们������������
	//����ֻ�����ҵ��������ظ��Ӵ�ʱ�������˳�ѭ����right-left���Ǵ��ڳ��ȣ�Ҳ��������ظ��Ӵ��ĳ���
	return right - left;
}

int MaxRepeatcharSubStringLenght2(const string& str, int K)
{
	int left = 0,
		right = 0,
		len = str.length(),
		max_freq = 0;

	int freq[26];
	for (int i = 0; i < len; ++i)
		freq[i] = 0;

	while (right < len)
	{
		if (++freq[str[right++] - 'a'] > max_freq)
			max_freq = freq[str[right] - 'a'];

		if (max_freq < len - k)
		{
			--freq[str[left++] - 'a'];
		}
	}
	return right - left;
}
