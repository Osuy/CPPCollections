#pragma once

namespace case35
{
/*
	1.缩小虚函数的职责粒度，声明为private，并由一个非虚函数调用它
	2.使用组合代替继承
	
	尽可能将一个虚函数所负责的功能缩小到原子级别
	以便让子类有更细的粒度去重新设计完整功能之中的某个不走

	考虑一个获取血量的函数
*/

class GameCharacter
{
public:
	virtual int getHealth1()const
	{
		return 0;
	}

	int getHealth2()const
	{
		// ... do some things 
		int value = doGetHealth();
		// ... do some things

		return value;
	}

private:
	virtual int doGetHealth()const
	{
		return 0;
	}
};

/*
	getHealth1 和 getHealth2 的不同之处在于
	getHealth1 全权负责了完整的获取生命值的功能，无论它将来会变得多复杂，子类若要重写，就要将完整的过程都考虑
		于是程序员在派生并重载时，往往得回过头参考父类的完整设计
	getHealth2 则一个非虚函数，内部调用了一个虚函数，同时自己还负责了一部分事务
		这部分事务也可能还包含有其他虚函数
		doGetHealth仅仅只负责了一部分职责，子类重写时，只需要针对这一部分职责去重写，更容易掌控
		
	值得注意的是doGetHealth声明为private，说明它并不希望被子类使用，仅仅只希望被子类重写
	这么做很好地向子类屏蔽了完整流程，也避免子类错误使用接口


	可能存在一种情况，一个类有10个虚函数分别负责不同的功能，而每个子类都可能会需要重写任意的数量
	并且可能func1存在重复，这时，针对重复的功能派生一个子类1完成共有的func1，再由他继续去派生
	然而对于子类1，仍然递归地存在func2有重复，于是又递归地子子类。。。
	这样毫无新意地增加继承树是非良构，可以考虑从基类功能地拆分入手
	把基类负责的10个功能拆分到子部件里，父类仅持有子部件的指针，
	子部件派生出特化的子类
	然后子类只需要持有不同的子部件即可组合出不同的功能
*/
}