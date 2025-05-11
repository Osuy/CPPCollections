#include <iostream>
#include <string>
using namespace std;

/*
在C中，字符串是以字符数组的形式实现的，
数组名是一个指针，所以，C中的字符串本质上是char *p
而C++中的字符串，string类封装了一个char *p
string的构造函数
string()----创建空字符串
string(const char *s)----使用一个字符串初始化
string(const string& str)----拷贝构造函数，使用一个string对象初始化
string(int n,char c)----使用一串重复的字符初始化，长度为n


*/

//构造
void test01()
{
	string s1;
	string s2("abcde");
	string s3(s2);
	string s4(5, 'a');

	cout << "s2:" << s2 << endl
		<< "s3:" << s3 << endl
		<< "s4:" << s4 << endl;
}

//赋值
void test02()
{
	string s1;
	string s2;
	string s3;
	string s4;
	string s5;
	string s6;
	string s7;
	s1 = "hello world";
	s2 = s1;
	s3 = 'a';
	s4.assign("hello world");
	s5.assign(s4);
	s6.assign(s4, 5);
	s7.assign(5, 'a');

	cout << "s1:" << s1 << endl
		<< "s2:" << s2 << endl
		<< "s3:" << s3 << endl
		<< "s4:" << s4 << endl
		<< "s5:" << s5 << endl
		<< "s6:" << s6 << endl
		<< "s7:" << s7 << endl;
}

//连接字符串
void test03()
{
	string s = "de";
	string s1 = "abd";
	string s2 = "abd";
	string s3 = "abd";
	string s4 = "abd";
	string s5 = "abd";
	string s6 = "abd";
	string s7 = "abd";

	s1 += "de";
	(s2 += 'd' )+='e';
	s3 += s;
	s4.append("de");
	s5.append("defgh", 2);
	s6.append(s);
	s7.append(s, 0, 2);//从一个string的第0开始到第2的字符串连接到s7

	cout << "s1:" << s1 << endl
		<< "s2:" << s2 << endl
		<< "s3:" << s3 << endl
		<< "s4:" << s4 << endl
		<< "s5:" << s5 << endl
		<< "s6:" << s6 << endl
		<< "s7:" << s7 << endl;
}

//查找和替换
void test04()
{
	string str = "hello world";
	string s = "llo";
	string s2 = "110";
	//正序查找
	cout << str.find(s) << endl
		<< str.find("llo") << endl
		<< str.find("llo", 1, 8) << endl
		<< str.find('l') << endl
		//逆序查找
		<< str.rfind(s) << endl
		<< str.rfind("llo") << endl
		<< str.rfind("llo", 1, 8) << endl
		<< str.rfind('l') << endl
		//替换
		<< str.replace(2, 4, s2) << endl
		<< str.replace(2, 4, "110") << endl;
}


//比较
//存取
//插入
//删除

int main()
{
	//test01();
	//test02();
	//test03();
	test04();
	return 0;
}
