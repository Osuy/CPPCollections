#pragma once
/*
	为了解决每个类都可以共用一套内存管理，
	创建一个类allocator专门做内存管理这件事
	哪个类需要，就重载自己的new/delete，跳转调用allocator即可
*/

class allocator
{
public:
	static void* allocate(size_t);
	static void deallocate(void*);
};

class A
{
private:
	int i;

protected:
	static allocator myAlloc;
public:
	// 重载new/delete单纯地跳转到allocator的allocate和deallocate
	static void* operator new (size_t size) { return myAlloc.allocate(size); }
	static void operator delete(void* p) { myAlloc.deallocate(p); }
};

/*
	于是，对于任何类，只要书写上面的代码，就能利用allocator来实现内存管理
	而不必自己实现内存管理

	但是注意到，每一个类书写的代码都是相同的，因此
	可以缩写成一个宏来强化复用
*/

//将通用代码写成宏
#define DECLARE_ALLOCATOR() \
protected:\
	static allocator myAlloc;\
public:\
	static void* operator new (size_t size) { return myAlloc.allocate(size); }\
	static void operator delete(void* p) { myAlloc.deallocate(p); }

class B
{
	DECLARE_ALLOCATOR()
public:
	B(int n) :i(n) {}
private:
	int i;
};