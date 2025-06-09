#pragma once

/*
	偏特化
	允许基于模板形参进行改写
	编译器会优先匹配偏特化，所有偏特化版本都不匹配后，才会去匹配主模板
	如果偏特化的形参为T&，当指定int为实参时，若int = T&，则无法推导出T所以不匹配，最后去匹配主模板，int = T，推导成功
		当指定int&为实参时，int& = T&，推导出T = int ，匹配

*/

// 是否引用，利用类模板偏特化进行分支判断
template<typename T> struct is_reference		{ static constexpr bool value = false; };
template<typename T> struct is_reference<T&>	{ static constexpr bool value = true; };
template<typename T> struct is_reference<T&&>	{ static constexpr bool value = true; };

template<typename T> struct remove_reference		{ using type = T; };
template<typename T> struct remove_reference<T&>	{ using type = T; };// 推导出原本的T。下同
template<typename T> struct remove_reference<T&&>	{ using type = T; };

/*
	以上，两个类模板接受实参，返回结果，它们调用了元编程中的"逻辑"部分，是最广泛使用的技术
	其中is_reference返回常量，被称为MetaData
	remove_reference返回类型，被称为MetaFunction
	
	由于在元编程阶段，类型往往比常量来得有用
	所以更多时候会将让其他类模板来作为MetaData，并将MetaData作为MetaFunction的typedef，并且命名为：type
	并且因为有专门的MetaData来存储常量，避免了常量重复定义的问题
	这是一个俗称的约定，许多元编程库都遵循这一约定
*/

// 由bool_constant作为MetaData
template<bool b>struct bool_constant { static constexpr bool value = b; };
// 将is_reference转换乘MetaFunction
template<typename T> struct is_reference2		{ using type = bool_constant < false >; };
template<typename T> struct is_reference2<T&>	{ using type = bool_constant < true >; };
template<typename T> struct is_reference2<T&&>	{ using type = bool_constant < true >; };


// 一个典型的MetaData
template<typename T, T v>
struct integral_constant
{
	static constexpr T value = v;// 存储常量
	
	using value_type = T;// 标准的内嵌类型声明，可以适配stl
	using type = integral_constant;// 标准的meta命名，可以适配MetaFunction
	constexpr operator value_type()const noexcept { return value; } // 支持向value_type的隐式转换
	constexpr value_type operator()()const noexcept { return value; } // 支持函数式调用
};

// 于是对上述的bool_constant，可以改写为：
template<bool B> using bool_constant = integral_constant<bool, B>; // 别名模板，一个泛化的typedef，也称alias template
using true_type  = bool_constant<true>;
using false_type = bool_constant<false>;

// 于是is_reference的定义可以改为：
template<typename T> struct is_reference3 { using type = false_type; };
template<typename T> struct is_reference3<T&> { using type = true_type; };
template<typename T> struct is_reference3<T&&> { using type = true_type; };

/*
	三种使用方式：
	is_reference3<int>::type::value;	使用value
	is_reference3<int>::type();			使用匿名对象隐式转型成value_type
	is_reference3<int>::type()();		使用匿名对象.operator()
*/

/*
	因为约定MetaData有type的typedef，所以其也可以算作一个MetaFunction，可以有相似的行为
	这么做不是多此一举，而是为了写更少的代码。（在这一点上，元编程的开发者们无所不用其极）
	对于直接将另一个MetaFunction作为返回值的MetaFunction，可以不用typedef，而是直接继承
	并且，继承得来的并不只有type，value也一并继承，而类的实现就直接省略为{}，而不必在using
*/
template<typename T> struct is_reference4 : public false_type {};
template<typename T> struct is_reference4<T&> : public true_type {};
template<typename T> struct is_reference4<T&&> : public true_type {};

/*
	对比四个版本的is_reference，一步步递进，并最终达到了和stl相似的版本
	中间产生的integral_constant、true_type、false_type也被其他模板大量使用

	并且也方便自己根据需求构建自己的元编程模板
	例如，希望实现一个Meta Function，用于判断是否是int或引用类型
*/
template<typename T> struct is_int_or_reference		 : public is_reference4<T> {};
template<>			 struct is_int_or_reference<int> : public true_type {};


// 几乎所有元编程库里都包含的一个最简单的MetaFunction
template<typename T> struct type_identity { using type = T };
/*
	接受一个形参，并返回其本身。这有什么用？
	using type = T是不是很熟悉？还记得remove_reference的三个版本里都有这么一句using吗？
	之前提到过，元编程库倾向于用继承来代替重using，使类实现为空{}
	那么有了type_identity后，也就能对remove_reference动刀了
*/

template<typename T> struct remove_reference2		: public type_identity<T> {};
template<typename T> struct remove_reference2<T&>	: public type_identity<T> {};
template<typename T> struct remove_reference2<T&&>  : public type_identity<T> {};

//type_identity用处简单而强大，已经在C++20被加入到标准库中！！


/*
	另外，常常带后缀_v的变量模板来简化从MetaFunction里获取常量的代码
	带后缀_t的别名模板来简化从MetaFunction获取type的代码
*/
template<typename T> inline constexpr bool is_reference_v = is_reference4<T>::value;
template<typename T> using remove_reference_t = typename remove_reference2<T>::type;


/*
	一些简单的多参数模板
*/
// 两个类型是否相同
template<typename T, typename U> struct is_same		  : public false_type {}; // 主模板，继承false_type
template<typename T>			 struct is_same<T, T> : public true_type {}; // 偏特化，两个类型相同时匹配，继承true_type

// 第一个类型是否是后续多个类型中的其中一个，用到了可变长模板参数
template<typename T, typename _1, typename ...Rest>
struct is_one_of : public bool_constant < is_one_of<T, _1>::value || is_one_of<T, Rest...>::value > {};
template<typename T, typename U> 
struct is_one_of : public is_same<T, U> {};

/*
	这是除了常量模板递归以外的第二种递归方式，可变长模板参数递归
	利用的剩余参数打包机制，重复递归有两个形参的is_one_of，最终将T与后面的所有参数进行is_same的判断

	另外 注意 || 操作符，通常发生在运行时，如果前面的表达式为true，就不会继续执行后面的表达式
	但是在编译期，模板具象化时并不是在对表达式求值，而是在确定常量。所以||后面的类模板依然会被实例化
*/

//利用is_one_of可以轻松实现某些复杂的MetaFunction
template<typename T>
using is_integral = is_one_of<T, bool, char, short, int, long, long long>; // 判断T是否时整型


/*
	is_instantiation_of
	判断一个类型是否时一个类模板的具象化
	显然需要两个形参：类型、类模板类型。
	注意，当类模板作为类型形参时，typename写在在template之后

	默认以主模板为不匹配，所以继承false_type，Inst<...>和Tmpl不匹配，即类名不匹配。（模板参数不对的化，编译就直接出错，所以只有类名不匹配）
	特化版本为匹配，所以继承true_type，由于不知道具体有几个形参，所以用可变长参数ARGS...，所以匹配的结果应该是： Inst<ARGS...> == Tmpl<ARGS...>
	所以第一个参数为Tmpl<ARGS...>，第二个参数为Tmpl
*/
template<typename Inst, template<typename ...>typename Impl>
struct is_instantiation_of :public false_type {};

template<template<typename ...> typename Tmpl, typename ...ARGS>
struct is_instantiation_of<Tmpl<ARGS...>, Tmpl> :public true_type {};

/*
	conditional，元编程中的if-else
	除了traits配合函数重载，也有专门用于分支判断的模板
	它接受一个bool常量和两个类型，当bool为true时返回第一个，当bool为false时返回第二个类型
	利用特化可以很轻松地实现
*/			 
template<bool B, typename T, typename F> struct conditional				 : public type_identity<T> {};
template<typename T,typename F>			 struct conditional<false, T, F> : public type_identity<F> {};

/*
	有意思地是，conditional地主模板和其特化中，只存在一个type_identity。
	只要存在type_identity<T>，就不存在type_identity<F>。反之同理
	而对于不存在的type_identity<T>，T也不会被具象化。
	这和||操作不同，||操作本质没有发生，是编译期在确定一个常量，所以前后表达式都会具象化
	而conditional的T和F作为形参，最终只有其中一个传入type_identity，也就只有一个被具象化，另一个只是被作为形参，用于匹配，匹配结束后就不会用到

	这个方法可以有效减少那些只用于匹配形参而不会实际用到的类模板，实现了类似运行时||前者true就不会对后者求值的效果
	可以有效减少编译的时间
*/

// 返回数组的维度
template<typename T>				 struct rank	   :public integral_constant<unsigned int, 0> {};
template<typename T>				 struct rank<T[]>  :public integral_constant<unsigned int, rank<T>::value + 1> {};
template<typename T, unsigned int N> struct rank<T[N]> :public integral_constant<unsigned int, rank<T>::value + 1> {};

// 返回数组第N维的大小
template<typename T, unsigned int N = 1> struct extent : public integral_constant<unsigned int, 0> {};
template<typename T, unsigned int N, unsigned int D> struct extent<T[D], N> : public integral_constant<unsigned int, extent<T, N - 1>::value> {};
template<typename T, unsigned int D> struct extent<T[D], 1> : public integral_constant<unsigned int, D> {};