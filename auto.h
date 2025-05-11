#pragma once

/*
	用auto声明变量，必须初始化，如：
		int a = 1;
		char b = 'b';
		std::vector<int>::iterator c = array.begin();
	以上，a和b还不够明确，但是c接收一个数组的begin()迭代器，它的类型显然就是这个数组的迭代器的类型
	std::vector<int>::iterator  只是此处的例子，实际开发中，可能由各种类名、模板参数、成员、指针组合在一起的情况
	一是类型检查困难，二是容易书写错误
	很容易让使用者觉得：C++这一语言死板、僵硬，不够智能

	C++11 新增了auto关键字
	对于声明即初始化的变量，可以使用auto让编译器自己替换类型，省去了由程序员来类型判断的时间
	如
		auto c = array.begin(); //自动推导为vector<int>::iterator
		auto d = queue.front(); //自动推导为queue<int>::reference

	注意事项
		auto配合for迭代容器时，默认是值拷贝元素的
			for(auto i : vec){...} // i拷贝了vec的元素的值，对i修改不会影响vec的元素
			for(auto& i : vec){...} // i引用了vec的元素，对i修改会影响vec的元素
			for(const auto& i : vec){...}  // i const引用了vec的元素，对i修改不会影响vec的元素
		
		因此，使用auto时，不要指望编译器加上&、*、const，除非初始化时足够明确
		*、&、const 等修饰，自己能加尽量加上
*/

/*
	auto和模板

	对于一个计算两个参数的乘积并返回的函数模板
	template<class T1, class T2>
	auto Multiply(T1 x, T2 y) -> decltype(x * y)
	{
		auto res = x * y;
		return res;
	}
	因为并不知道两个参数的类型，也就不知道它们的乘积是什么类型
	因此可以使用auto，自动推导出其类型。

*/

/*
	auto去除引用语义
	结合auto配合for循环遍历容器的例子，auto以编译器的角度去考虑
	总是会做尽可能少的事情，而把更多是事情交给程序员去做

	例如
		int a = 10;
		int& b = a;
		auto c = b;//此时c不是int&，而是int。因为它认为程序员是想声明一个变量，而不是引用

	auto推导出引用语义时，会进一步推导出实际的数据类型，而不是包含引用

	同理，auto也会去除const语义、volatile语义

	因此使用auto时，把它当作一个原子类型，如果需要const、&、volatile 就加上
*/