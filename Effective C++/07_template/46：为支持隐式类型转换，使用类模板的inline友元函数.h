#pragma once

namespace case46
{
/*
	将类模板的左乘运算定义为inline友元函数

	首先考虑一个来自case24的问题：
		为了让rational能够左乘，需要把operator*声明为no-member函数并传入rational参数
		于是能够隐式转为rational的数值类型也都可以使用这个operator*

	但是当rational被设计为一个类模板时，它的那个no-member operator*也不得不声明为一个函数模板：
*/
template<typename T>
class Rational
{
public:
	Rational(T numerator = 0, T denominator = 1)
		: _numerator{ numerator }
		, _denominator{ denominator }
	{
	}

	T numerator()const { return _numerator; }
	T denominator()const { return _denominator; }

private:
	T _numerator;
	T _denominator;
};

template<typename T>
const Rational<T> operator*(const Rational<T>& lhs, const Rational<T>& rhs)
{
	return { lhs.numerator() * rhs.denominator() + lhs.denominator() * rhs.numerator(), lhs.denominator() * rhs.denominator() };
}

/*
	然而这样的做法时无效的。因为函数模板在使用前需要推导模板参数
	而rational和int左乘时，一个ration和一个int是无法推导出T的
	隐式类型转换只在函数声明存在的情况下发生，而函数模板在完成推导之前，声明并不存在
	于是推导阶段无法适用隐式类型转换，也就无法支持左乘

	为了避开函数模板参数推导，将函数模板写在类模板内部，于是也不必是一个函数模板借用类模板具象化确定模板参数，避开函数模板参数推导
	参数推导只有函数模板才能做到，类模板不会使用参数推导，而是通过具象化来确定函数的参数。
*/

template<typename T>
class Rational
{
	// ...
	const Rational operator*(const Rational& lhs, const Rational& rhs);
};
/*
	然而这样就是一个member成员了，无法支持左乘（case24）
	为了以no-member的方式实现，需要用friend修饰
*/
template<typename T>
class Rational
{
	// ...
	friend const Rational operator*(const Rational& lhs, const Rational& rhs);
};

template<typename T>
const Rational<T> operator*(const Rational<T>& lhs, const Rational<T>& rhs)
{
	return { lhs.numerator() * rhs.denominator() + lhs.denominator() * rhs.numerator(), lhs.denominator() * rhs.denominator() };
}

/*
	operator*似乎是一个函数模板，但是由于它是Rational的友元
	所以Ration实例必然内部有一个operator*的声明，有了这个声明也就支持隐式类型转换了
	而使用operator*之前，必然以及存在了Rational的实例化
	然而已久存在一个问题：无法链接
	为了能链接成功，需要inline，直接把声明和定义都写在类内
*/
}