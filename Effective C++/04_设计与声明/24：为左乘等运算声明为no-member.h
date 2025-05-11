#pragma once

/*
	有理数的乘法存在左乘和右乘。右乘可以声明为member
	但是左乘比较特殊，需要声明为no-member
	而左乘同样能匹配右乘，所以只需声明一个左乘
*/

class Rational
{
public:
	Rational(int numerator = 0, int denominator = 1)
		: _numerator{ numerator }
		, _denominator{ denominator }
	{
	}

	int numerator()const { return _numerator; }
	int denominator()const { return _denominator; }

	// 右乘，可被左乘替代
	/*const Rational operator*(const Rational& rhs)const
	{
		return { _numerator * rhs._denominator + _denominator * rhs._numerator, _denominator * rhs._denominator };
	}*/

private:
	int _numerator;
	int _denominator;
};

const Rational operator*(const Rational& lhs, const Rational& rhs)
{
	return { lhs.numerator() * rhs.denominator() + lhs.denominator() * rhs.numerator(), lhs.denominator() * rhs.denominator() };
}