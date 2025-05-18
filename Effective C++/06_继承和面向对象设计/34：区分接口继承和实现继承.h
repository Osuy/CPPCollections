#pragma once

namespace case34
{
/*
	pure virtual ��������Ϊ������ֻ�̳нӿڶ����̳�ʵ��
	virtual �������ǽӿں�ȱʡʵ�ֽԱ��̳�
	��virtual��������ǿ�Ƽ̳нӿں�ʵ��

	pure virtual ���� ǿ����������Լ�ʵ�ֵ�����Ȼ�����ṩ����
*/

class Base
{
public:
	virtual void pv_func() = 0
	{
		// implementation of pure virtual function
	}
};

/*
	pure virtual ������ʵ���������ǻᱻ���า�ǣ�������޷����ڶ�̬
	Ψһ���Ա����õ��ķ�ʽ����ʽָ�������ռ�������Base::pv_func();
	�������������ṩһ�ֽ��ڡ�ֻ�̳нӿڡ��͡��̳нӿں�ȱʡʵ�֡�֮��ļ̳�ˮƽ

	������Ϊ��
		1.pure virtual ��������������ʵ�֣���������������ʵ�ֶ���ͬʱ�������ʹ��룬��ȱʡʵ�ֻ����
		2.virtual�����ĸ���ʵ�ֻᱻ��ʽ�̳У������һ�����಻ϣ���̳У���ֻ����ʽoverride
			��������ǣ�����û���㹻��ȷ��ע�ͺ��ֲᵼ�²�֪��Ҫoverride�����ܻ���������ʱ����
			����Ը���������󣬲�Ҫ����ʱ����
			����Ҫָ���˵ļ����ϸ�ģ�

	��������1��Ϊ�˱���������ͣ���Ȼ����������ʵ�ֻ���ͬ������ȫ����ͬһ��non-virtual�������ṩȱʡʵ��
	��������2��Ϊ�˲���ʽ�̳У�ʹ��pure virtual����������������������ĸ�non-virtual��ȱʡʵ��
*/

class Base2
{
public:
	virtual void pv_func() = 0;
	void default_impl_func() {}
};

// ϣ��ʹ��ȱʡʵ��
class Derived :public Base2
{
public:
	virtual void pv_func() override { default_impl_func(); }
};

// ��ϣ��ʹ��ȱʡʵ��
class Derived2 :public Base2
{
public:
	virtual void pv_func() override { /*���ص�ʵ��*/ }
};

/*
	Ϊ�˻ر���ʽ�̳�virtual������ȱʡʵ�֣���ʹ����һ��non-virual��������ȱʡʵ��
	�ٰ�virtual������Ϊpure virtual����
	�������ڶ���һ�����������������������Ȼ�ͽӿڵ����ֺ�����������ʹʹ���߻���
	��˿��Ը�pure virtual����ʵ�ֶ��塣��������Ȳ��ᱻ�������
	Ҳ��������µĺ�����
	���������virtual�������non-virtual�����ĵ��ø�Ϊ��Base::pv_func�ĵ��ü���
*/

// Base�Ķ�������Ŀ�ͷ
// ����Base����non-virtual����������pure virtual�����Ķ���
// ϣ��ʹ��ȱʡʵ��
class Derived3 :public Base
{
public:
	virtual void pv_func() override { Base::pv_func();  /*ʹ��ȱʡʵ��*/
	}
};

// ��ϣ��ʹ��ȱʡʵ��
class Derived4 :public Base
{
public:
	virtual void pv_func() override { /*���ص�ʵ��*/ }
};
}