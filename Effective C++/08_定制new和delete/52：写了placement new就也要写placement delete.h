#pragma once

namespace case52
{
/*
	任何拥有额外参数的 new/delete 都属于 placement new/delete
	但是通常 placement new/delete 指的是第二参数为 void* 的版本

	placement new 可以由程序员自由使用，但是对应版本的 placement delete 通常是用于处理异常的
	因为对于一个指针，程序员和编译器其实都无法知道它是由普通new分配的，还是placement new构造的
	因此根本谈不上调用对应版本的 placement delete，除非：
		1.手动调用，前提是程序员知道，所以不可靠
		2.在使用new的地方，构造函数抛出异常，立刻调用delete，此时由于知道调用了哪个new，所以也能决定调用哪个delete

	换言之， placement delete 是自动调用的，且仅在构造函数异常时被调用

	placement new 是 new 的重载，如果一个class定义了placement new 而没有定义典型 new，
	则placement new会覆盖 ::new 于是这个class只能使用placement来构造
*/

// ::new 的典型格式
void* operator new(size_t size);

// ::delete 的典型格式
void operator delete(void* data);

// placement new 的典型格式
void* operator new(size_t size, void* data);

class Base
{
public:
	// 类的 new 典型格式
	static void* operator new(size_t size);
	
	// 类的 placement new 如果不定义类的典型new，则此类只能用placement new构造，因为::new已被placement new 覆盖
	static void* operator new(size_t size, void* data); 

	// 相比 ::delete 有第二参数size
	static void operator delete(void* data, size_t size);
};

class Derived :public Base
{
public:
	// 覆盖了Base的所有new（见case33）
	static void* operator new(size_t size);
};

}