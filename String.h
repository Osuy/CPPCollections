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
			//strlen ����֪��charָ����ָ���ַ����ĳ���
			//����ĩβ�н�����\0����˳���Ҫ��1
			m_data = new char[strlen(cstr) + 1];
			strcpy(m_data, cstr);
		}
		else
		{
			//[1]��Ϊ��ͳһ���������������������ʽ����
			m_data = new char[1];
			*m_data = '\0';
		}
	}

	//�����Ϊָ���Ա�����ڴ沢����һ����ָ������
	String::String(const String& str)
	{
		m_data = new char[strlen(str.m_data) + 1];
		strcpy(m_data, str.m_data);
	}

	//������ֵ���ȼ�����Ҹ�ֵ�����ͷ�������������ֵ�����ڴ�͸���
	String& String::operator=(const String& str)
	{
		//��ֹ���Ҹ�ֵ
		if (this == &str)
			return *this;

		//��ֵ��Ҫ���ͷ������ָ�룬�ٸ���ͬ������ڴ棬��󿽱�
		//���û�м�����Ҹ�ֵ�����ͷ��Լ������Ǿ��޷��ɹ������ڴ�
		delete[] m_data;
		m_data = new char[strlen(str.m_data) + 1];
		strcpy(m_data, str.m_data);

		return *this;
	}

	String::~String()
	{
		//[]�����������ͷŵ�������ڴ�
		delete[] m_data;
	}


}