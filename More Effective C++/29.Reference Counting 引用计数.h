#pragma once
#include <string.h>

/*
	多个使用同一个数据的对象不直接持有数据，而是引用已存在的那份
	直到它需要修改数据时才发生复制（cow，copy on write ）
	而发生拷贝时，由于可能还存在其他对象引用这份数据，所以阻止数据释放
	直到没有任何对象引用数据，即 引用计数

	引用数据是为每一个数据准备的，而不是为引用了数据的对象准备的，因此它不能是对象的成员，而是数据的成员

	实现引用计数的对象，一般会声明在其配合使用的对象的private或protected内，并且是struct，以方便访问

	对象在修改数据前，需要判断是否与其他对象共享，如果共享中，则需要复制一份给自己去修改，并解除与旧数据的引用关系
	但是，假如某时刻，对象独占这一个数据，它的成员返回了数据的引用，如operator[]，但是却没有修改数据
	而是把数据的引用保存起来，稍后修改。而在这个稍后之前，有一个新的对象引用了数据
	那么稍后发生的修改会影响到两个对象
	在介绍case30，解决办法是：增加一个可共享标记，一旦某个非const函数被调用，标记设为false
	引用共享标记为false的数据时，总是拷贝
*/

class MyString
{
private:
	struct StringValue
	{
		int ref_count = 0;
		char* data = nullptr;
		bool sharable = true;
		StringValue(const char* init_value)
			: ref_count{ 1 }
		{
			data = new char[strlen(init_value) + 1];
			strcpy(data, init_value);
		}

		~StringValue()
		{
			delete[]data;
		}
	};

	StringValue* value = nullptr;
public:
	// 从字符串常量构造，需创建动态内存。即便已存在相同的string，无法与它们共享引用
	MyString(const char* init_value = "")
		: value{ new StringValue{ init_value } }
	{}

	// 拷贝构造。不发生创建，直接共享引用
	MyString(const MyString& rhs)
	{
		if (rhs.value && rhs.value->sharable)
		{
			++value->ref_count;// 拷贝自其他string，不发生复制，引用计数直接加1
		}
		else
		{
			value = new StringValue{ rhs.value->data };
		}
	}

	// 减少引用计数，如果为0则释放
	~MyString()
	{
		if (0 == --value->ref_count)delete value;
	}

	// 1.证同；2.减少旧的引用；3.共享新的引用
	MyString& operator=(const MyString& rhs)
	{
		if (value == rhs.value)return *this;

		if (0 == --value->ref_count)delete value;

		value = rhs.value;
		++value->ref_count;
		return *this;
	}

	// 下标操作符 const 不会也不能修改数据，返回char
	char operator[](size_t index)const 
	{
		return value->data[index];
	}

	// 下标操作符 非const 可能希望修改数据，因此如果与其他对象共享时，开始复制，返回char&
	char& operator[](size_t index)
	{
		if (value->ref_count > 1)
		{
			value = new StringValue{ value->data };
		}
		value->sharable = false;// 此数据可能被修改，不再与其他对象共享
		return value->data[index];
	}
};

/*
	如何复用引用计数功能的代码，使多个类都拥有此功能
	负责引用计数的是内部的struct，可以设计一个基类让struct继承，以自动拥有计数功能
	有计数还不够，可以发现：增加或减少ref_count的操作大部分还是发生在class里，如何让这部分代码也隐含起来被复用呢？
	class 内部持有的是一个struct*，许多操作都不支持，可以使用一个智能指针，让其构造、析构、拷贝负责操作ref_count
	（一些希望被复用的代码原本植根于目标类中，现在提取出来做一个新的类RCPtr，将并把代码安置在新类的构造、析构、赋值中
	使这些操作自动完成，极大地减少代码量）

	操作ref_count的代码被提取出来后，String类就仿佛什么也没做就能自动引用计数了
	因为它并非是在destructor、costructor、copy、operator= 里不需要再写关于refcount的代码
	而是甚至不需要写 destructor、costructor、copy、operator= 了

	RCObject 的ref_count 初始时总为0。虽然它应该为1，但是为0可以让下文的RCPtr::init少写一个else

	RCPtr 指向RCObject的子类，后者持有数据，但RCPtr在复制时只能使用其copy构造，因此要RCObject的子类自己负责深拷贝
*/

class RCObject
{
public:
	RCObject() = default;

	// RCObject被引用时，是它的指针被复制，并增加ref_count
	// 如果它被复制了，说明正在用new创建新的RCObject，所以ref_count和shareable都是初始值
	/*RCObject(const RCObject& rhs)
		: ref_count{ 0 }
		, shareable{ true }
	{
	}*/

	// 另一种拷贝构造的设计是弃置，这样子类就必须定义拷贝构造，然后只要又只能定义父类的默认构造，从而总是达到重置效果
	RCObject(const RCObject& rhs) = delete;

	// 赋值不会被使用，因为一旦有默认赋值行为，则会失去旧的引用计数、共享标记、数据
	RCObject& operator=(const RCObject& rhs) = delete;

	virtual ~RCObject() = 0;

	void addReference()
	{
		++ref_count;
	}

	void RemoveReference()
	{
		if (0 == --ref_count)delete this;// 在代码设计上，RCObject的子类必然是在heap中的，因此可以这么操作
	}

	void MarkUnshareable()
	{
		shareable = false;
	}

	bool IsShareable()const
	{
		return shareable;
	}

	bool IsShared()const { return ref_count > 1; }

private:
	int ref_count = 0;
	bool shareable = true;
};

// 封装引用数据类的指针
template<typename T>
class RCPtr
{
public:
	RCPtr(T* realPtr = nullptr)
		: pointee{ realPtr }
	{
		init();
	}

	RCPtr(const RCPtr& rhs)
		: pointee{ rhs.pointee }
	{
		init();
	}

	~RCPtr()
	{
		if (pointee)
		{
			pointee->RemoveReference();
		}
	}

	RCPtr& operator=(const RCPtr& rhs)
	{
		if (pointee != rhs.pointee)
		{
			if (pointee)
			{
				pointee->RemoveReference();
			}
			pointee = rhs.pointee;
			init();
		}

		return *this;
	}


	T* operator->()const { return pointee; }
	T& operator* ()const { return *pointee; }
private:
	T* pointee = nullptr;
	void init()
	{
		if (!pointee)return;

		if (!pointee->IsShareable())
		{
			pointee = new T{ pointee }; // 此处调用copy构造，因此要求T必须具备深拷贝的能力
		}

		pointee->addReference();
	}
};

/*
	复用后的MyString
	它除了RCPtr、operator[]成员外，什么都没有
	StringValue不再需要operator= ，因为RCPtr已自顾完成拷贝，它可以使用编译器自带的拷贝
*/
class MyString2
{
private:
	struct StringValue :public RCObject
	{
		char* data = nullptr;
		StringValue(const char* init_value)
		{
			init(init_value);
		}

		StringValue(const StringValue& rhs) // 总是深拷贝，因为共享引用由rcptr负责
		{
			init(rhs.data);
		}

		~StringValue()
		{
			delete[] data;
		}

		void init(const char* init_value)
		{
			data = new char[strlen(init_value) + 1];
			strcpy(data, init_value);
		}

		char  operator[](size_t index) const { return data[index]; }
		char& operator[](size_t index) { MarkUnshareable(); return data[index]; }
	};

	// 当拷贝自其他String时，会自动调用 RCPtr<StringValue>的拷贝构造
	// 如果可共享，则（减少旧数据的引用计数）复制指针，增加计数；
	// 如果不可共享，则（减少旧数据的引用计数）创建并复制数据
	// 这些操作已全部隐含地由RCPtr的构造、拷贝、析构、赋值去执行，所以String什么都不用做
	RCPtr<StringValue> value;
public:
	MyString2() = default;
	MyString2(const MyString2&) = default;
	MyString2(const char* str)
		: value{ new StringValue{str} }
	{
	}

	char  operator[](size_t index) const { return value->operator[](index); }
	char& operator[](size_t index) { return value->operator[](index); }
};

/*
	为任意class增加引用计数功能
	现在只要一个class A继承自RCObject，再让另一个class B内部持有一个RCPtr指向这个class
	在程序中去使用B，就能使A能够被引用计数

	但是让A去继承自RCObject意味着要修改定义，并且定义本身是能够被修改的
	但假如一个class来自第三方库，它无法被修改，如何让它能够被引用计数呢？

	看看字符串本身是什么：它是char[]，它自己不就是一个不继承自其他类、也不能被修改的类型吗？
	而它是作为指针被StringValue持有的，StringValue其实自己也不是个真正的数据类，
	它也是个间接层，指向了真正的数据

	那么对于任何不能修改定义的类，把它视为和char[]类似的资源即可。只需要多一层间接层。
	即：让一个类C继承自RCObject，C有不可修改类的指针成员

	那么MyString其实也不需要有一个内部类StringValue
	可以改为使用RCIPtr<char>，当然字符串的操作有些特殊，所以需要特化
*/

template<typename T>
class RCIPtr
{
public:
	RCIPtr(T* real_ptr = nullptr) : counter{ new CountHolder } { counter->pointee = real_ptr; init(); }
	RCIPtr(const RCIPtr& rhs) : counter{ rhs->counter } { init(); }
	~RCIPtr() { counter->RemoveReference(); }
	RCIPtr& operator=(const RCIPtr& rhs)
	{
		if (counter != rhs.counter)
		{
			counter->RemoveReference();
			counter = rhs.counter;
			init();
		}
		return *this;
	}

	// const
	const T* operator->()const { return counter->pointee; }
	const T& operator* ()const { return *counter->pointee; }

	// 非const 需要cow
	T* operator->() { makeCopy(); return counter->pointee; }
	T& operator* () { makeCopy(); return *counter->pointee; }

private:
	struct CountHolder : public RCObject
	{
		~CountHolder() { delete pointee; }
		T* pointee = nullptr;
	};

	CountHolder* counter = nullptr;

	void init()
	{
		// 此时是初始化，还未增加引用计数
		// 如果不可共享，则复制一份
		if (!counter->IsShareable())
		{
			T* oldValue = counter->pointee;
			counter = new CountHolder;
			counter->pointee = new T{ *oldValue };
		}

		// 增加引用计数
		counter->addReference();
	}

	void makeCopy()
	{
		// 如果数据已被共享，则复制，否则什么都不做
		if (counter->IsShared())
		{
			auto oldValue = counter->pointee;
			counter->RemoveReference();
			counter = new CountHolder;
			counter->pointee = new T{ *oldValue };
			counter->addReference();
		}
	}
};

/*
	总结
		引用对象类：引用一个共享数据的类
		引用数据类：支持被共享，记录被共享次数和可共享标记的数据类

		1.引用计数和共享标记是引用数据类的成员，而非引用对象类的成员
		2.引用数据类应该作为引用对象类的内部struct
		3.为了提高复用性，将引用数据类关于计数和共享状态的逻辑提取到RCObject，所有引用数据类继承自RCObject
		4.RCObject的拷贝构造应该重置引用计数和共享标记，因为它被拷贝发生于希望复制数据；它被共享时只拷贝指针
		5.继承自RCObject的引用数据类必须支持深拷贝，才能达到复制整个数据的效果
		6.为了隐含引用计数的操作，引用对象类不使用原生指针成员，而使用RCPtr成员，它内部有元素指针，指向引用数据类
			RCPtr自顾实现构造、拷贝、析构、赋值等去操作引用数据类的引用次数和共享状态，于是引用对象类就无需关心如何操作
		7.为了给不能修改的类也加上引用计数功能，可以让引用数据类继承RCObject，内部持有该类的指针。这样的增加间接层的类叫RCIPtr
*/

namespace my
{
class RCObject
{
	int ref_count = 0;
	bool shareable = true;
public:
	RCObject() = default;
	RCObject(const RCObject&) = delete;
	RCObject& operator=(const RCObject&) = delete;
	virtual ~RCObject() = 0;

	bool is_shared()const { return ref_count > 1; }
	bool is_shareable()const { return shareable; }

	void add_ref() { ++ref_count; }
	void remove_ref() { if (0 == --ref_count)delete this; }
	void unshareable() { shareable = false; }
};

template<typename T>
class RCPtr
{
	T* pointee = nullptr;
public:
	RCPtr(T* ptr = nullptr) : pointee{ ptr } 
	{
		if (!pointee)return;

		if (!pointee->is_shareable())
		{
			pointee = new T{ pointee };
		}

		pointee->add_ref();
	}

	RCPtr(const RCPtr& rhs)
		: pointee{ rhs.pointee }
	{
		if (!pointee)return;
		
		if (!pointee->is_shareable())
		{
			pointee = new T{ pointee };
		}

		pointee->add_ref();
	}


	RCPtr& operator=(const RCPtr& rhs)
	{
		if (pointee == rhs.pointee)return *this;

		if (pointee)
		{
			pointee->remove_ref();
		}

		pointee = new T{ pointee };
		pointee->add_ref();

		return *this;
	}

	~RCPtr() { pointee->remove_ref(); }

	T* operator->()const { return pointee; }
	T& operator* ()const { return *pointee; }
};

class MyString
{
	struct StringValue : public RCObject
	{
		char* data = nullptr;

		StringValue(const char* str = nullptr)
		{
			data = new char[strlen(str) + 1];
			strcpy(data, str);
		}
		StringValue(const StringValue& rhs)
		{
			data = new char[strlen(rhs.data) + 1];
			strcpy(data, rhs.data);
		}

		~StringValue() { delete[] data; }

		char  operator[](size_t index) const { return data[index]; }
		char& operator[](size_t index) { unshareable(); return data[index]; }
	};

	RCPtr<StringValue> value;

public:
	MyString() = default;
	MyString(const char* str)
		: value{ new StringValue{str} }
	{
	}
	char  operator[](size_t index) const { return value->operator[](index); }
	char& operator[](size_t index)		 { return value->operator[](index); }
};
}