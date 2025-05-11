#include <vector>
#include <list>
#include <set>
#include <stack>
#include <string>
#include <iostream>
#include <map>
#include <unordered_map>
#include <xmemory>
#include "BubbleSort.h"
#include "SelectionSort.h"
#include "InsertionSort.h"
#include "QuickSort.h"
#include "MergeSort.h"
#include "./carpet.h"
#include "smart_ptr.h"
using namespace std;

/*3种遍历
常规for
for+迭代器遍历
	for (vector<int>::iterator i = v1.begin();i!=v1.end();i++)
	{
		cout << *i << "  ";
	}

foreach
	for (auto i : v1)
	{
		cout << i << "  ";
	}*/

//vector的使用
	/*{
		vector<int> v1(9, 9);

		

		vector<int> v2;
		//assign，对vector重新分配，会清除之前的内容
		v2.assign(v1.begin(), v1.end());//用区间分配
		for (auto i : v2)
		{
			cout << i << "  ";
		}
		cout <<endl;
		v2.assign(6, 2);//用数量和值分配
		for (auto i : v2)
		{
			cout << i << "  ";
		}
		cout << endl;
		v2.assign(v1.data(), v1.data() + 2);//用数组和数量分配。.data()返回vector第一个元素的地址，相当于数组名指针arr
		for (auto i : v2)
		{
			cout << i << "  ";
		}
	}*/

int Mg[10][10] =
{
	{1,1,1,1,1,1,1,1,1,1},
	{1,0,0,1,0,0,0,1,0,1},
	{1,0,0,1,0,0,0,1,0,1},
	{1,0,0,0,0,1,1,0,0,1},
	{1,0,1,1,1,0,0,0,0,1},
	{1,0,0,0,1,0,0,0,0,1},
	{1,0,1,0,0,0,1,0,0,1},
	{1,0,1,1,1,0,1,1,0,1},
	{1,1,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1}
};

struct Box
{
	int i;
	int j;
	int di;

};

class TestClass
{
public:
	TestClass() { id = 0; };
	TestClass(int i) :id(i) { cout << "ctor. id = " << id << endl; }
	~TestClass() { cout << "dtor. id = " << id << endl; }
private:
	int id;
};

bool MgPath(int xi, int yi, int xe, int ye)
{
	Box Path[50], e;
	int i, j, di, i1, j1, k;
	bool find;
	stack<Box> st;
	e.i = xi; e.j = yi; e.di = -1;
	st.push(e);
	Mg[xi][yi] = -1;
	while (!st.empty())
	{
		e = st.top();
		i = e.i, j = e.j; di = e.di;
		if (i == xe && j == ye)
		{
			printf("road:\n");
			k = 0;
			while (!st.empty())
			{
				e = st.top(); st.pop();
				Path[k++] = e;
			}
			while (k > 0)
			{
				--k;
				printf("\t(%d,%d)", Path[k].i, Path[k].j);
				if ((k + 2) % 5 == 0)
				{
					printf("\n");
				}
			}
			printf("\n");
			return true;
		}
		find = false;
		while (di < 4 && !find)
		{
			di++; 
			switch (di) 
			{
			case 0:i1 = i - 1; j1 = j; break;
			case 1:i1 = i; j1 = j + 1; break;
			case 2:i1 = i + 1; j1 = j; break;
			case 3:i1 = i; j1 = j - 1; break;
			}
			if (Mg[i1][j1] == 0)find = true;
		}
		if (find)
		{
			st.top().di = di;
			e.i = i1, e.j = j1, e.di = -1;
			st.push(e);
			Mg[i1][j1] = -1;
		}
		else
		{
			e = st.top();
			st.pop();
			Mg[e.i][e.j] = 0;
		}
	}
	return false;
}

int f2(int a)
{
	return ++a;
}

int f3(int a)
{
	return --a;
}

int f1(int, int f2(int));
int (*pr)(int, int f2(int)) = f1;

void f4(int& a)
{
	a++;
}

int& testFun(void (*pr)(int&))
{
	static int num;
	pr(num);
	return num;
}

int __CountBracketsExpression(int n)
{
	if (n == 1 || n == 0)return 1;

	int res = 0;
	for (int i = 0; i < n; ++i)
	{
		res += __CountBracketsExpression(i) * __CountBracketsExpression(n - 1 - i);
	}

	return res;
}
int CountBracketsExpression(int n)
{
	if (n == 0)return 0;
	return __CountBracketsExpression(n);
}




int main()
{
	
	use_shared_ptr();
	return 0;
	
}

int f1(int a, int f(int))
{
	return f(a);
}

