#pragma once
#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;

/*
	智能指针的使用
*/

void use_unique_ptr()
{
	unique_ptr<int> a(new int(3));
	cout << *(a.get()) << endl;

	a.reset(new int(4));
	cout << *(a.get()) << endl;
	
	a.reset();
	if (a.get())
		cout << *(a.get()) << endl;
	else
		cout << "a hold nothing" << endl;
	
	unique_ptr<int> b(new int(2));
	cout << *(b.get()) << endl;
	
	a.swap(b);
	cout << *(a.get()) << endl;
	if (b.get())
		cout << *(b.get()) << endl;
	else
		cout << "b hold nothing" << endl;
	
	a.release();
	if (a.get())
		cout << *(a.get()) << endl;
	else
		cout << "a hold nothing" << endl;


	//unique_ptr<int> b = a;
}

void use_shared_ptr()
{
	shared_ptr<int> a;
	cout << a.use_count() << endl;
	if (a.get())
		cout << *(a.get()) << endl;
	else
		cout << "a hold nothing" << endl;

	shared_ptr<int> b(new int(3));
	cout << b.use_count() << endl;
	a.reset(b.get());
	cout << a.use_count() << endl;
	cout << b.use_count() << endl;
}

void use_weak_ptr()
{}

/*
	RAII思想：资源获取时就进行初始化
	C++中，程序员通过new和delete在堆上分配和释放资源，由于这个过程不是自动的，因此容易造成内存泄漏。
	智能指针是一类有着指针行为的对象
*/

/*
	auto_ptr
	只做单纯的持有、释放等功能
	提供了一般构造，拷贝构造，=重载，析构函数
	
	问题：
		auto_ptr在设计上是独占指针的，但是允许将一个指针给多个auto_ptr初始化
		因此所有权的概念模糊。

		允许转移所有权，但是不提示，原auto_ptr在交出所有权后如果继续使用将会出错
		
*/
template<class _Ty>
class auto_ptr
{
public:
	typedef _Ty element_type;

	//通过原生指针初始化
	//显式单参构造，防止隐式类型转换的初始化
	explicit auto_ptr(_Ty* _Ptr = 0) noexcept
		: _Myptr(_Ptr)
	{
	}

	//通过另一个auto_ptr转交所有权初始化
	auto_ptr(auto_ptr& _Right) noexcept
		: _Myptr(_Right.release())
	{
	}

	// 通过其他类型的auto_ptr类型转换来初始化
	// 以下模板没有参数，必须显式指定模板参数
	// 由于重载了auto_ptr
	// 行为和构造函数十分相似，用于持有的指针的类型转换
	template <class _Other>
	operator auto_ptr<_Other>() noexcept { // convert to compatible auto_ptr
		return auto_ptr<_Other>(*this);
	}

	//拷贝构造
	template <class _Other>
	auto_ptr(auto_ptr<_Other>& _Right) noexcept : _Myptr(_Right.release()) {}

	//=重载
	auto_ptr& operator=(auto_ptr& _Right) noexcept {
		reset(_Right.release());
		return *this;
	}

	//return持有的指针，_Myptr指向空
	_Ty* release() noexcept {
		_Ty* _Tmp = _Myptr;
		_Myptr = nullptr;
		return _Tmp;
	}

	//析构函数，释放持有的指针所指向的内存
	~auto_ptr() noexcept {
		delete _Myptr;
	}

private:
	element_type _Myptr;
};


/*
	unique_ptr
	独占指针，一个指针同一时刻只能被一个unique_ptr持有
	解决了auto_ptr的所有权问题

	auto_ptr不是有release支持转交所有权吗？为什么说不能单独持有？
	auto_ptr的所有权转交依赖于程序员手动，换言之，程序员如果心思缜密，
	是可以手动让auto_ptr独占一个指针的。但是unique_ptr可以自动完成独占

	具体实现就是在auto_ptr的基础上，禁用了拷贝构造和=重载，允许移动构造

	auto_ptr可以使用指针初始化，但是unique_ptr只能使用右值初始化
	而一个右值是不可能同时出现在两句代码中的。也就是unique_ptr持有了这个右值后，
	其他unique_ptr只能通过拷贝和=号获得所有权。于是同一时间只有一个unique_ptr持有这个右值
*/


/*
	shared_ptr

*/