#pragma once
#include <string>
/*
	考虑以下类定义
*/
namespace case31
{

class Date {};
class Address {};
class Person
{
public:
	Person(const std::string& name, const Date& birth, const Address& addr);
	std::string name()const { return _name; }
	Date birthday()const { return _birthday; }
	Address address()const { return _address; }
private:
	std::string _name;
	Date _birthday;
	Address _address;
};

/*
	因为Person的定义直接使用Date和Address作为bits的一部分
	因此必须得到它们的定义
	它依赖std::string、Data、Address
	而它们的定义要么在本文件内，要么递归地在#include ...的文件内
	除开std::string作为标准库，它定义不会被修改
	一旦Date、Address或者它们三者依赖的头文件被修改
	那么Person将被重新编译，并可能因为错误的修改而导致一片报错

	C++不能支持仅凭声明式就去创建对象，因为任何创建对象的行为，必然需要申请对应大小的内存，无论是在堆上还是在栈上
	而一个类的声明式或者其内部成员仅有声明式，是不能确定类的大小的
	（因此会经常看见提供类的声明，但是成员变量使用类指针，无论类是什么，指针的大小都是确定的，因此可以确定类的大小）
	（Java和其他支持声明式创建对象的语言就是因为拿到的对象不是本身而只是对象的引用）

	函数的声明，不要求参数有定义，即便是参数传值pass by value
	因为函数的声明还未到真正使用的时候，一旦需要使用函数，就要有函数的定义，那时才是需要参数定义的时候

	依赖包含的关键在与包含具有传递性，A包含B，B包含C，那么修改C时，A就会被牵连
	而解决包含依赖的关键在于断开包含链

	解决编译依赖有两种方法：
		1.pimpl 内部只有另一个类的指针，功能交给另一个类实现，而自身只负责转调
			使用者只需依赖外部类，而功能的修改只发生在内部类上，只要接口不发生改变，外部就无需重新编译
		2.interface 只有虚函数，没有变量的基类
			使用者使用的是基类指针，但是指针指向可能是子类对象。
			修改子类，外部无需重新编译

	方法二的启示：并非需要多态的情况才使用继承。仅仅只是为了接口与实现分离、降低耦合，也可以对单一类进行抽象

	pimpl的Person：
*/

#include <string>	// 标准库的依赖可以无视
#include <memory>	// shared_ptr

class PersonImpl;	// Person的实现类，由于只持有他的指针，只需提供声明，去除了编译依赖
class Date;			// Date和Address，也只需要声明
class Address;

class Person
{
public:
	Person(const std::string& name ,const Date& birth,const Address& addr);
	std::string name()const;
	Date birthday()const;
	Address address()const;
private:
	std::shared_ptr<PersonImpl> pImpl;
};

// Person.cpp
Person(const std::string& name, const Date& birth, const Address& addr)
	: pImpl{ std::make_shared<PersonImpl>(name, birth, addr) }
{
}

// PersonImpl.h
// 获取Data和Address的定义，直接定义或include
class PersonImpl
{
public:
	PersonImpl(const std::string& name, const Date& birth, const Address& addr);
	std::string name()const { return _name; }
	Date birthday()const { return _birthday; }
	Address address()const { return _address; }
private:
	std::string _name;
	Date _birthday;
	Address _address;
};
	
/*
	Date和Address修改，只会牵扯到PersonImpl
	Person不会被牵扯，因此，任何包含Person头文件的其他文件也不会被牵扯

	以简单的话讲A包含B， D包含C，
	而B由于持有的是D的指针，所以声明不依赖D，但是定义依赖D
	因此可以在定义式中再去包含D

	Handle class 引出了三个可以极大改善程序依赖关系的做法
		1.如果使用引用或指针可以完成的事，就不要使用对象本身
		2.只提供类地声明式而不include
		3.将声明式和定义式分开到两个文件中



	interface class，即虚基类，只提供一个虚析构和若干纯虚接口
	由于虚基类没有实现，所以不需要任何类的定义式，只需提供声明
	而真正实现交由子类去重写
	于此，使用Person时，则要以指针或引用的方式，同时，为了能够获得对象
	Person常常会提供一个static函数，返回一个Person指针，而具体返回和种对象
	则可以由参数决定并让工厂去实现

	以简单的话讲A包含B， D包含C，
	D是B的子类，B不包含C，而是由子类D包含C
	A使用B的指针和引用，会通过虚函数只需真正的实现



	无论Handle class还是interface class，代价是需要消耗更多的内存，以及至少两次转调时间
	但即便不使用这两种，难保Person本身就已经使用了虚函数
	放弃它们一如放弃虚函数一样，使用C++却自断双臂
	最小化的编译依赖能帮助你更快地编译，更快地定位到编译错误


	总结
		尽量使用声明而非定义
		头文件仅有声明式，定义式交给cpp去实现
*/
}