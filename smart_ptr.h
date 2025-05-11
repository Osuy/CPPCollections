#pragma once

/*
	智能指针最重要的sharedptr和weakptr
		它二者是基于引用计数的的智能指针。引用记数就是记录有多少个智能指针指引用了这块资源
		构造时引用记数初始化为1，析构时，引用记数减1，如果减为0，则释放资源。
		而最重要的拷贝构造和赋值操作符
		拷贝构造时，就是浅拷贝，但是引用记数加1
		赋值时，先判断是否自我赋值，然后将自己的引用记数减1，减到0则释放，然后进行浅拷贝，再把对方的引用记数加1，自己赋值对方的引用记数

	单纯使用sharedptr时，可能会出现循环引用的问题。A引用B，B引用A。释放A时，要先释放B，释放B时要先释放A，造成崩溃
	因此，需要一种弱化的智能指针。weakptr在引用资源时，不会参与引用记数，一个资源被释放时，不会引用被一个weakptr引用而被阻止
	
	对于持有关系，使用sharedptr
	对于借用关系，使用weakptr
*/
#include <memory>
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;
//智能指针的使用
void use_unique_ptr()
{
	//使用原生指针初始化
	unique_ptr<int> a(new int(3));

	//使用
	unique_ptr<int> a(new int(3));
	
}

void use_shared_ptr()
{
	shared_ptr<int> a(new int(3));
}

void use_weak_ptr()
{
	//weak_ptr的使用依赖于shared_ptr
	//无法通过原生指针初始化
	//weak_ptr<int> _a(new int(3));

	//通过shared_ptr初始化
	shared_ptr<int> a = make_shared<int>(new int(3));
	weak_ptr<int> b(a);

	//通过weak_ptr初始化
	weak_ptr<int> c(b);

	//通过weak_ptr的右值初始化
	weak_ptr<int> d(weak_ptr<int>());
}
