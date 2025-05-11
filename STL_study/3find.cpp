/*
摘自SGI<stl_algo.h>
根据参数给出的区间和value，在容器中寻找与value系统的对象
返回指向结果的迭代器*/
namespace Self {
	template <class InputIterator, class T>
	InputIterator find(InputIterator first,
		InputIterator last,
		const T& value) {
		while (first != last && *first != value)
			++first;
		return first;
	}
}

#include<vector>
#include<list>
#include<deque>
#include<algorithm>
#include<iostream>
using namespace std;

void test() {
	const int arraySize = 7;
	int ia[arraySize] = { 0,1,2,3,4,5,6 };

	vector<int>ivect(ia, ia + arraySize);
	list<int>ilist(ia, ia + arraySize);
	deque<int>ideque(ia, ia + arraySize);

	vector<int>::iterator it1 = Self::find(ivect.begin(), ivect.end(), 4);
	if (it1 == ivect.end())
		cout << "4 not found" << endl;
	else
		cout << "4 found" << *it1<<endl;

	list<int>::iterator it2 = Self::find(ilist.begin(), ilist.end(), 4);
	if (it2 == ilist.end())
		cout << "4 not found" << endl;
	else
		cout << "4 found" << *it2 << endl;

	deque<int>::iterator it3 = Self::find(ideque.begin(), ideque.end(), 4);
	if (it3 == ideque.end())
		cout << "4 not found" << endl;
	else
		cout << "4 found" << *it2 << endl;
}