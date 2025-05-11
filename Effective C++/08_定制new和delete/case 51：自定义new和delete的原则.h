#pragma once
/*
new
	一、必须得到正确的指针，不足时请调用new-handler，没有new-handler时抛出错误
		（因此你必须提供一个真的做了什么事的new-handler，不然就只能寄希望于其他程序退让出内存）
		如果你没法拿到new-handler，可以通过两次set_new_handler取出再放回的方式拿到，但再多线程环境下必须考虑加锁

	二、能有对size为0的情况有正常表现。默认new会在四则为0时申请1大小的内存

	三、对于非希望的size有一般正常表现。如果你专门为一个类定制了operator new，而它又存在子类并且子类没有重写new
		那么子类会继承父类的new。可是Base::operator new是专门为sizeof(Base)准备的，对于Derived，可能表现不佳
		你可以这么做：
			if(size != sizeof(Base)) return ::operator new(size);
		无需担心size为0的情况，因为Base至少有1的大小，所以为0时就会由::new处理

	四、对于member operator new[]，你不能假定有多少元素，因为它可能用于创建子类对象数组，那么变不能通过size / sizeof(Base)来获取数量


delete
	一、确保释放对象是正确的。即
		if(!p)return;

	二、对于member operator delete，如果size不正确则交给::delete以能够有一般表现
		if(size != sizeof(Base))
		{
			::operator delete(p);
			return;
		}
		如果前提是你将Derived的析构函数声明为virtual，否则传给delete的size仍然是Base的大小
*/