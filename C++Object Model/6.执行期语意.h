#pragma once
class Complex 
{
public:
	Complex(double, double = 0.0) {}
};

// 没有默认构造，编译时出错，全局/静态类对象数组的初始化仅支持默认构造
Complex g_Complexes[10]; 