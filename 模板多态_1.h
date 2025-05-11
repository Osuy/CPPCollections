#pragma once
#include <iostream>
using namespace std;

template<class T>
class printer
{
public:
	void operator()(T& t)
	{
		cout << "default: ";
		t.say();
	}
};

template<>
class printer<int>
{
public:
	void operator()(int& t)
	{
		cout << "specialize: ";
		cout << "num is " << t << endl;
	}
};

template<class T>
class printer<T*>
{
public:
	void operator()(T* t)
	{
		cout << "specialize: ";
		t->say();
	}
};

template<class T>
void printfunc(T& t)
{
	cout << "default: ";
	t.say();
};

//int特化
template<>
void printfunc<int>(int& t)
{
	cout << "specialize: ";
	cout << "num is " << t << endl;
};


//函数模板的重载
template<class T>
void printfunc(T* t)
{
	cout << "override: ";
	t->say();
};


class test01
{
public:
	void say()
	{
		cout << "test01 say" << endl;
	}
};

class test02
{
public:
	void say()
	{
		cout << "test02 say" << endl;
	}
};

void test_1()
{
	int a = 3;
	test01 t1;
	test02 t2;
	test01* t3 = new test01;

	printfunc(t1);
	printfunc(t2);
	printfunc(t3);
	printfunc(a);

	cout << endl;

	printer<test01> p1;
	printer<test02> p2;
	printer<test01*> p3;
	printer<int> p4;

	p1(t1);
	p2(t2);
	p3(t3);
	p4(a);

	//int a = 5;
	//printer<int>(a);
}