#pragma once

namespace case33
{
/*
	子类的函数会覆盖父类的同名函数，包括重载版本，包括虚函数
	（把虚函数当作普通函数使用才受影响，多态时是根据虚函数表调用，不受影响）
	如果不希望覆盖，可以使用using Base::XXX;
	如果不希望继承，可以使用private继承
	
	同名意味着子类可能想另起炉灶，此时父类、以及更遥远的祖先们的所有其他重载如果依然存在，是一种累赘
	因此C++将所有父类以上的同名函数都覆盖，若想使用，需要加上Base::
	继承父类的名称是public的基本规则，如果使用的同名函数覆盖，那意味着失去了这部分继承
	它们变成了某种非public的成员
	如果要让它们重新变回public，可以在子类的public部分里加上using Base::XXX;
	这样所有来自父类的同名函数又能使用了

	public继承会继承父类的名称，而子类的同名函数又会覆盖父类的所有同名函数

*/
class Base
{
public:
	virtual void mf1() = 0;
	virtual void mf1(int);
	virtual void mf2();
	void mf3();
	void mf3(double);
};

class Derived :public Base
{
public:
	using Base::mf1;
	using Base::mf3;
	virtual void mf1();
	void mf3();
	void mf4()
	{
		// 自身命名空间没有mf2，查找父类命名空间的mf2
		mf2();
	}
};

void test()
{
	Derived obj;
	obj.mf1(0); // 如果不在Derived的public区域内加上 using Base::mf1; 此行错误
	obj.mf3(0); // 如果不在Derived的public区域内加上 using Base::mf3; 此行错误
}
}