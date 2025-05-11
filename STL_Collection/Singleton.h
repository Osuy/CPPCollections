#pragma once
#include <thread>

/*
	����ģʽ
		Ψһ��Ϊ�����ഴ��һ��ʵ��
		�ڳ������й����У�������ʼ��ֻ����һ������

	����ģʽ������ʵ�ַ���
		1.����ʽ���ڵ�һ������ʱ����
		2.����ʽ�����ڰ���һ����̬ʵ��

	��������һ��ʵ�ַ�������Ҫ�󣺾�̬�������Ա��˽�й��캯��
*/

//����ʽ
//ȫ�ֶ���ָ�롣��һ������ʱ�Ŵ�����Ȼ�󷵻ص�ַ������
//���̲߳���ȫ������
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

	//�̰߳�ȫ�棬������
	//�Զ��������⣬���۴�
	static Singleton1* GetInstance_Ptr()
	{
		//Lock l;
		if (!Instance)
			Instance = new Singleton1();
		return Instance;
	}

	//�̰߳�ȫ�棬˫����
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

//����ʽ
//ȫ�ֶ��󣬳���ʼ����������Ҫʱֱ�ӷ��ص�ַ������
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