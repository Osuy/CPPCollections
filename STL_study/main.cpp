#include <vector>
#include <list>
#include <iostream>

using namespace std;

int main(){
	
	{//vector的使用
		vector<int> v1(9, 9);
		for (vector<int>::iterator i = v1.begin(), i != v1.end(); i++)
		{
			cout << *i << "  ";
		}
		vector<int> v2;
		//assign，对vector重新分配，会清除之前的内容
		v2.
		v2.assign(v1.begin(), v1.end());//用区间分配

	}

	{


	}
}
