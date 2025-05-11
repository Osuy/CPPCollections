//vector容器
//算法：for_eache
//迭代器：vector<int>::iterator
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

class GreaterNum
{
public:
	bool operator ()(int val) {
		return val > 2;
	}
};

bool GreaterNum2(int val) {
	return val > 2;
}



template<typename T>
void MyPrint(T val)
{
	cout << val << endl;
}

class Person
{
	friend ostream& operator<<(ostream& cout, Person &p);
	friend ostream& operator<<(ostream& cout, Person *p);

public:
	Person(string name,int age)
	{
		this->m_age = age;
		this->m_name = name;
	}

private:
	string m_name;
	int m_age;
};

ostream& operator<<(ostream& cout, Person &p)
{
	cout << "name:"<<p.m_name<<" age:"<<p.m_age<< endl;
	return cout;
}
ostream& operator<<(ostream& cout, Person *p)
{
	cout << "name:" << p->m_name << " age:" << p->m_age << endl;
	return cout;
}

//测试int数据类型
void test01()
{
	//创建整型vector容器
	vector<int> v;

	//向尾部插入数据整数
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);

	//通过迭代器访问容器中的数据
	vector<int>::iterator itBegin = v.begin();//指向容器中的第一个元素
	vector<int>::iterator itEnd = v.end();//指向容器中最后一个元素的下一个位置

	//while遍历
	while (itBegin != itEnd)
	{
		cout << *itBegin << endl;
		itBegin++;

	}

	//for遍历
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
	{
		cout << *it << endl;
	}

	//for_each遍历,for_each需要三个参数：起点，终点，和操作函数
	//for_each会把每个元素传入函数中执行
	for_each(v.begin(), v.end(), MyPrint<int>);
}

//测试自定义数据类型
void test02()
{
	vector<Person>v;
	Person p1("a", 1);
	Person p2("b", 2);
	Person p3("c", 3);
	Person p4("d", 4);
	Person p5("e", 5);
	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);
	v.push_back(p4);
	v.push_back(p5);

	for_each(v.begin(),v.end(),MyPrint<Person>);
}

//测试自定义数据类型指针
void test03()
{
	vector<Person*>v;

	Person p1("a", 1);
	Person p2("b", 2);
	Person p3("c", 3);
	Person p4("d", 4);
	v.push_back(&p1);
	v.push_back(&p2);
	v.push_back(&p3);
	v.push_back(&p4);

	for_each(v.begin(), v.end(), MyPrint<Person*>);

}

//容器嵌套容器
void test04()
{
	vector<vector<int>> v;
	vector<int> v1;
	vector<int> v2;
	vector<int> v3;
	vector<int> v4;
	for (int i = 0; i < 4; i++)
	{
		v1.push_back(i + 1);
		v2.push_back(i + 2);
		v3.push_back(i + 3);
		v4.push_back(i + 4);
	}
	v.push_back(v1);
	v.push_back(v2);
	v.push_back(v3);
	v.push_back(v4);

	for (vector<vector<int>>::iterator it = v.begin(); it != v.end(); it++)
	{
		for_each((*it).begin(), (*it).end(), MyPrint<int>);
	}
}

//测试仿函数和普通函数
void test05() {
	vector<int> v;
	for (int i = 0; i < 5; i++)
	{
		v.push_back(i);
	}

	for_each(v.begin(), v.end(), MyPrint<int>);
	vector<int>::iterator it = find_if(v.begin(), v.end(), GreaterNum());
	if (it == v.end()) {
		cout << "failed" << endl;
	}
	else {
		cout << "succeed" << endl;
	}
}

int main() {
	test05();
	return 0;
}