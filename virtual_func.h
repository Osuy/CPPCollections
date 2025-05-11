#pragma once
#include "test_helper.h"
/*
	虚函数是C++实现多态的手段
	表现为：
		父类指针指向子类对象时，通过该指针调用虚函数，会正确地调用子类的虚函数
		于是一个父类指针指向不同的子类对象时，调用相同的虚函数，会有不同的表现

	基本原理是：
		将所有虚函数地址存放在虚函数表中
		在类内存空间的首位存放指向虚函数表的指针
		子类重写虚函数时，会将表中对应虚函数的地址覆盖

	虚函数在表中的顺序和虚函数声明的顺序一致。父类虚函数在子类虚函数前面

	当有多个父类时，子类会继承每个父类的虚函数表。即，在首位往后偏移一个指针的地方存放第二个父类的虚函数表
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

	printf("手动调用a的虚函数:\n");
	a.vfunc1();
	a.vfunc2();
	printf("手动调用b的虚函数:\n");
	b.vfunc1();
	b.vfunc2();
	b.vfunc3();

	printf("a和b的地址:\n");
	printf("    &a :%x\n", &a);
	printf("    &b :%x\n", &b);

	printf("a和b的虚函数表和虚函数地址:\n");
	//取a的地址，转化位int*指针，再提领，得到a的首地址的值，该值为虚函数表地址
	printf("    vftb of a :%x\n", *(int*)(&a));
	//对a的首地址的值再次转化位int*，然后提领，得到a的首地址指向的地址的值，该值为虚函数的值
	printf("    first address of vftba point :%x\n", *(int*)*(int*)(&a));
	printf("    seceond address of vftba point :%x\n", *((int*)*(int*)(&a) + 1));
	printf("    vftb of b :%x\n", *(int*)(&b));
	printf("    first address of vftbb point :%x\n", *(int*)*(int*)(&b));
	printf("    seceond address of vftba point :%x\n", *((int*)*(int*)(&b) + 1));

	int* vtba = (int*)*(int*)(&a);
	int i = 0;
	Fun pf;
	printf("利用虚函数指针顺序调用所有a的虚函数:\n");
	while (i < 2)
	{
		pf = (Fun) * ((int*)*(int*)(&a) + i);
		pf();
		++i;
	}
	i = 0;
	printf("利用虚函数指针顺序调用所有b的虚函数:\n");
	while (i < 3)
	{
		pf = (Fun) * ((int*)*(int*)(&b) + i);
		pf();
		++i;
	}
	printf("    虚函数以声明的顺序排列在虚函数表中\n");




	//Fun pf = (Fun )*((int*)*(int*)(&b) + 1);
	//pf();
	////pf = &B::vfunc2;
	////Fun pf2 = A::vfunc1;

	printf("\n");
}