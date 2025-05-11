#pragma once
#include <thread>

/*
	单例模式
		唯一地为单例类创建一个实例
		在程序运行过程中，单例类始终只存在一个对象

	单例模式有两种实现方法
		1.懒汉式：在第一次请求时生成
		2.饿汉式：类内包含一个静态实例

	无论用哪一种实现方法，都要求：静态自身类成员、私有构造函数
*/

//懒汉式
//全局对象指针。第一次请求时才创建，然后返回地址或引用
//有线程不安全的问题
class Singleton1
{
private:
	static Singleton1* Instance;
	Singleton1() {}
	Singleton1(const Singleton1& other) {}
	Singleton1(const Singleton1&& other) {}
	~Singleton1() { delete Instance; }
public:
	static Singleton1* GetInstance_Ptr() 
	{
		if (!Instance)
			Instance = new Singleton1();
		return Instance;
	}

	//线程安全版，单检锁
	//对读操作互斥，代价大
	static Singleton1* GetInstance_Ptr()
	{
		//Lock l;
		if (!Instance)
			Instance = new Singleton1();
		return Instance;
	}

	//线程安全版，双检锁
	//
	static Singleton1* GetInstance_Ptr()
	{
		if (!Instance) 
		{
			//Lock l;
			if (!Instance)
				Instance = new Singleton1();
		}	
		return Instance;
	}

	static const Singleton1& GetInstance_Ref()
	{
		if (!Instance)
			Instance = new Singleton1();
		return *Instance;
	}
};

//饿汉式
//全局对象，程序开始即创建，需要时直接返回地址或引用
class Singleton2
{
private:
	static Singleton2 Instance;
	Singleton2() {}
public:
	static Singleton2* GetInstance_Ptr()
	{
		return &Instance;
	}

	static const Singleton2& GetInstance_Ref()
	{
		return Instance;
	}
};