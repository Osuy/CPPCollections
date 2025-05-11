#pragma once
#include <vector>
#include <stack>
#include <algorithm>
using std::vector;
using std::stack;
/*
	���ڸ��������飬��������Ի�����������
	ʹ�õ���ջ����ΪҪ�������Σ����Զ�Ԫ��i�������Ԫ������С������α����ƣ������󣬾��β�������
	���Ҫ�󵥵�ջΪ��������
	��ѭ���������Ԫ����ջ������ջ��Ԫ���ǵ�������ġ������Ԫ�ر�ջ��С������Ҫ��ջ
	��ѭ������ջ��ֱ��ջ������Ԫ��С��
		   ÿ��ջһ��Ԫ�أ���˵����Ԫ�����ܻ����ľ��α���Ԫ�ؽضϣ����ǿ��Լ������
		   ���ǻ�Ҫ�жϳ�ջ���ջ�Ƿ�Ϊ�գ����ջ�ǿյģ�Ҫô��ջ���ǵ�һ��Ԫ�أ�Ҫô����ǰ�沿����С����
		   ������������ζ������쵽������ײ�����ˣ����ο�Ϊi����Ϊ��ջԪ��
		   ���ջ�ǿգ�˵��������ǰ���죬��Ϊi-��ջ������
		��ѭ��ÿ����һ�����Σ��ͺ��Ѽ�¼�������αȽϣ���ȡ�ϴ��ߡ�

	����ѭ������ʱ��ջ��Ϊ�գ�������һ���������汣���������β���ĵ������֡�
	���ǻ���Ҫһ��ѭ������ջ��������Σ������������һ��

	����ջ��Ӧ��
	��ϸ�ڷ��棬��Ҫ�ú������εļ��㡣�����Ǽ����ջʱ�ľ���
*/

int GetMaxRetangleOfArray(const vector<int>& arr)
{
	int len = arr.size();
	int res = 0;
	int height, weight;
	stack<int> st;


	for (int i = 0; i < len; i++)
	{
		while (!st.empty() && arr[st.top()] > arr[i])
		{
			height = arr[st.top()];
			weight = i;
			if (!st.empty())//���ջ��Ϊ�գ��������ǰ����
			{
				weight = i - st.top() - 1;
			}

			res = max(res, height * weight);
		}
		st.push(i);
	}

	while (!st.empty())
	{
		height = arr[st.top()];
		weight = len;
		if (!st.empty())//���ջ��Ϊ�գ��������ǰ����
		{
			weight = len - st.top() - 1;
		}

		res = max(res, height * weight);
	}

	return res;
}