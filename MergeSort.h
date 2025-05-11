#pragma once
#include <iostream>
#include <vector>
using std::vector;
using std::cout;
using std::endl;

void _merge(vector<int>& v, size_t l, size_t mid, size_t r)
{
	vector<int> tmp;
	for (size_t i = l; i < r; ++i)
		tmp.push_back(v[i]);

	size_t lstart = 0, rstart = mid - l;
	size_t lend = mid - l, rend = r - l;

	for (size_t i = l; i < r; ++i)
	{
		if (lstart >= lend)
			v[i] = tmp[rstart++];
		else if (rstart >= rend)
			v[i] = tmp[lstart++];
		else
		{
			if (tmp[lstart] < tmp[rstart])
				v[i] = tmp[lstart++];
			else
				v[i] = tmp[rstart++];
		}
	}	

}

void MergeSort(vector<int>& v, size_t l, size_t r)
{
	if (l + 1 < r)
	{
		size_t mid = (l + r) / 2;
		MergeSort(v, l, mid);
		MergeSort(v, mid, r);
		_merge(v, l, mid, r);
	}
}

void test_merge_sort()
{
	cout << "merge sort" << endl;
	vector<int> v = { 3,8,1,4,7,23,9,64,8,12,0 };
	for (auto el : v)
	{
		cout << el << ' ';
	}
	cout << endl;
	MergeSort(v, 0, v.size());

	for (auto el : v)
	{
		cout << el << ' ';
	}
}