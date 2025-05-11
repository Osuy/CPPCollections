#pragma once
/*
	作用一：用于模板列表，和class没有区别

	作用二：标识嵌套类型名称

	不能用于继承父类生命和初始化列表
	template<class T>
	class ClassD : public ClassB<T>::InnerClass // 此处无需加 typename
	{
		ClassD(...)
			: ClassB<T>::InnerClass{ ... } // 此处无需加 typename
		{}
	};

	当想要使用Class1<T>::SomeType来定义一个变量时，编译器仅凭这个标识是无法知道它是什么
	一个函数？一个变量？即便是一个类型，但由于在类模板里，尚未具象化，因此不能知道是什么类型
	虽然在具象化之后就能确定是类型，但是在具象化之前，编译期需要语法分析，它必须要求代码的正确性
	而此时不能知道是什么。通过typename可以告知编译器这是一个类模板的一个内部类型，让编译期在语法分析阶段不会报错
*/