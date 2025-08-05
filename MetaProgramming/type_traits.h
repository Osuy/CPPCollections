#pragma once

/*
	模板参数声明习惯用class而typename，可能是历史原因，或字符数更少。但一些新的库推荐使用typename
	对于类型用struct而非class，可以省略继承和成员域的public
	无后缀：实现类
	后缀_t：别名模板，一般形式为：类::type。用于省略“::type”，减少文本
	后缀_v：别名模板，可能是类::value；也可能是直接变量
		一些实现类可能是基于integral_constants<XXX_v<T>>实现的

	C++有定长数组arr[N]和不定长数组arr[]，因此关于数组的traits，会有两个版本的偏特化
	如果是bounded_array，只有定长偏特化；如果是unbounded_array，只有不定长的偏特化

	偏特化模板参数列表内的匹配失败才是SFINAE
	父类和定义里的推导失败会引起具体的错误
	void_t就是用来在偏特化参数列表里推导用的
*/
// false 常量，对于任何模板参数都是false
template<typename> constexpr bool _Always_false = false;

// 整形常量
template<typename T, T val>
struct integral_constant
{
	static constexpr T value = val;
	using value_type = T;
	using type = integral_constant;

	constexpr operator value_type()const noexcept { return  value; }
	constexpr value_type operator()()const noexcept { return  value; }
};

// bool常量，是整型常量的别名模板
template<bool val>using bool_constant = integral_constant<bool, val>;

using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

/*
	conjunction：从参数包中找到第一个value为false的类型
	接收一个bool和可变模板参数，要求每个参数有value成员
	主模板：bool匹配false，取参数包的第一个作为type
	偏特化：bool匹配true，显示接受参数包的第一和第二，并递归传递第二::value和剩余参数包
		
	当参数包内的成员不存在value成员时，编译错误
*/
template<bool first_value, typename First, typename...>
struct _Conjunction {
	using type = First;
};

template<typename First, typename Next, typename... Rest>
struct _Conjunction<true, First, Next, Rest...> {
	using type = _Conjunction<Next::value, Next, Rest...>::type;
};

template<typename... T> struct conjunction : true_type {};
template<typename First, typename...Rest> 
struct conjunction<First, Rest...> : _Conjunction<First::value, First, Rest...>::type {};

template<typename... T>
constexpr bool conjunction_v = conjunction<T...>::value;

// 取反 继承自bool_constant，其值为T::value取反
template<typename T> struct negation : bool_constant<!static_cast<bool>(T::value)> {};

template <typename _Trait> constexpr bool negation_v = negation<_Trait>::value;

// 标识类，通常用于作为父类，以省略using type = XXX;，减少代码量
template<typename T> struct identity { using type = T; };

template<typename T> using identity_t = identity<T>::type;

/*
	remove_cv 移除类型中的const和volatile
	主模板匹配原始类型
	偏特化匹配const、volatile、const volatile 三种不同的类型

*/
template<typename T> struct remove_cv					 : identity<T> {};
template<typename T> struct remove_cv<const T>			 : identity<T> {};
template<typename T> struct remove_cv<volatile T>		 : identity<T> {};
template<typename T> struct remove_cv<const volatile T> : identity<T> {};

template<typename T> using remove_cv_t = typename remove_cv<T>::type;

// 两个类型是否相同
template<typename, typename>  constexpr bool is_same_v		= false;
template<typename T>		constexpr bool is_same_v<T, T>  = true;

template<typename T1, typename T2>
struct is_same : bool_constant<is_same_v<T1, T2>> {};

// 是否为void类型
template <typename _Ty>
constexpr bool is_void_v = is_same_v<remove_cv_t<_Ty>, void>;

// void_t 不具有直接使用的价值，仅用于SFINAE。相当于一个纯模板参数推导工具
template<typename... T>using void_t = void;

// add_const \ add_volatile \ add_cv 增加const\volatile\const volatile修饰
template<typename T>struct add_const	 :identity<const T> {};
template<typename T>struct add_volatile :identity<volatile T> {};
template<typename T>struct add_cv		 :identity<const volatile T> {};

template<typename T>using add_const_t	  = add_const<T>::type;
template<typename T>using add_volatile_t = add_volatile<T>::type;
template<typename T>using add_cv_t		  = add_cv<T>::type;

// is_reference 是否为引用
// template<typename>constexpr bool is_reference_v = false;
template<typename T>constexpr bool is_reference_v<T&> = true;
template<typename T>constexpr bool is_reference_v<T&&> = true;
template<typename T>struct is_reference :bool_constant<is_reference_v<T>> {};

template<typename>constexpr bool is_lvalue_reference_v = false;
template<typename T>constexpr bool is_lvalue_reference_v<T&> = true;
template<typename T>struct is_lvalue_reference :bool_constant<is_lvalue_reference_v<T>> {};

template<typename>constexpr bool is_rvalue_reference_v = false;
template<typename T>constexpr bool is_rvalue_reference_v<T&&> = true;
template<typename T>struct is_rvalue_reference :bool_constant<is_rvalue_reference_v<T>> {};

/*
	add_reference 为类型添加引用修饰
	主模板匹配不可引用类型（void和函数指针）。它有第二个模板参数，匿名，默认void。目的是让偏特化进行SFINAE
	偏特化版本匹配可引用类型，并且使用void_t进行类型推导，推导成功表示该类型是可引用类型
	因此偏特化需要主模板有第二个模板参数

	引申一下引用折叠，四句话：
		左值是左值
		右值是右值
		左值引用是左值
		右值引用是左值（重要）

	用代码的形式就是
		T& & = T&
		T&& & = T&
		T& && = T&
		T&& && = T&&

	所以即使把引用传入 add_reference<...> 也是能正确推导出实际的左值与右值的
	若T = T&，则 void_t<T& &> 推导成功，匹配成功
	于是 lvalue = T&  & = T&; rvalue = T& && = T&;
*/
template<typename T, typename = void>
struct add_reference
{
	using lvalue = T;
	using rvalue = T;
};

template<typename T>
struct add_reference<T, void_t<T&>> // 使用void_t进行SFINAE
{
	using lvalue = T&;
	using rvalue = T&&;
};

template<typename T> struct add_lvalue_reference { using type = add_reference<T>::lvalue; };
template<typename T> struct add_rvalue_reference { using type = add_reference<T>::rvalue; };
template<typename T> using add_lvalue_reference_t = add_lvalue_reference<T>::type;
template<typename T> using add_rvalue_reference_t = add_rvalue_reference<T>::type;

/*
	declval() 一个用于推导类型有无某非静态成员函数的工具
	它没有实现，或者实现内部为总是false的static assert
	因此不能被调用，只能出现在decltype(...)内部
	它的返回值是T&& 因此可以用.“调用”非静态成员函数，再推导出其返回值的类型
*/
template<typename T>
add_rvalue_reference_t<T> declval() noexcept;

// is_pointer

// remove_pointer 移除指针修饰
template<typename T>struct remove_pointer					 :identity<T> {};
template<typename T>struct remove_pointer<T*>				 :identity<T> {};
template<typename T>struct remove_pointer<T* const>			 :identity<T> {};
template<typename T>struct remove_pointer<T* volatile>		 :identity<T> {};
template<typename T>struct remove_pointer<T* const volatile> :identity<T> {};

template <typename T> using remove_pointer_t = typename remove_pointer<T>::type;

// add_point添加指针修饰， T可能是非原始类型，所以需要remove_reference
template<typename T, typename = void>
struct add_pointer :identity<T> {};
template<typename T>
struct add_pointer<T, void_t <remove_reference_t<T>*>> 
{
	using type = remove_reference_t<T>*;
};

template <typename _Ty> using add_pointer_t = typename add_pointer<T>::type;

// is_array 是否是数组 
template<typename>			   constexpr bool is_array_v	   = false;
template<typename T>		   constexpr bool is_array_v<T[]>  = true;
template<typename T, size_t N>constexpr bool is_array_v<T[N]> = true;
template<typename T>struct is_array :bool_constant<is_array_v<T>> {};

template<typename>			   constexpr bool is_bounded_array_v = false;
template<typename T, size_t N>constexpr bool is_bounded_array_v<T[N]> = true;
template<typename T>struct is_bounded_array :bool_constant<is_bounded_array_v<T>> {};

template<typename>			   constexpr bool is_unbounded_array_v = false;
template<typename T>		   constexpr bool is_unbounded_array_v<T[]> = true;
template<typename T>struct is_unbounded_array :bool_constant<is_unbounded_array_v<T>> {};

/*
	remove_extent 移除数组维度，推导出数组元素的类型
	remove_all_extent 移除数组的所有维度，推导最基层的元素类型
*/
template<typename T>			struct remove_extent :identity<T> {};
template<typename T>			struct remove_extent<T[]> :identity<T> {};
template<typename T, size_t N> struct remove_extent<T[N]> :identity<T> {};

template <typename _Ty> using remove_extent_t = typename remove_extent<_Ty>::type;

template<typename T>			struct remove_all_extent :identity<T> {};
template<typename T>			struct remove_all_extent<T[]> :remove_all_extent<T> {};
template<typename T, size_t N> struct remove_all_extent<T[N]> :remove_all_extent<T> {};

template <typename T> using remove_all_extent_t = typename remove_all_extent<T>::type;

void test()
{
}