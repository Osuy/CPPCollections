#include <iostream>
#include <string>
#include <list>
using namespace std;


/*list容器中提供了强大的排序函数L.sort()
默认提供从小到大的排序功能
可以提供一个函数名做参数，实现更复杂的排序
函数为bool函数，返回想要比较大小的值的表达式
*/
class Person
{
public:
	Person(string name, int age, int height)
	{
		m_name = name;
		m_age = age;
		m_height = height;
	}
	string m_name;
	int m_age;
	int m_height;
};

bool comparePerson(Person& p1, Person& p2)
{
	//按年龄升序
	if (p1.m_age == p2.m_age)
	{
		return p1.m_height > p2.m_height;
	}
	else
	{
		return p1.m_age < p2.m_age;
	}
	
}

void test01()
{
	list<Person>L;

	Person p1("刘备", 35, 175);
	Person p2("曹操", 45, 180);
	Person p3("孙权", 40, 170);
	Person p4("赵云", 25, 190);
	Person p5("张飞", 35, 160);
	Person p6("关羽", 35, 200);
	L.push_back(p1);
	L.push_back(p2);
	L.push_back(p3);
	L.push_back(p4);
	L.push_back(p5);
	L.push_back(p6);

	for (list<Person>::iterator it = L.begin(); it != L.end(); it++)
	{
		cout << "姓名：" <<(*it).m_name<< " 年龄：" << it->m_age << " 身高：" << it->m_height << endl;
	}

	cout <<endl<< "排序后的效果" << endl<<endl;

	L.sort(comparePerson);

	for (list<Person>::iterator it = L.begin(); it != L.end(); it++)
	{
		cout << "姓名：" << (*it).m_name << " 年龄：" << it->m_age << " 身高：" << it->m_height << endl;
	}
}

int main()
{
	test01();
	return 0;
}

