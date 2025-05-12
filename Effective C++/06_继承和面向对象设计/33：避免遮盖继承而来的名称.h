#pragma once

namespace case33
{
/*
	����ĺ����Ḳ�Ǹ����ͬ���������������ذ汾�������麯��
	�����麯��������ͨ����ʹ�ò���Ӱ�죬��̬ʱ�Ǹ����麯������ã�����Ӱ�죩
	�����ϣ�����ǣ�����ʹ��using Base::XXX;
	�����ϣ���̳У�����ʹ��private�̳�
	
	ͬ����ζ���������������¯���ʱ���ࡢ�Լ���ңԶ�������ǵ������������������Ȼ���ڣ���һ����׸
	���C++�����и������ϵ�ͬ�����������ǣ�����ʹ�ã���Ҫ����Base::
	�̳и����������public�Ļ����������ʹ�õ�ͬ���������ǣ�����ζ��ʧȥ���ⲿ�ּ̳�
	���Ǳ����ĳ�ַ�public�ĳ�Ա
	���Ҫ���������±��public�������������public���������using Base::XXX;
	�����������Ը����ͬ����������ʹ����

	public�̳л�̳и�������ƣ��������ͬ�������ֻḲ�Ǹ��������ͬ������

*/
class Base
{
public:
	virtual void mf1() = 0;
	virtual void mf1(int);
	virtual void mf2();
	void mf3();
	void mf3(double);
};

class Derived :public Base
{
public:
	using Base::mf1;
	using Base::mf3;
	virtual void mf1();
	void mf3();
	void mf4()
	{
		// ���������ռ�û��mf2�����Ҹ��������ռ��mf2
		mf2();
	}
};

void test()
{
	Derived obj;
	obj.mf1(0); // �������Derived��public�����ڼ��� using Base::mf1; ���д���
	obj.mf3(0); // �������Derived��public�����ڼ��� using Base::mf3; ���д���
}
}