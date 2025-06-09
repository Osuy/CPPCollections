// MetaProgramming.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include "CRTP.h"
#include <assert.h>

// 转型为子类去实现函数
template<typename E>
struct VecExpression
{
    double operator[](size_t i) const
    {
        return static_cast<const E&>(*this)[i];
    }

    size_t size() const 
    {
        return static_cast<const E&>(*this).size();
    }
};

class Vec : public VecExpression<Vec>
{
    std::vector<double> elems;

public:
    Vec(size_t n) : elems(n) {}

    Vec(std::initializer_list<double> list) : elems{list} {}

    template<typename E>
    Vec(const VecExpression<E>& vec) : elems(vec.size())
    {
        for (size_t i = 0; i < vec.size(); ++i)
        {
            elems[i] = vec[i];
        }
    }
};

template<typename E1, typename E2>
class VecSum :public VecExpression<VecSum<E1, E2>>
{
    const E1& _u;
    const E2& _v;
public:
    VecSum(const E1& u, const E2& v) :_u{u}, _v{v}
    {
        assert(u.size() == v.size());
    }

	double operator[](size_t i) const
	{
        return _u[i] + _v[i];
	}

	size_t size() const
	{
		return _v.size();
	}
};

template<typename E1, typename E2>
const VecSum<E1, E2> operator+(const E1& u, const E2& v)
{
    return VecSum<E1, E2>(u, v);
}


int main()
{
    _crtp::example2::Derived obj;
    obj.base_func();
    Vec v0 = { 1.0, 1.0, 1.0 };
    Vec v1 = { 2.0, 2.0, 2.0 };
    Vec v2 = { 3.0, 3.0, 3.0 };

    Vec v3 = v0 + v1 + v2;

    std::cout << "Hello World!\n";
}