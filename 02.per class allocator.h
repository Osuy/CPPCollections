#pragma once
/*
	根据malloc和new的特性，引出了一个问题：
		频繁使用malloc会造成大量cookie而浪费内存

	这也是内存管理要解决的问题

	为了减少malloc的使用次数，可以有这样的做法：
		一次性分配一大块内存，
		每当有内存分配的请求时，从者一大块中取出一小块返回
		内存归还时，将其回收到大快内存中，以便下次继续使用

	首先思考一个问题：
		内存分配和归还，是顺序的吗？
	显然，内存分配可以是自定义的，因此能够做到顺序
	但是内存归还不一定是顺序的
	因此，使用链表 + 头插法，来管理归还的内存是比较省事的
	使用链表也是内存管理的公认做法，stl也是采用链表

	但是如何使用链表来内存管理，也是有许多技巧的
	下面是最早、最初级，也是每个研究内存管理的人都会想到的做法

		定义两个静态成员：1.可用内存链表的头节点；2.每次内存分配的数量

		分配内存时，如果还有可用的内存，就直接取头节点
			如果没有了，就根据静态成员分配足够数量的内存，然后串成链表，
			再取出链表的头节点
		归还内存时，利用链表头插法直接将归还的内存插入链表头部
*/

class Screen
{
public:
	Screen(int x) : i(x) {}
	int get()const { return i; }

	void* operator new(size_t);
	void operator delete(void*);

private:
	Screen* next;//指向下一个可用内存快
	static Screen* freeStore;//可用内存链表的头节点
	static const int screenChunk;//每次分配内存的数量

private:
	int i;
};

Screen* Screen::freeStore = 0;
const int Screen::screenChunk = 24;

void* Screen::operator new(size_t size)
{
	Screen* p;
	if (!freeStore)//freeStore已经耗尽
	{
		//计算要多少字节内存，并分配
		size_t chunk = screenChunk * size;
		freeStore = p = (Screen*) new char[chunk];
		
		//循环串联成链表
		for (; p != &freeStore[screenChunk - 1]; ++p)
			p->next = p + 1;
		p->next = 0;
	}

	//p取出第一块内存，freeStore指向下一块
	p = freeStore;
	freeStore = freeStore->next;
	return p;
}

void Screen::operator delete(void* p)
{
	//头插法回收p的内存
	((Screen*)p)->next = freeStore;
	freeStore = (Screen*)p;
}

/*
	下面这种利用了union，节省了一个指针的空间，因此更为巧妙
*/

class Airplane
{
private:
	struct AirplaneRep
	{
		unsigned long miles;
		char type;
	};

	union 
	{
		AirplaneRep rep;
		Airplane* next;
	};

private:
	static const int BLOCK_SIZE;
	static Airplane* headOfFreeList;

public:
	unsigned long getMiles()const { return rep.miles; }
	char getType()const { return rep.type; }
	void set(unsigned long m, char t) { rep.miles = m; rep.type = t; }

public:
	static void* operator new(size_t);
	static void operator delete(void*, size_t);

};

Airplane* Airplane::headOfFreeList;
const int Airplane::BLOCK_SIZE = 512;

void* Airplane::operator new(size_t size)
{
	if (size != sizeof(Airplane))
		return ::operator new(size);

	//p取链表的头节点，如果p为空，则再分配一大块内存
	Airplane* p = headOfFreeList;
	if (p)
		headOfFreeList = p->next;
	else
	{
		//分配大块内存
		Airplane* newBlock = (Airplane*)::operator new(BLOCK_SIZE * sizeof(Airplane));

		//串成链表
		for (int i = 1; i < BLOCK_SIZE; ++i)
			newBlock[i].next = &newBlock[i + 1];

		//最后一个的next置空，第一个交给p，第二个作为链表头
		newBlock[BLOCK_SIZE - 1].next = 0;
		p = newBlock;
		headOfFreeList = &newBlock[1];
	}
	return p;
}

void Airplane::operator delete(void* deadObject, size_t size)
{
	if (deadObject == 0)return;
	if (size != sizeof(Airplane))
	{
		::operator delete(deadObject);
		return;
	}

	Airplane* carcass = (Airplane*)deadObject;
	carcass->next = headOfFreeList;
	headOfFreeList = carcass;
	return;
}

/*
	以上两种类都利用重载new和delete来实现了类自身的内存管理
	但是，还存在两个亟待解决的问题：
		1.分配大量内存（一百万个），归还后仍然留在链表里，而没有释放掉，因此
			对系统而言，内存依然被占用
		2.对每一个类都要写一套内存管理机制，不够泛用

	我们希望的是有这样一套内存管理机制：
		这个类也能用，那个类也能用，不必每个类写一套
		当空闲内存过多时，动态地将内存归还内存给系统
*/