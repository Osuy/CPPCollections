#pragma once
#include <iostream>
using namespace std;

/*
	函数模板一般声明和使用

	重载
		函数模板可以重载。
		函数模板的重载可以是一个普通函数，也可以是另一个函数模板
		和普通函数重载一样，要求函数名，形参列表不同（返回值可以相同可以不同，形参一定要不同）
		典型的例子是，按值传递和引用传递的不同函数重载

	特化
		函数模板不能偏特化，只能全特化。
		因为偏特化的声明从不被认为是偏特化，而是被认为函数重载

		全特化
			template<>
			rettype funcname<type1,...>(type1 v1,...){}
		其中template<>内不写类型
		在函数名和括号之间加上<type1,...> ，表示特化为这种类型组合
		
		实际上函数模板全特化也几乎不使用，而是重载一个不同函数来处理特化的数据类型
		因为调用函数是考虑顺序是：普通函数、特化的模板、通用模板
		因此重载一个普通函数，不仅能实现函数模板全特化的作用，还比全特化版本要快
		如果同时存在普通函数和全特化函数，并且针对同样的类型，那将会优先调用普通函数
		但是如果仍然希望调用全特化函数模板，则可以在函数名后面加上模板参数<...>，里面可写可不写
		
		如果需要偏特化的灵活性，可以使用防函数和类模板偏特化

		
		
		总结：
			函数模板只能全特化，不能偏特化
			建议用重载普通函数类代替模板全特化
*/

class Speaker1
{
public:
	void say()
	{
		cout << "speaker1 talk" << endl;
	}
};

class Speaker2
{
public:
	void say()
	{
		cout << "speaker2 talk" << endl;
	}
};

//标准格式
template<class T1, class T2>
void func(T1 t1, T2 t2)
{
	cout << "函数模板" << endl;
	t1.say();
	t2.say();
	cout << endl;
};

//普通函数重载
void func(int t1, int t2)
{
	cout << "普通函数" << endl;
	cout << "speaker1 is: " << t1 << endl;
	cout << "speaker2 is: " << t2 << endl;
	cout << endl;
}

//全特化（不如重载为普通函数）
template<>
void func(int t1, int t2)
{
	cout << "全特化函数模板" << endl;
	cout << "speaker1 is: " << t1 << endl;
	cout << "speaker2 is: " << t2 << endl;
	cout << endl;
};