#pragma once

namespace case3
{
	// const 与指针
	const int* point_to_constant;// const在*左边，表示指针指向的是一个常量。同 int const*
	int* const point_constant;// const在*右边，表示指针自身是个常量
	// 可以与常引用对比加深记忆。& == *const
	int integer = 1;
	const int& ref = integer;// 由于引用是一种指针常量，所以const是修饰int的
	const int* ptr = &integer;// 和引用的格式相同，所以const也是修饰int的

	// const 与迭代器
	// const 迭代器不能改变指向， 因为从修饰的是整个迭代器，即 const (T*)
	// const_iterator不能修改所指的内容，它被视为 (const T)*

	// const 与返回值
	// 拷贝类型的返回值本身应该直接使用，或者再拷贝修改，而不应该在其上修改。能够避免一些错误
	// 预防意外的拼写错误： a*b == c 少些了一个=，变成 a*b = c，如果operator*返回值是const，能在编译期发现错误

	// const 与参数

	// const 与成员函数
	// 要求成员函数不得修改对象，或者所该成员函数内部的this 是一个const T*
	// const可以形成重载。
	// 未重载时，非const对象都可以使用，但const对象不能使用非const函数
	// 重载后，const和非const对象只能使用对应版本的函数
	struct test
	{
		void func() {}// 没重载，只能非const对象调用
		void cfunc() const{}// 没重载，非const对象和const对象都可以调用（一个非const对象调用const函数也无可厚非）
		int& func2(); // 重载了，非const对象只能会使用此函数
		const int& func2()const {}// 重载了，const对象只能会使用此函数
	};
	// 可以认为，如果一个函数有const重载，那么非const对象调用此函数时
	// 如果不能修改自身是一种多余的限制，从而总会被认为它希望修改自身，从而只能调用非const版本
	// 诸如标准库的容器的map和string，都提供了const重载，从而无法修改const对象的[i]
	// 但又可以修改非const对象的

	// const 与mutable
	// const 成员函数有不能修改变量的约束。但是时而又会有应该更新某些变量只是外界不知道的需要
	// 此时可以将这些成员变量声明为mutable，表示它们即使在const函数内可以被修改
	// 但这样的技巧已完全被const_cast取代

	// const 与 const_cast
	// 如果所有成员变量都值得在某些const函数里修改，如何呢？难道所有成员变量都声明为mutable吗？
	// 它额外的会使所有变量不得不从内存读取而使缓存无用，降低了性能
	// 再者，当你真的有了const和非const两个版本的函数时，它虽然有不同之处，但总体的逻辑依然一样，甚至完全一样
	// （就像operator[]，函数体完全一样，返回值一个是拷贝，另一个是引用）
	// 那么在日后的不断修改中，都需要同时修改两个函数，还存在可能的疏忽大意带来的不一致性
	// 因此可以配合const_cast，在非const版本的内部调用const版本
	int& test::func2()
	{
		return const_cast<int&>(static_cast<const test*>(this)->func2());
	}

}