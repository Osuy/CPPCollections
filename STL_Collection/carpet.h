#pragma once
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;

/*
	n=n-1+2*n-2
	
*/

int carpet(int n)
{
	vector<int> v(n + 1, 0);
	v[1] = 1;
	v[2] = 2;
	v[3] = 4;
	for (int i = 2; i < n; i++)
	{
		v[i] = v[i - 1] + v[i - 2] + v[i - 3];
	}
	if (n == 1 )return 1;
	else
	{
		return (carpet(n - 1) + carpet(n - 2) + carpet(n - 3)) % 10007;
	}
	//2*4+1*2+1*1+1*2
	/*if (n == 2)return 2;
	if (n == 3)return 4;
	if (n > 3)
	{

	}*/
}
struct QuestionSet
{
	int E;
	int M;
	int H;
	QuestionSet() :E(-1), M(0), H(2) {}
};

bool FindQuestionSet(int* q, QuestionSet& qs)
{
	if (q[qs.E + 1] > 0 && qs.E + 1 < 2)
	{
		qs.E++;
	}
	else if (q[qs.M + 1] > 0 && qs.M + 1 < 4)
	{
		qs.M++;
	}
	else if (q[qs.M + 2] > 0 && qs.M + 2 < 4)
	{
		qs.M += 2;
	}
	else if (q[qs.H + 1] > 0 && qs.H + 1 < 5)
	{
		qs.H++;
	}
	else
		return false;

	return true;
}

void GetQuestionSet(int* q, const QuestionSet& qs)
{
	q[qs.E]--;
	q[qs.H]--;
	q[qs.M]--;
}

void ReleasQuestionSet(int* q, const QuestionSet& qs)
{
	q[qs.E]++;
	q[qs.H]++;
	q[qs.M]++;
}

int SimulatedMatch(int* q)
{
	int length = 0;
	int top = 0;
	QuestionSet qs;
	vector< QuestionSet> qArray;
	if (!FindQuestionSet(q, qs))return length;
	else
	{
		GetQuestionSet(q, qs);
		qArray.push_back(qs);
	}


	while (top >= 0)
	{
		qs = qArray[top];
		if (FindQuestionSet(q, qs))
		{
			qArray.push_back(qs);
			top++;
			GetQuestionSet(q, qs);
			if (qArray.size() > length)length = qArray.size();
		}
		else
		{
			top--;
			//qs.Reset();
		}
	}
	return length;
}

void resolution()
{
	int n,i,j;
	cin >> n;
	vector<string>v;
	string str;
	for (i = 0; i < n; i++)
	{
		cin >> str;
		v.push_back(str);
	}

	int a, b;
	char c[8];
	for (i = 0; i < n; i++)
	{
		strcpy_s(c, str.c_str());
		for (j = 0; j < 8; j += 3)
		{
			a = v[i][j] - '0';
			b = v[i][j + 1] - '0';

			switch (i)
			{
			case 0:
				if (a > 2 || (a = 2 && b > 3))a = 0;
					break;
			default:
				if (a >= 6)a = 0;
				break;
			}

			c[j] = a;
		}
		cout << c << endl;
	}

}

void resolution2()
{
	vector<vector<int>> v;
	int n,m;
	int k;
	bool find;
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		v.emplace(v.end());
		cin >> m;
		for (int j = 0; j < m; j++)
		{
			cin >> k;
			find = false;
			vector<int>::const_iterator it = v[i].begin();
			while (it != v[i].end())
			{
				if (k == *it)
				{
					find = true;
					break;
				}
				it++;
			}

			if(!find)
				v[i].push_back(k);
		}
	}

	for (int i = 0; i < n; i++)
	{
		for (vector<int>::const_iterator j = v[i].cend(); j != v[i].begin(); j--)
			cout << *j <<" ";
		cout << endl;
	}
}
