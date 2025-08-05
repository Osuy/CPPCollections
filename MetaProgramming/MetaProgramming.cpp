// MetaProgramming.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include "CRTP.h"
#include <assert.h>
#include "01_MetaFunction & MetaData.h"
#include "type_traits.h"

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

    rank<int[]>::value;
    extent<int[], 2>::value;

    test();

    /*
        v1 + v2 产生一个匿名对象VecSum<Vec, Vec>，
        v0 + 匿名对象，产生一个匿名对象VecSum<Vec, VecSum<Vec, Vec>>
        VecSum<...>对象只是建立了两个和数的引用关系，而和数自身也可以是一个VecSum
        Vec支持泛化的构造函数，可以传入一个VecExpression，VecSum<...>继承自VecExpression<VecSum<...>>，所以可以转型
        构造函数内遍历VecExpression，挨个[]取出元素并赋值给自己
        对VecSum<...>使用[]取元素，才会进行真正的运算，调用两个和数的[]取出元素并相加
        这个过程会一直递归达到树的叶子节点，然后向上返回，一直相加，直到根节点

        也就是说，=才进行了真正的运算
        如果只是 v0 + v1 + v2; 则不存在运算，只是产生了持有引用关系的匿名对象
    */
    Vec v3 = v0 + v1 + v2;

    std::cout << "Hello World!\n";
}