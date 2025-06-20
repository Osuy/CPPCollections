#pragma once
#include <memory>
#include <iostream>

/*
	�������족�ͷǳ�Ա�����黯

	������� ���� ʩ�����ţ���֪������Ĺ��캯����
	�黯��Ҳ����ָ����麯����

	���ݳ�ʶ�����캯�����������麯�����ǳ�Ա���޷����麯��

	�˴����黯ָ�������麯����ʵ�֣��������ͷǳ�Ա����ʹ�ö�̬
*/

class Base
{
public:
	// ���ڹ��������麯�����ر���������Լ�
	virtual std::unique_ptr<Base> clone() const = 0;

	// ��������ԭ�����ڷǳ�Ա�����Ĺ��� �� �麯�� 
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

// �ǳ�Ա operator<< ���黯�������麯����ʹos������������ << ���
// ���ֱ��ʹ�ó�Ա operator<<�� ��os������ << �ұߣ� �����Ϲ淶
std::ostream& operator<<(std::ostream& os, const Base& obj)
{
	obj.serialize(os);
	return os;
}