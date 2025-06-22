#pragma once
#include "29.Reference Counting ���ü���.h"
/*
	��case 29�����ٵ�һ�����⣺
		�� const �汾�� operator[] ����������
	������Ϊ const [] �ͷ�const [] ��ƥ�䲢���ǻ��������ĵĶ�д������ȡ����String�����Ƿ�Ϊconst
	���һ����const String������õ�һ���� ��const [] Ҳ��һ���ᣬ��������û�з���д������ȴ��Ȼ�´��

	����� [] ���ص�ֻ��һ���ַ��Ĵ���Ϳɽ���������

	�����˼����CPTRҲ�����֣�
		Ϊ�˱�����������+����������ʱ������������ÿ��+�ķ�����һ������/�������
		��������ڲ�����һ���������Լ���һ���������
		�����+�Ľ��Ҳ��һ���������Ȼ����Ҫ = ��ֵ����ʽ���
		���Ǵ������ſ�ʼ���������Ľ�����ݹ�ش�ÿ���Ӵ��������ѽ�������
		Ȼ�󷵻ظ���һ��ݹ飬���ֵ
		Ҳ����˵�����ļ���ֻ�����ڸ�ֵǰ�����һ����ǰ�����+ֻ���ڽ����������ù�ϵ��

	ͬ����[]ȡ���ַ����е��ַ������ǲ���֪�����ڶ�����д����ôֱ�ӷ��ء����Ա�����ʹ�õ��ַ�����û�б�Ҫ
	���Է����ַ��Ĵ������ַ��ڱ�ʹ�ã�����д����ǰһ����������ȡ������ʱҲ��������ж��Ƿ�������
	��[]������ʱ��const�汾�ķ���const������const�汾���ط�const����
	��ʱ�ַ���δ��ʹ�ã�����ֻ�ǵõ��˴������㹻�ˡ�
	һ�ж�������������ʽת��char
	һ��д����������operator= ����Ȼ��һ����const ������Ϊconst�������޷����÷�const��Ա�ģ�


	Proxy˼�룺
		���һ���������ۺܴ󣬾��������������Ҫʱ�ű�����ִ��
		�����һЩ����������Ҫ������������������
		��ʹ�ô�������ֻ�ǽ��������ù�ϵ���Ա�����Ҫʱ����ִ�У������ٴ������
		����ķ�const��Ա(�磺=)����������ִ��
		��const��Ա(�磺��ʽת��)����Ǳ������

*/

class String
{
public:
	class CharProxy
	{
	public:
		CharProxy(String& str, int index);

		// ��const CharProxy ���ܵ���
		CharProxy& operator=(const CharProxy& rhs)
		{
			if (theString.value->IsShared())
			{
				theString.value = new StringValue{ theString.value->data };
			}

			theString[charIndex] = rhs;

			return *this;
		}
		CharProxy& operator=(char c)
		{
			if (theString.value->IsShared())
			{
				theString.value = new StringValue{ theString.value->data };
			}
			theString[charIndex] = c;
			return *this;
		}

		// const �ͷ� const CharProxy �������ڶ�ʱ��������ʽת��Ϊchar
		operator char()const { return theString[charIndex]; }
	private:
		String& theString; // CharProxy��������һ���Ѵ��ڵ��ַ����������ܶ���ʹ��
		int charIndex;
	};

	friend class CharProxy;

	// const [] ���� const CharProxy�����Բ��ܵ��� = 
	const CharProxy operator[](int index)const
	{
		return CharProxy(const_cast<String&>(*this), index);
	}

	// ��const [] ���ط� const CharProxy�����Զ�д
	CharProxy operator[](int index)
	{
		return CharProxy(*this, index);
	}

private:
	struct StringValue :public RCObject
	{
		char* data = nullptr;
		StringValue(const char* str = nullptr)
		{
			data = new char[strlen(str) + 1];
			strcpy(data, str);
		}
		StringValue(const StringValue& rhs)
		{
			data = new char[strlen(rhs.data) + 1];
			strcpy(data, rhs.data);
		}

		~StringValue() { delete[] data; }

		char  operator[](size_t index) const { return data[index]; }
		char& operator[](size_t index) { MarkUnshareable(); return data[index]; }
	};
	RCPtr<StringValue> value;
};