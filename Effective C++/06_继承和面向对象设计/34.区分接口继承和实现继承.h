#pragma once

namespace case34
{
/*
	pure virtual 函数就是为了让类只继承接口而不继承实现
	virtual 函数则是接口和缺省实现皆被继承
	非virtual函数则是强制继承接口和实现

	pure virtual 函数 强制子类必须自己实现但是仍然可以提供定义
*/

class Base
{
public:
	virtual void pv_func() = 0
	{
		// implementation of pure virtual function
	}
};

/*
	pure virtual 函数的实现由于总是会被子类覆盖，因此它无法用于多态
	唯一可以被调用到的方式是显式指定命名空间来调用Base::pv_func();
	它的意义在于提供一种介于“只继承接口”和“继承接口和缺省实现”之间的继承水平

	这是因为：
		1.pure virtual 函数必须让子类实现，而当大多数子类的实现都相同时，会膨胀代码，有缺省实现会更好
		2.virtual函数的父类实现会被隐式继承，如果有一个子类不希望继承，它只能显式override
			而如果忘记，或者没有足够明确的注释和手册导致不知道要override，可能会引发运行时错误
			（宁愿编译器错误，不要运行时错误）
			（不要指望人的记忆和细心）

	对于问题1，为了避免代码膨胀，既然大多数子类的实现会相同，那完全可以同一个non-virtual函数来提供缺省实现
	对于问题2，为了不隐式继承，使用pure virtual函数，并让所有子类调用哪个non-virtual的缺省实现
*/

class Base2
{
public:
	virtual void pv_func() = 0;
	void default_impl_func() {}
};

// 希望使用缺省实现
class Derived :public Base2
{
public:
	virtual void pv_func() override { default_impl_func(); }
};

// 不希望使用缺省实现
class Derived2 :public Base2
{
public:
	virtual void pv_func() override { /*独特的实现*/ }
};

/*
	为了回避隐式继承virtual函数的缺省实现，而使用另一个non-virual函数负责缺省实现
	再把virtual函数改为pure virtual函数
	但是由于多了一个函数，而且这个函数名必然和接口的名字含义解决，容易使使用者混淆
	因此可以给pure virtual函数实现定义。这个函数既不会被错误调用
	也不会产生新的函数名
	所有子类的virtual函数里对non-virtual函数的调用改为对Base::pv_func的调用即可
*/

// Base的定义见本文开头
// 它比Base少了non-virtual函数，多了pure virtual函数的定义
// 希望使用缺省实现
class Derived3 :public Base
{
public:
	virtual void pv_func() override { Base::pv_func();  /*使用缺省实现*/
	}
};

// 不希望使用缺省实现
class Derived4 :public Base
{
public:
	virtual void pv_func() override { /*独特的实现*/ }
};
}