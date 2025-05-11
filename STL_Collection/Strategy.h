#pragma once


/*
	策略模式
	通常，我们利用enum+ifelse/switch实现多分支的语句：
		定义一个enum{a,b,c,d}
		对一个变量进行分支判断或switch：
			if(var==a){...}                          switch(var){
			else if(var==b){...}                         case a:...break;
			else if(var==c){...}                         case b:...break;
			else if(var==d){...}                         case c:...break;
								                         case d:...break;
														 default:break;
													 }

		这是一种效率十分低且难以复用的代码。
		一旦需要扩展，就必须在enum和分支同时修改。

	策略模式基于多态实现
	为所有分支设计一个父类，包含一个纯虚函数
	class Father{
	public:
		virtual void func()=0;
	};
	然后每个分支继承自Father，并重写func：
	class A{
	public:
		virtual void func(){...}
	}
	class B{...}
	class C{...}

	通过父类指针和多态来实现分支选择：
		Father* obj;//obj可以接收A，B，C，D类对象
		obj->func();//根据对象的类型调用对应的实现
*/