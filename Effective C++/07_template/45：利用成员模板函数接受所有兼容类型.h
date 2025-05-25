#pragma once

namespace case45
{
/*
	利用成员函数模板接受所有兼容类型

	一个类的成员函数通过重载可以接受不同类型，进行不同的处理

	当这些所有处理都具有高度共性时，如果兼容类型的数量不多，重载还是可以胜任
	但加入希望能兼容尽可能多的类型，那么每次增加一种兼容，就需要重载一份
	更好的做法是一函数模板来接受所有类型，由模板具象化来省去重载的工作，一劳永逸
	前提是对于每种兼容类型都具有共性，如果需要特殊处理，仍然需要重载


	对于像shared_ptr来说，为了能够更好的模仿原生指针，需要向上转型的功能
	然而，shared_ptr<Base>和shared_ptr<Derived>被认为是两种类型，之间没有任何联系
	为了模拟指针的向上转型，需要用到函数模板接受兼容类型，即：泛化的copy构造
*/

template<class T>
class smart_ptr
{
public:
	explicit smart_ptr(T* raw) : _handle{ raw } {} // 原生指针初始化时，通常声明显式

	// 注意：泛化的拷贝构造严格意义上不是拷贝构造，因为不同的类模板实例是不同的类，相当于用一个无关参数来构造
	// 因此拷贝构造和泛化拷贝构造没有关联，互不影响
	smart_ptr(const smart_ptr& other); // 拷贝构造，和泛化的拷贝构造可以同时存在，如果不声明且需要用到，编译器仍然会提供

	template<class U>
	smart_ptr(const shared_ptr<U>& other) //泛化的copy构造，非explicit，因为向上转型是隐式的
		: _handle{ other.get() } // 利用原生指针的转型，如果不能转型，会在编译器报错，因此是安全的
	{
		...
	} // 处理引用计数

  // smart_ptr的其他泛化构造
	template<class U>
	explicit smart_ptr(U* other);
	template<class U>
	explicit smart_ptr(const weak_ptr<U>& other);
	template<class U>
	explicit smart_ptr(auto_ptr<U>& other);// auto_ptr 不能是const，他独有控制权，获取指针后会修改自身指针为空
	template<class U>
	smart_ptr(const smart_ptr<U>& other);

	template<class U>
	smart_ptr(const smart_ptr<U>& other);
	template<class U>
	smart_ptr<T>& operator=(const smart_ptr<U>& other); // 泛化的=操作符

private:
	T* _handle;
};

/*
	注意只有来自shared_ptr的转型是非explicit的，这是只允许自己隐式转换
	不要兼容其他指针的隐式转化，那样于代码实现、安全性、可读性都是无益的
*/
}