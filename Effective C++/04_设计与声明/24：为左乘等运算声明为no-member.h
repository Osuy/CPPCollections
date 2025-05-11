#pragma once

/*
	�������ĳ˷�������˺��ҳˡ��ҳ˿�������Ϊmember
	������˱Ƚ����⣬��Ҫ����Ϊno-member
	�����ͬ����ƥ���ҳˣ�����ֻ������һ�����
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

	// �ҳˣ��ɱ�������
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