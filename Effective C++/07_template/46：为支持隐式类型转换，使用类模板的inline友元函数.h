#pragma once

namespace case46
{
/*
	����ģ���������㶨��Ϊinline��Ԫ����

	���ȿ���һ������case24�����⣺
		Ϊ����rational�ܹ���ˣ���Ҫ��operator*����Ϊno-member����������rational����
		�����ܹ���ʽתΪrational����ֵ����Ҳ������ʹ�����operator*

	���ǵ�rational�����Ϊһ����ģ��ʱ�������Ǹ�no-member operator*Ҳ���ò�����Ϊһ������ģ�壺
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
	Ȼ������������ʱ��Ч�ġ���Ϊ����ģ����ʹ��ǰ��Ҫ�Ƶ�ģ�����
	��rational��int���ʱ��һ��ration��һ��int���޷��Ƶ���T��
	��ʽ����ת��ֻ�ں����������ڵ�����·�����������ģ��������Ƶ�֮ǰ��������������
	�����Ƶ��׶��޷�������ʽ����ת����Ҳ���޷�֧�����

	Ϊ�˱ܿ�����ģ������Ƶ���������ģ��д����ģ���ڲ�������Ҳ������һ������ģ�������ģ�����ȷ��ģ��������ܿ�����ģ������Ƶ�
	�����Ƶ�ֻ�к���ģ�������������ģ�岻��ʹ�ò����Ƶ�������ͨ��������ȷ�������Ĳ�����
*/

template<typename T>
class Rational
{
	// ...
	const Rational operator*(const Rational& lhs, const Rational& rhs);
};
/*
	Ȼ����������һ��member��Ա�ˣ��޷�֧����ˣ�case24��
	Ϊ����no-member�ķ�ʽʵ�֣���Ҫ��friend����
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
	operator*�ƺ���һ������ģ�壬������������Rational����Ԫ
	����Rationʵ����Ȼ�ڲ���һ��operator*�������������������Ҳ��֧����ʽ����ת����
	��ʹ��operator*֮ǰ����Ȼ�Լ�������Rational��ʵ����
	Ȼ���Ѿô���һ�����⣺�޷�����
	Ϊ�������ӳɹ�����Ҫinline��ֱ�Ӱ������Ͷ��嶼д������
*/
}