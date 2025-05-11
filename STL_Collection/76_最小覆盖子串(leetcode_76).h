#pragma once
#include<string>
#include<unordered_map>
using std::string;
using std::unordered_map;
/*
	��������s��ģ��t
	��s�а�������t����ĸ����С�Ӵ�
	�ظ�����ĸ��Ҫ������ͬ�ĸ���

	ѭ���壺left�Ƶ�sβ��ֹͣ
	����ұ߽�δ����β���Ҽ���С��t���ȣ����ұ߽����ƣ�˵�����仹δ����t��ȫ���ַ���
		����������Ԫ��Ƶ��--������������������++
	������߽����ƣ������Ѿ�����t��ȫ���ַ���������߿�������Ч�ַ��������߽����ƣ�
		���ƺ��뿪��Ԫ��Ƶ��++�����==0�������--
	�������count==t�ĳ��ȣ�����right-left��ֵ����һ�μ�¼�����䳤��len��С��
	��lenȡ��С������


	s�д��ڶ���Ӵ����������t��ȫ���ַ�
	���������ұ߽縺����������ַ�����߽縺����С���䣬
	�����������ȫ���ַ�����߽�������ƣ���count--
	�Ա���һ��ѭ�����ұ߽�������ж�Ϊ��
	����������������ҵ�һ���Ӵ��󣬼���Ѱ����һ���Ӵ�
	ÿ�ҵ�һ�����ͱȽ����䳤�ȣ������֮ǰ�ĸ��̣������len������̵����䣬
	������res�ַ���ȡ��ֵ
*/

string minSubContainString(const string& s, const string& t)
{
	unordered_map<char, int> freq;
	for (int i = 0; i < t.length(); i++)
		freq[t[i]]++;

	int left = 0;
	int right = 0;
	int count = 0;
	string res;
	int len = s.length();

	while (right < s.length())
	{
		if (freq[s[right]] > 0)
			count++;
		freq[s[right]]--;
		right++;

		while (count >= t.length())
		{
			if (right - left < len)
			{
				len = right - left;
				res = s.substr(left, right - left);
			}

			if (freq[s[left]]==0)
				count--;

			freq[s[left]]++;
			left++;
		}
	}
	return res;
}

string minSubContainString2(const string& s, const string& t)
{
	int freq[26];
	for (int i = 0; i < 26; ++i)
		freq[i] = 0;

	for (int i = 0; i < t.length(); ++i)
		freq[t[i] - 'a']++;


	int left = 0,
		right = 0,
		count = 0,
		len = t.length();

	string res;


	while (right < s.length())
	{
		if (freq[s[right++]]-- > 0)++count;
		
		while (count >= t.length())
		{
			if (right - left < len)
			{
				len = right - left;
				res = s.substr(left, right);
			}

			if (freq[s[left++]]-- == 0)--count;
		}

	}
	return 
}