#pragma once
/*
	执行在编译期的程序

	case 47 的traits就是一种元编程，它发生在编译器，并且配合函数重载匹配来实现类似if-else的逻辑
	对于那些已经在编译器就能确定的if-else，可以用traits来优化
	
	另外一个例子就斐波那契数列，它之于元编程如同“hello world”之于所有编程语言入门

*/
template<unsigned n>
struct Factorial
{
	enum { value = n * Factorial<n-1>::value };
};

template<>
struct Factorial<0>
{
	enum { value = 1 };
};

/*
	以上由于F(n)的value依赖于F(n-1)，所以具象化n之前会去具象化n-1的类模板
	以此类推，最终止F(0)，因为F(0)是全特化的类模板，不依赖于n-1，

	那么结果是，对于Factorial<n>，当然n的是常量，会具象化出一堆类模板
	相当于编译期就算好了结果，就没有运行时计算的行为了
	并且另外的好处是，对于任意的小于n的常量，由于已经具象化好，就能直接拿到value。一次编译，到处使用
	颇有一种面向答案编程的感觉，也和算法里的动态规划类似

	如果是计算代码，通常是用一个循环去实现
	但是元编程不涉及逻辑代码，它并没有循环的语法。而类模板递归特化，是循环的替代品
	可能在C++里，你不喜欢递归，认为它们低效、不可读，但是这里是元编程，递归特化在编译期完成，它的时间复杂度是O(1)
	它就相当于直接拿到了答案，不仅比函数递归快，还比函数循环快
	只是它的使用不那么方便，因为你必须在模板参数列表里指定一个常量。
	如：Factorial<10>::value


	TMP技术
		expression templates
		policy-based design
		CRTP
*/