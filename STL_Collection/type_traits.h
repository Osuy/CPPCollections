#pragma once

/*
	remove_reference就是利用偏特化和traits技巧来实现的
	不论是常规、引用、右值引用，通过remove_reference<ClassXXX>::Type就能获取其原本的数据类型
*/
template <class T>
struct remove_reference
{
	typedef T Type;
};

//左值引用偏特化
template <class T>
struct remove_reference<T&>
{
	typedef T Type;
};

//右值引用偏特化
template <class T>
struct remove_reference<T&&>
{
	typedef T Type;
};