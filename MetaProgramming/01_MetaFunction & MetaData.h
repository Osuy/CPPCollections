#pragma once

/*
	ƫ�ػ�
	�������ģ���βν��и�д
	������������ƥ��ƫ�ػ�������ƫ�ػ��汾����ƥ��󣬲Ż�ȥƥ����ģ��
	���ƫ�ػ����β�ΪT&����ָ��intΪʵ��ʱ����int = T&�����޷��Ƶ���T���Բ�ƥ�䣬���ȥƥ����ģ�壬int = T���Ƶ��ɹ�
		��ָ��int&Ϊʵ��ʱ��int& = T&���Ƶ���T = int ��ƥ��

*/

// �Ƿ����ã�������ģ��ƫ�ػ����з�֧�ж�
template<typename T> struct is_reference		{ static constexpr bool value = false; };
template<typename T> struct is_reference<T&>	{ static constexpr bool value = true; };
template<typename T> struct is_reference<T&&>	{ static constexpr bool value = true; };

template<typename T> struct remove_reference		{ using type = T; };
template<typename T> struct remove_reference<T&>	{ using type = T; };// �Ƶ���ԭ����T����ͬ
template<typename T> struct remove_reference<T&&>	{ using type = T; };

/*
	���ϣ�������ģ�����ʵ�Σ����ؽ�������ǵ�����Ԫ����е�"�߼�"���֣�����㷺ʹ�õļ���
	����is_reference���س���������ΪMetaData
	remove_reference�������ͣ�����ΪMetaFunction
	
	������Ԫ��̽׶Σ����������ȳ�����������
	���Ը���ʱ��Ὣ��������ģ������ΪMetaData������MetaData��ΪMetaFunction��typedef����������Ϊ��type
	������Ϊ��ר�ŵ�MetaData���洢�����������˳����ظ����������
	����һ���׳Ƶ�Լ�������Ԫ��̿ⶼ��ѭ��һԼ��
*/

// ��bool_constant��ΪMetaData
template<bool b>struct bool_constant { static constexpr bool value = b; };
// ��is_referenceת����MetaFunction
template<typename T> struct is_reference2		{ using type = bool_constant < false >; };
template<typename T> struct is_reference2<T&>	{ using type = bool_constant < true >; };
template<typename T> struct is_reference2<T&&>	{ using type = bool_constant < true >; };


// һ�����͵�MetaData
template<typename T, T v>
struct integral_constant
{
	static constexpr T value = v;// �洢����
	
	using value_type = T;// ��׼����Ƕ������������������stl
	using type = integral_constant;// ��׼��meta��������������MetaFunction
	constexpr operator value_type()const noexcept { return value; } // ֧����value_type����ʽת��
	constexpr value_type operator()()const noexcept { return value; } // ֧�ֺ���ʽ����
};

// ���Ƕ�������bool_constant�����Ը�дΪ��
template<bool B> using bool_constant = integral_constant<bool, B>; // ����ģ�壬һ��������typedef��Ҳ��alias template
using true_type  = bool_constant<true>;
using false_type = bool_constant<false>;

// ����is_reference�Ķ�����Ը�Ϊ��
template<typename T> struct is_reference3 { using type = false_type; };
template<typename T> struct is_reference3<T&> { using type = true_type; };
template<typename T> struct is_reference3<T&&> { using type = true_type; };

/*
	����ʹ�÷�ʽ��
	is_reference3<int>::type::value;	ʹ��value
	is_reference3<int>::type();			ʹ������������ʽת�ͳ�value_type
	is_reference3<int>::type()();		ʹ����������.operator()
*/

/*
	��ΪԼ��MetaData��type��typedef��������Ҳ��������һ��MetaFunction�����������Ƶ���Ϊ
	��ô�����Ƕ��һ�٣�����Ϊ��д���ٵĴ��롣������һ���ϣ�Ԫ��̵Ŀ����������������伫��
	����ֱ�ӽ���һ��MetaFunction��Ϊ����ֵ��MetaFunction�����Բ���typedef������ֱ�Ӽ̳�
	���ң��̳е����Ĳ���ֻ��type��valueҲһ���̳У������ʵ�־�ֱ��ʡ��Ϊ{}����������using
*/
template<typename T> struct is_reference4 : public false_type {};
template<typename T> struct is_reference4<T&> : public true_type {};
template<typename T> struct is_reference4<T&&> : public true_type {};

/*
	�Ա��ĸ��汾��is_reference��һ�����ݽ��������մﵽ�˺�stl���Ƶİ汾
	�м������integral_constant��true_type��false_typeҲ������ģ�����ʹ��

	����Ҳ�����Լ��������󹹽��Լ���Ԫ���ģ��
	���磬ϣ��ʵ��һ��Meta Function�������ж��Ƿ���int����������
*/
template<typename T> struct is_int_or_reference		 : public is_reference4<T> {};
template<>			 struct is_int_or_reference<int> : public true_type {};


// ��������Ԫ��̿��ﶼ������һ����򵥵�MetaFunction
template<typename T> struct type_identity { using type = T };
/*
	����һ���βΣ��������䱾������ʲô�ã�
	using type = T�ǲ��Ǻ���Ϥ�����ǵ�remove_reference�������汾�ﶼ����ôһ��using��
	֮ǰ�ᵽ����Ԫ��̿��������ü̳���������using��ʹ��ʵ��Ϊ��{}
	��ô����type_identity��Ҳ���ܶ�remove_reference������
*/

template<typename T> struct remove_reference2		: public type_identity<T> {};
template<typename T> struct remove_reference2<T&>	: public type_identity<T> {};
template<typename T> struct remove_reference2<T&&>  : public type_identity<T> {};

//type_identity�ô��򵥶�ǿ���Ѿ���C++20�����뵽��׼���У���


/*
	���⣬��������׺_v�ı���ģ�����򻯴�MetaFunction���ȡ�����Ĵ���
	����׺_t�ı���ģ�����򻯴�MetaFunction��ȡtype�Ĵ���
*/
template<typename T> inline constexpr bool is_reference_v = is_reference4<T>::value;
template<typename T> using remove_reference_t = typename remove_reference2<T>::type;


/*
	һЩ�򵥵Ķ����ģ��
*/
// ���������Ƿ���ͬ
template<typename T, typename U> struct is_same		  : public false_type {}; // ��ģ�壬�̳�false_type
template<typename T>			 struct is_same<T, T> : public true_type {}; // ƫ�ػ�������������ͬʱƥ�䣬�̳�true_type

// ��һ�������Ƿ��Ǻ�����������е�����һ�����õ��˿ɱ䳤ģ�����
template<typename T, typename _1, typename ...Rest>
struct is_one_of : public bool_constant < is_one_of<T, _1>::value || is_one_of<T, Rest...>::value > {};
template<typename T, typename U> 
struct is_one_of : public is_same<T, U> {};

/*
	���ǳ��˳���ģ��ݹ�����ĵڶ��ֵݹ鷽ʽ���ɱ䳤ģ������ݹ�
	���õ�ʣ�����������ƣ��ظ��ݹ��������βε�is_one_of�����ս�T���������в�������is_same���ж�

	���� ע�� || ��������ͨ������������ʱ�����ǰ��ı��ʽΪtrue���Ͳ������ִ�к���ı��ʽ
	�����ڱ����ڣ�ģ�����ʱ�������ڶԱ��ʽ��ֵ��������ȷ������������||�������ģ����Ȼ�ᱻʵ����
*/

//����is_one_of��������ʵ��ĳЩ���ӵ�MetaFunction
template<typename T>
using is_integral = is_one_of<T, bool, char, short, int, long, long long>; // �ж�T�Ƿ�ʱ����


/*
	is_instantiation_of
	�ж�һ�������Ƿ�ʱһ����ģ��ľ���
	��Ȼ��Ҫ�����βΣ����͡���ģ�����͡�
	ע�⣬����ģ����Ϊ�����β�ʱ��typenameд����template֮��

	Ĭ������ģ��Ϊ��ƥ�䣬���Լ̳�false_type��Inst<...>��Tmpl��ƥ�䣬��������ƥ�䡣��ģ��������ԵĻ��������ֱ�ӳ�������ֻ��������ƥ�䣩
	�ػ��汾Ϊƥ�䣬���Լ̳�true_type�����ڲ�֪�������м����βΣ������ÿɱ䳤����ARGS...������ƥ��Ľ��Ӧ���ǣ� Inst<ARGS...> == Tmpl<ARGS...>
	���Ե�һ������ΪTmpl<ARGS...>���ڶ�������ΪTmpl
*/
template<typename Inst, template<typename ...>typename Impl>
struct is_instantiation_of :public false_type {};

template<template<typename ...> typename Tmpl, typename ...ARGS>
struct is_instantiation_of<Tmpl<ARGS...>, Tmpl> :public true_type {};

/*
	conditional��Ԫ����е�if-else
	����traits��Ϻ������أ�Ҳ��ר�����ڷ�֧�жϵ�ģ��
	������һ��bool�������������ͣ���boolΪtrueʱ���ص�һ������boolΪfalseʱ���صڶ�������
	�����ػ����Ժ����ɵ�ʵ��
*/			 
template<bool B, typename T, typename F> struct conditional				 : public type_identity<T> {};
template<typename T,typename F>			 struct conditional<false, T, F> : public type_identity<F> {};

/*
	����˼���ǣ�conditional����ģ������ػ��У�ֻ����һ��type_identity��
	ֻҪ����type_identity<T>���Ͳ�����type_identity<F>����֮ͬ��
	�����ڲ����ڵ�type_identity<T>��TҲ���ᱻ���󻯡�
	���||������ͬ��||��������û�з������Ǳ�������ȷ��һ������������ǰ����ʽ�������
	��conditional��T��F��Ϊ�βΣ�����ֻ������һ������type_identity��Ҳ��ֻ��һ�������󻯣���һ��ֻ�Ǳ���Ϊ�βΣ�����ƥ�䣬ƥ�������Ͳ����õ�

	�������������Ч������Щֻ����ƥ���βζ�����ʵ���õ�����ģ�壬ʵ������������ʱ||ǰ��true�Ͳ���Ժ�����ֵ��Ч��
	������Ч���ٱ����ʱ��
*/

// ���������ά��
template<typename T>				 struct rank	   :public integral_constant<unsigned int, 0> {};
template<typename T>				 struct rank<T[]>  :public integral_constant<unsigned int, rank<T>::value + 1> {};
template<typename T, unsigned int N> struct rank<T[N]> :public integral_constant<unsigned int, rank<T>::value + 1> {};

// ���������Nά�Ĵ�С
template<typename T, unsigned int N = 1> struct extent : public integral_constant<unsigned int, 0> {};
template<typename T, unsigned int N, unsigned int D> struct extent<T[D], N> : public integral_constant<unsigned int, extent<T, N - 1>::value> {};
template<typename T, unsigned int D> struct extent<T[D], 1> : public integral_constant<unsigned int, D> {};