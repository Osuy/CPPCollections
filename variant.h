#pragma once
#include<variant>
/*
	variant是类型安全的union
	variant在同一时间只能表示其多个模板参数类型中的其中一个，或者出错（没有值）
	和union一样，如果variant持有了一个值，那么其内存将直接在variant的内存中分配
	variant不允许分配额外的动态内存
	variant不允许持有引用、数组、void
	variant不能为空
	variant允许持有多个相同的类型，允许const、volatile

	与union的初始化一样，variant的默认构造函数会使用其第一个替代类型，
	除非定义了构造函数并指定了其他类型作为构造函数的参数
*/

