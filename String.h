#pragma once
#include <string>

namespace study
{
	class String
	{
	private:
		char* m_data;
	public:
		String(const char* cstr = 0);
		String(const String& str);
		String& operator=(const String& str);
		~String();

		char* get_c_str()const { return m_data; }
	};

	String::String(const char* cstr)
	{
		if (cstr)
		{
			//strlen 可以知道char指针所指的字符串的长度
			//由于末尾有结束符\0，因此长度要加1
			m_data = new char[strlen(cstr) + 1];
			strcpy(m_data, cstr);
		}
		else
		{
			//[1]是为了统一在析构函数中以数组的形式析构
			m_data = new char[1];
			*m_data = '\0';
		}
	}

	//深拷贝：为指针成员申请内存并复制一份所指的数据
	String::String(const String& str)
	{
		m_data = new char[strlen(str.m_data) + 1];
		strcpy(m_data, str.m_data);
	}

	//拷贝赋值，先检测自我赋值，再释放自身，最后根据右值分配内存和复制
	String& String::operator=(const String& str)
	{
		//防止自我赋值
		if (this == &str)
			return *this;

		//赋值需要先释放自身的指针，再根据同类分配内存，最后拷贝
		//如果没有检测自我赋值，会释放自己，于是就无法成功分配内存
		delete[] m_data;
		m_data = new char[strlen(str.m_data) + 1];
		strcpy(m_data, str.m_data);

		return *this;
	}

	String::~String()
	{
		//[]可以完整地释放掉数组的内存
		delete[] m_data;
	}


}