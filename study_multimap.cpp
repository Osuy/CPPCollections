#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <functional>
#define CEHUA 0
#define MEISHU 1
#define YANFA 2
using namespace std;

class Worker
{
public:
	string m_name;
	int m_salary;
};

void createWorker(vector<Worker>& v)
{
	string nameSeed= "ABCDEFGHIJ";
	for (int i = 0; i < 10; i++)
	{
		Worker worker;
		worker.m_name = "员工";
		worker.m_name += nameSeed[i];
		worker.m_salary = rand() % 10001 + 10000;
		v.push_back(worker);
	}
}

void setGroup(vector<Worker>& v, multimap<int, Worker>& m)
{
	for (vector<Worker>::iterator it = v.begin(); it != v.end(); it++)
	{
		int deptID = rand() % 3;
		m.insert(make_pair(deptID, *it));
	}
}

void showWorkerByGroup(multimap<int, Worker>& m)
{
	int count = 0;
	multimap<int, Worker>::iterator it;
	for (int i = 0; i < 3; i++)
	{
		switch (i) {
		case 0:cout << "策划部门：" << endl; break;
		case 1:cout << "美术部门：" << endl; break;
		case 2:cout << "研发部门：" << endl; break;
		}
		count = m.count(i);
		it = m.find(i);
		for (int j = 0; j < count; j++,it++)
		{
			cout << "姓名:" <<it->second.m_name<< " 工资:" <<it->second.m_salary<< endl;
		}
		cout << "----------------------------" << endl;
	}
	logical_and
}

int main()
{
	//创建员工
	vector<Worker> vWorker;
	createWorker(vWorker);

	//员工分组
	multimap<int, Worker>mWorker;
	setGroup(vWorker,mWorker);
	
	//分组打印员工
	showWorkerByGroup(mWorker);
	
	//测试
	/*for (vector<Worker>::iterator it = vWorker.begin(); it != vWorker.end(); it++)
	{
		cout << "姓名:" << it->m_name << " 工资:" << it->m_salary << endl;
	}*/
}
