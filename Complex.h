#pragma once
#include <iostream>
using std::cout;
using std::ostream;

namespace study
{
	class Complex
	{
	private:
		double re, im;
	public:
		Complex() : re(0), im(0) {}
		Complex(double r, double i) : re(r), im(i) {}

		double real()const { return re; }
		double imag()const { return im; }

		//�����ڲ�������ߵģ�дΪ��Ա
		Complex& operator+=(const Complex& r);
		
		//��������ڲ��������ߵģ�дΪfriend
		friend Complex operator+(const Complex& x, const Complex& y);

		//����ֻ���ڲ������ұߵģ�дΪfriend
		//���� ostream����Ϊconst
		friend ostream& operator<<(ostream& os, const Complex& x);

	};

	inline Complex& Complex::
		operator+=(const Complex& r)
	{
		this->re += r.re;
		this->im += r.im;
		return *this;
	}

	Complex operator+(const Complex& x, const Complex& y)
	{
		return Complex( x.re + y.re,
						x.im + y.im  );
	}

	ostream& operator<<(ostream& os, const Complex& x)
	{
		return os << "(" << x.re << "," << x.im << ")";
	}
}