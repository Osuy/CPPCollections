#pragma once
#include <cstdlib>
#include <vector>
#include <iostream>
using std::vector;
using std::cout;
using std::end;

int Place(int* Column, int index)
{
	int i;
	for (i = 1; i < index; ++i)
	{
		int Column_differ = abs(Column[index] - Column[i]);
		int Row_differ = abs(index - i);
		if (Column[i] == Column[index] || Column_differ == Row_differ)
			return 0;
	}
	return 1;
}

int N_Queue(int n)
{
	int Column_Num[20];

	int index = 1;
	int i;
	int answer_num = 0;

	Column_Num[index] = 0;

	while (index > 0)
	{
		Column_Num[index]++;
		while (Column_Num[index] <= n && !Place(Column_Num, index))
			Column_Num[index]++;

		if (Column_Num[index] <= n)
		{
			if (index == n)
			{
				answer_num++;
				//如果要输出答案，在此处
			}
			else
			{
				index++;
				Column_Num[index] = 0;
			}
		}
		else
			index--;
	}

	return answer_num;
}

bool Place(const vector<int>& v, int index)
{
	for (int i = 1;i < index; ++i)
	{
		int diff_h = index - i;
		int diff_v = abs(v[i] - v[index]);

		if (diff_v == 0 || diff_v == diff_h)
			return false;
	}
	return true;
}

//递归式回溯
void backtracking(int k, int n, int& ans, vector<int>& v)
{
	if (k == n)
	{
		++ans;
		return;
	}

	for (int i = 1; i < n; ++i)
	{
		v[k] = i;
		if (Place(v, k))
		{
			backtracking(k + 1, n, ans, v);
		}
	}
}

int N_Queue2(int n)
{
	int ans = 0;
	vector<int> v(n + 1, 0);
	backtracking(1, n + 1, ans, v);

	return ans;
}

void test_Nqueen(int n)
{
	cout << N_Queue(n) << endl << N_Queue2(n);
}