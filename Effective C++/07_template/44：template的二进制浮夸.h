#pragma once

namespace case44
{
/*
	存在多个逻辑相同但仅有数个类型不同的函数或类，为其定义一个函数/类模板
	很棒，这样极大地减少代码量，并且由于模板只在被使用时才实例化
	未被使用的类模板的成员或函数模板也不存在
	这似乎已经是尽善尽美的方案了
	但仅仅只对于代码而言，编译器会为每个实例化的模板生成的二进制码
	因此实际上仍然存在重复

	就和非模板函数的优化一样，把共同逻辑提取为新的函数，并让其他函数调用
	对于函数/类模板，它们同样进行类似的提取
	分为类型提取和非类型提取两种
*/

/*
	非类型提取
	考虑以下类模板，一个矩阵，存储的元素类型以及矩阵的维度由模板参数提供
	有一个成员函数invert，用于将矩阵逆转
	假设matrix不存在特化，因此invert必须为所有维度提供逆转功能
	很明显，当为类模板实例化出两个仅维度不同的类时
	matrix<int, 5>和matrix<int, 10> invert为5和10各生成了一份代码
	这就是代码膨胀的所在，为了复用，5和10应该作为invert的参数
*/
template<typename T, size_t N>
class matrix
{
public:
	void invert();
};

/*
	修改后的matrix，它private继承自 matrixbase
	matrixbase则实现了带参数版本的invert
*/
template<typename T>
class matrix_base
{
	// 父类不带数据，所以需要一个指针只需子类持有的数据，
	// 以避免每个函数都要多一个T*参数
	T* _pdata = nullptr;
public:
	matrix_base(T* data) : _pdata{ data } {}
	void invert(size_t N);
};

template<typename T, size_t N>
class matrix2 : private matrix_base<T>
{
	T _data[N * N];
public:
	void invert()
	{
		matrix_base<T>::invert(N);
	}
};

/*
	类型提取
	类型提取并不好发挥，通常只局限于两种情况
	1.int和long在本平台表现为相同时，无需为它们二者各实例化一份。因此，可以考虑特化long，内部用int实现
	2.任何指针类型，本质是一样的，因此可以考虑对指针偏特化，并让所有偏特化都在内部用void*特化版来实现
*/
}