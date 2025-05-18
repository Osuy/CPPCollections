#pragma once
namespace case42
{
/*
	一、用于模板列表，和class没有区别
	二、标识嵌套类型名称
*/
template<typename C>
void func(C&& ctn)
{
	/*
		value_type是C命名空间里的元素，但是C是模板参数
		因此并不知道value_type到底是什么？如果是一个静态变量，那么下面的语句就是一个乘法表达式
		如果是一个类型，那么就是定义了一个指针局部变量
		为了在语法分析阶段让编译器知道这是类型，需要用typename指明
		value_type 也称 模板参数从属类型
	*/
	C::value_type* p;
}
/*
	typename不能用于继承父类生命和初始化列表
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
}