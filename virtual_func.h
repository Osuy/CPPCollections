#pragma once
#include "test_helper.h"
/*
	�麯����C++ʵ�ֶ�̬���ֶ�
	����Ϊ��
		����ָ��ָ���������ʱ��ͨ����ָ������麯��������ȷ�ص���������麯��
		����һ������ָ��ָ��ͬ���������ʱ��������ͬ���麯�������в�ͬ�ı���

	����ԭ���ǣ�
		�������麯����ַ������麯������
		�����ڴ�ռ����λ���ָ���麯�����ָ��
		������д�麯��ʱ���Ὣ���ж�Ӧ�麯���ĵ�ַ����

	�麯���ڱ��е�˳����麯��������˳��һ�¡������麯���������麯��ǰ��

	���ж������ʱ�������̳�ÿ��������麯������������λ����ƫ��һ��ָ��ĵط���ŵڶ���������麯����
*/

class A
{
public:
	virtual void vfunc1() { printf("    call A::vfunc1\n"); }
	virtual void vfunc2() { printf("    call A::vfunc2\n"); }
};

class B :public A 
{
public:
	virtual void vfunc2() { printf("    call B::vfunc2\n"); }
	virtual void vfunc3() { printf("    call B::vfunc3\n"); }
};
typedef void(*Fun)(void);
void vtest1()
{
	A a;
	B b;

	printf("�ֶ�����a���麯��:\n");
	a.vfunc1();
	a.vfunc2();
	printf("�ֶ�����b���麯��:\n");
	b.vfunc1();
	b.vfunc2();
	b.vfunc3();

	printf("a��b�ĵ�ַ:\n");
	printf("    &a :%x\n", &a);
	printf("    &b :%x\n", &b);

	printf("a��b���麯������麯����ַ:\n");
	//ȡa�ĵ�ַ��ת��λint*ָ�룬�����죬�õ�a���׵�ַ��ֵ����ֵΪ�麯�����ַ
	printf("    vftb of a :%x\n", *(int*)(&a));
	//��a���׵�ַ��ֵ�ٴ�ת��λint*��Ȼ�����죬�õ�a���׵�ַָ��ĵ�ַ��ֵ����ֵΪ�麯����ֵ
	printf("    first address of vftba point :%x\n", *(int*)*(int*)(&a));
	printf("    seceond address of vftba point :%x\n", *((int*)*(int*)(&a) + 1));
	printf("    vftb of b :%x\n", *(int*)(&b));
	printf("    first address of vftbb point :%x\n", *(int*)*(int*)(&b));
	printf("    seceond address of vftba point :%x\n", *((int*)*(int*)(&b) + 1));

	int* vtba = (int*)*(int*)(&a);
	int i = 0;
	Fun pf;
	printf("�����麯��ָ��˳���������a���麯��:\n");
	while (i < 2)
	{
		pf = (Fun) * ((int*)*(int*)(&a) + i);
		pf();
		++i;
	}
	i = 0;
	printf("�����麯��ָ��˳���������b���麯��:\n");
	while (i < 3)
	{
		pf = (Fun) * ((int*)*(int*)(&b) + i);
		pf();
		++i;
	}
	printf("    �麯����������˳���������麯������\n");




	//Fun pf = (Fun )*((int*)*(int*)(&b) + 1);
	//pf();
	////pf = &B::vfunc2;
	////Fun pf2 = A::vfunc1;

	printf("\n");
}