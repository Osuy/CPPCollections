#pragma once
#include <iostream>
#include <vector>
using std::vector;
using std::cout;
using std::endl;

size_t patition(vector<int>& v, int l, int r)
{
	int tmp = v[l];
	while (l < r)
	{
		while (l < r && v[r] >= tmp)--r;
		v[l] = v[r];
		
		while (l < r && v[l] <= tmp)++l;
		v[r] = v[l];
	}
	v[l] = tmp;
	return l;
}

void QuickSort(vector<int>& v, int l, int r)
{
	if (l < r)
	{
		size_t mid = patition(v, l, r);
		QuickSort(v, l, mid - 1);
		QuickSort(v, mid + 1, r);
	}
}

void test_quick_sort()
{
	cout << "quick sort" << endl;
	vector<int> v = { 3,8,1,4,7,23,9,64,8,12,0 };
	for (auto el : v)
	{
		cout << el << ' ';
	}
	cout << endl;
	QuickSort(v, 0, v.size() - 1);

	for (auto el : v)
	{
		cout << el << ' ';
	}
}