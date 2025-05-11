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

		//自身在操作符左边的，写为成员
		Complex& operator+=(const Complex& r);
		
		//自身可以在操作符两边的，写为friend
		friend Complex operator+(const Complex& x, const Complex& y);

		//自身只能在操作符右边的，写为friend
		//另外 ostream不能为const
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