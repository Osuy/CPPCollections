#pragma once
#include "29.Reference Counting 引用计数.h"
/*
	在case 29中面临的一个问题：
		非 const 版本的 operator[] 会引发拷贝
	这是因为 const [] 和非const [] 的匹配并不是基于上下文的读写，而是取决于String对象是否为const
	因此一个非const String对象调用的一定是 非const [] 也就一定会，哪怕它并没有发生写操作，却依然会拷贝

	如果让 [] 返回的只是一个字符的代理就可解决这个问题

	代理的思想在CPTR也有体现：
		为了避免多个对象连+产生大量临时变量，可以让每个+的返回是一个代理/替身对象
		代理对象内部持有一个和数，以及另一个代理对象
		最后连+的结果也是一个代理对象，然后又要 = 赋值给等式左边
		于是代理对象才开始计算真正的结果，递归地从每个子代理对象里把结果计算好
		然后返回给上一层递归，最后赋值
		也就是说真正的计算只发生在赋值前的最后一步，前面的连+只是在建立和数引用关系树

	同样，[]取用字符串中的字符，但是并不知道用于读还是写，那么直接返回“可以被立即使用的字符”就没有必要
	可以返回字符的代理，让字符在被使用（读或写）的前一步才真正被取出，此时也才依情况判断是否发生拷贝
	当[]被调用时，const版本的返回const代理，非const版本返回非const代理
	此时字符还未被使用，仅仅只是得到了代理，但足够了。
	一切读操作都调用隐式转型char
	一切写操作都调用operator= （必然是一个非const 代理。因为const代理是无法调用非const成员的）


	Proxy思想：
		如果一个操作代价很大，就让这个操作在需要时才被真正执行
		而如果一些不是真正需要的情况引发了这个操作
		就使用代理，仅仅只是建立其引用关系，以便在需要时立刻执行，并减少代码差异
		代理的非const成员(如：=)负责真正的执行
		而const成员(如：隐式转型)负责非必须操作

*/

class String
{
public:
	class CharProxy
	{
	public:
		CharProxy(String& str, int index);

		// 非const CharProxy 才能调用
		CharProxy& operator=(const CharProxy& rhs)
		{
			if (theString.value->IsShared())
			{
				theString.value = new StringValue{ theString.value->data };
			}

			theString[charIndex] = rhs;

			return *this;
		}
		CharProxy& operator=(char c)
		{
			if (theString.value->IsShared())
			{
				theString.value = new StringValue{ theString.value->data };
			}
			theString[charIndex] = c;
			return *this;
		}

		// const 和非 const CharProxy 都可以在读时把自身隐式转型为char
		operator char()const { return theString[charIndex]; }
	private:
		String& theString; // CharProxy必须引用一个已存在的字符串，而不能独立使用
		int charIndex;
	};

	friend class CharProxy;

	// const [] 返回 const CharProxy。所以不能调用 = 
	const CharProxy operator[](int index)const
	{
		return CharProxy(const_cast<String&>(*this), index);
	}

	// 非const [] 返回非 const CharProxy，可以读写
	CharProxy operator[](int index)
	{
		return CharProxy(*this, index);
	}

private:
	struct StringValue :public RCObject
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
		char& operator[](size_t index) { MarkUnshareable(); return data[index]; }
	};
	RCPtr<StringValue> value;
};