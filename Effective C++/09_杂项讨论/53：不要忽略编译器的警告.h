#pragma once
/*
	在编译器的最高警告等级下争取0警告

	不要过度依赖编译器的警告来排查错误，不同编译器的警告能力不同

	C++给了程序员很多自由，这也意味着更容易发生错误。忽略警告极有可能发生运行时的崩溃或非预期结果


	例如：
	struct Base
	{
		virtual void f()const;
	};

	struct Derived : public Base
	{
		virtual void f(); // 没有写override，也没有写const，被认为是一个重载而非重写
	};

	如果运行时，一个Derived的const对象调用f()，那总是会调用父类的f()，即没有发生多态。

*/