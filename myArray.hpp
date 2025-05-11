#pragma once
#include <iostream>
using namespace std;

template<class T>
class MyArray
{
public:
	MyArray(int capacity)
	{
		cout << "youcan" << endl;
		this->m_Capacity = capacity;
		this->m_Size = 0;
		this->pAddress = new T[this->m_Capacity];//在堆区开辟内存
	}
	MyArray(const MyArray& arr)//传入一个同类型的对象，所以是拷贝构造函数
	{
		cout << "kaobei" << endl;
		this->m_Capacity = arr.m_Capacity;
		this->m_Size = arr.m_Size;

		//this->pAddress = arr.pAddress;浅拷贝问题
		this->pAddress = new T[arr.m_Capacity];
		for (int i = 0; i < this->m_Size; i++)
		{
			this->pAddress[i] = arr.pAddress[i];
		}
	}
	//=号重载
	MyArray& operator=(const MyArray& arr)
	{
		cout << "chongzai" << endl;
		if (this->pAddress != NULL)//将之前的内容清楚
		{
			delete[] this->pAddress;
			this->pAddress = NULL;
			this->m_Capacity = 0;
			this->m_Size = 0;
		}

		this->m_Capacity = arr.m_Capacity;
		this->m_Size = arr.m_Size;
		this->pAddress = new T[arr.m_Capacity];
		for (int i = 0; i < this->m_Size; i++)
		{
			this->pAddress[i] = arr.pAddress[i];
		}
		return *this;
	}

	~MyArray()
	{
		cout << "xigou" << endl;
		if (this->pAddress != NULL)
		{
			delete[] this->pAddress;
			this->pAddress = NULL;
		}
	}
	private:
		T *pAddress;
		int m_Capacity;
		int m_Size;
};