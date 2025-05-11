#pragma once
#include <iostream>
using namespace std;

int maxLength = 0;
int length = 0;
int question[5];

void findNextSet(int* qset)
{
	if (qset[2] < 4)
		qset[2]++;
	else if (qset[1] < 3)
		qset[1]++;

	else 
		qset[0]++;
}

bool CanSet(int* qset)
{
	for (int i = 0; i < 3; i++)
	{
		if (question[qset[i]] == 0)
			return false;
	}

	return true;
}

void BackTrace()
{
	int qset[3] = {0, 1, 3};
	int i;

	if (length > maxLength)maxLength = length;

	while (qset[0]<2)
	{
		if (CanSet(qset))
		{
			for (i = 0; i < 3; i++)
				question[qset[i]]--;
			length++;

			BackTrace();

			for (i = 0; i < 3; i++)
				question[qset[i]]++;
			length--;
		}
		
		findNextSet(qset);
	}	
}


void Resolution()
{
	cin >> question[0] >> question[1] >> question[2] >> question[3] >> question[4];
}




//#include <iostream>
//#include <string>
//using namespace std;
//
//int main()
//{
//	string s;
//	char t[500000];
//	cin >> s;
//	cin >> t;
//	int tlen;
//	int i, j;
//	for (int j = 499999; j >= 0; j--)
//		if (t[j] != '\0')
//		{
//			tlen = j + 1;
//			break;
//		}
//	i = s.length() - 1;
//	for (int j = tlen - 1; j >= 0 && i >= 0; j--)
//	{
//		if (t[j] == s[i])i--;
//	}
//
//	if (i >= 0)
//		cout << "false";
//	else
//		cout << "true";
//
//	return 0;
//}

//#include<iostream>
//#include <string>
//#include <vector>
//using namespace std;
//
//struct dis
//{
//	int ld;
//	int rd;
//
//	dis() :ld(0), rd(0) {}
//	dis(int l, int r) :ld(l), rd(r) {}
//	int getdist() { return ld > rd ? rd : ld; }
//}
//
//int main()
//{
//	vector<int> arr;
//	string s;
//	getline(cin, s);
//	for (int i = 0; i < s.length(); i++)
//	{
//		if (s[i] >= '0' && s[i] <= '9')
//			arr.push_back(s[i] - '0');
//	}
//
//	int dist = 0;
//	vector<dis> res(arr.size());
//	if (arr[0] == 1)res[0] = dis();
//	else
//	{
//		for (dist = 1; dist < arr.size(); dist++)
//			if (arr[dist])break;
//		res[0] = dis(0, dist);
//	}
//	for (int i = 1; i < arr.size(); i++)
//	{
//		if (arr[i] == 0)
//		{
//			res[i] = dis(res[i - 1].ld + 1, res[i - 1].rd - 1);
//		}
//		else
//		{
//			res[i] = dis();
//		}
//	}
//
//	int maxDist = 0;
//	for (int i = 0; i < res.size(); i++)
//	{
//		if (maxDist < res[i].getdist())
//			maxDist = res[i].getdist();
//	}
//
//	cout << maxDist;
//
//}