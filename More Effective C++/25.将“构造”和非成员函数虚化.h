#pragma once
#include <memory>
#include <iostream>

/*
	将“构造”和非成员函数虚化

	此条款对 构造 施以引号，可知并非真的构造函数。
	虚化，也并非指真的虚函数化

	根据常识，构造函数不可以是虚函数，非成员更无法是虚函数

	此处的虚化指的是用虚函数来实现：构造对象和非成员函数使用多态
*/

class Base
{
public:
	// 用于构造对象的虚函数，特别地它复制自己
	virtual std::unique_ptr<Base> clone() const = 0;

	// 用于完整原本属于非成员函数的功能 的 虚函数 
	virtual void serialize(std::ostream&)const = 0;
};

class Derived1 : public Base
{
public:
	virtual std::unique_ptr<Base> clone()const override 
	{ 
		return std::make_unique<Derived1>(*this); /*copy constructor*/ 
	}

	virtual void serialize(std::ostream&)const override
	{
		// todo...
	}
};

class Derived2 : public Base
{
public:
	virtual std::unique_ptr<Base> clone()const override
	{
		return std::make_unique<Derived2>(*this); /*copy constructor*/
	}

	virtual void serialize(std::ostream&) const override
	{
		// todo...
	}
};

// 非成员 operator<< 的虚化：调用虚函数，使os能正常出现在 << 左边
// 如果直接使用成员 operator<<， 则os出现在 << 右边， 不符合规范
std::ostream& operator<<(std::ostream& os, const Base& obj)
{
	obj.serialize(os);
	return os;
}