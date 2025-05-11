#pragma once
#include <string>
/*
	���������ඨ��
*/
namespace case31
{

class Date {};
class Address {};
class Person
{
public:
	Person(const std::string& name, const Date& birth, const Address& addr);
	std::string name()const { return _name; }
	Date birthday()const { return _birthday; }
	Address address()const { return _address; }
private:
	std::string _name;
	Date _birthday;
	Address _address;
};

/*
	��ΪPerson�Ķ���ֱ��ʹ��Date��Address��Ϊbits��һ����
	��˱���õ����ǵĶ���
	������std::string��Data��Address
	�����ǵĶ���Ҫô�ڱ��ļ��ڣ�Ҫô�ݹ����#include ...���ļ���
	����std::string��Ϊ��׼�⣬�����岻�ᱻ�޸�
	һ��Date��Address������������������ͷ�ļ����޸�
	��ôPerson�������±��룬��������Ϊ������޸Ķ�����һƬ����

	C++����֧�ֽ�ƾ����ʽ��ȥ����������Ϊ�κδ����������Ϊ����Ȼ��Ҫ�����Ӧ��С���ڴ棬�������ڶ��ϻ�����ջ��
	��һ���������ʽ�������ڲ���Ա��������ʽ���ǲ���ȷ����Ĵ�С��
	����˻ᾭ�������ṩ������������ǳ�Ա����ʹ����ָ�룬��������ʲô��ָ��Ĵ�С����ȷ���ģ���˿���ȷ����Ĵ�С��
	��Java������֧������ʽ������������Ծ�����Ϊ�õ��Ķ����Ǳ����ֻ�Ƕ�������ã�

	��������������Ҫ������ж��壬�����ǲ�����ֵpass by value
	��Ϊ������������δ������ʹ�õ�ʱ��һ����Ҫʹ�ú�������Ҫ�к����Ķ��壬��ʱ������Ҫ���������ʱ��

	���������Ĺؼ�����������д����ԣ�A����B��B����C����ô�޸�Cʱ��A�ͻᱻǣ��
	��������������Ĺؼ����ڶϿ�������

	����������������ַ�����
		1.pimpl �ڲ�ֻ����һ�����ָ�룬���ܽ�����һ����ʵ�֣�������ֻ����ת��
			ʹ����ֻ�������ⲿ�࣬�����ܵ��޸�ֻ�������ڲ����ϣ�ֻҪ�ӿڲ������ı䣬�ⲿ���������±���
		2.interface ֻ���麯����û�б����Ļ���
			ʹ����ʹ�õ��ǻ���ָ�룬����ָ��ָ��������������
			�޸����࣬�ⲿ�������±���

	����������ʾ��������Ҫ��̬�������ʹ�ü̳С�����ֻ��Ϊ�˽ӿ���ʵ�ַ��롢������ϣ�Ҳ���ԶԵ�һ����г���

	pimpl��Person��
*/

#include <string>	// ��׼���������������
#include <memory>	// shared_ptr

class PersonImpl;	// Person��ʵ���࣬����ֻ��������ָ�룬ֻ���ṩ������ȥ���˱�������
class Date;			// Date��Address��Ҳֻ��Ҫ����
class Address;

class Person
{
public:
	Person(const std::string& name ,const Date& birth,const Address& addr);
	std::string name()const;
	Date birthday()const;
	Address address()const;
private:
	std::shared_ptr<PersonImpl> pImpl;
};

// Person.cpp
Person(const std::string& name, const Date& birth, const Address& addr)
	: pImpl{ std::make_shared<PersonImpl>(name, birth, addr) }
{
}

// PersonImpl.h
// ��ȡData��Address�Ķ��壬ֱ�Ӷ����include
class PersonImpl
{
public:
	PersonImpl(const std::string& name, const Date& birth, const Address& addr);
	std::string name()const { return _name; }
	Date birthday()const { return _birthday; }
	Address address()const { return _address; }
private:
	std::string _name;
	Date _birthday;
	Address _address;
};
	
/*
	Date��Address�޸ģ�ֻ��ǣ����PersonImpl
	Person���ᱻǣ������ˣ��κΰ���Personͷ�ļ��������ļ�Ҳ���ᱻǣ��

	�Լ򵥵Ļ���A����B�� D����C��
	��B���ڳ��е���D��ָ�룬��������������D�����Ƕ�������D
	��˿����ڶ���ʽ����ȥ����D

	Handle class �������������Լ�����Ƴ���������ϵ������
		1.���ʹ�����û�ָ�������ɵ��£��Ͳ�Ҫʹ�ö�����
		2.ֻ�ṩ�������ʽ����include
		3.������ʽ�Ͷ���ʽ�ֿ��������ļ���



	interface class��������ֻ࣬�ṩһ�������������ɴ���ӿ�
	���������û��ʵ�֣����Բ���Ҫ�κ���Ķ���ʽ��ֻ���ṩ����
	������ʵ�ֽ�������ȥ��д
	�ڴˣ�ʹ��Personʱ����Ҫ��ָ������õķ�ʽ��ͬʱ��Ϊ���ܹ���ö���
	Person�������ṩһ��static����������һ��Personָ�룬�����巵�غ��ֶ���
	������ɲ����������ù���ȥʵ��

	�Լ򵥵Ļ���A����B�� D����C��
	D��B�����࣬B������C������������D����C
	Aʹ��B��ָ������ã���ͨ���麯��ֻ��������ʵ��



	����Handle class����interface class����������Ҫ���ĸ�����ڴ棬�Լ���������ת��ʱ��
	�����㲻ʹ�������֣��ѱ�Person������Ѿ�ʹ�����麯��
	��������һ������麯��һ����ʹ��C++ȴ�Զ�˫��
	��С���ı��������ܰ��������ر��룬����ض�λ���������


	�ܽ�
		����ʹ���������Ƕ���
		ͷ�ļ���������ʽ������ʽ����cppȥʵ��
*/
}