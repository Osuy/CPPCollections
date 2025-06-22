#include "02_构造、析构、赋值/12：自定义拷贝和赋值的隐患.h"
#include "07_template/43：类模板基类的成员.h"

int main()
{
	case12::test();
	B b{};
	B b2{};
	b2 = b;
	return 0;
}