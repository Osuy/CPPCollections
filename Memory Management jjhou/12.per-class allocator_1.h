#pragma once
/*
	per-class allocator

	重载operator new/delete 每次从预分配的池里取出内存
	如果 池为空，则使用new分配，并串成链表
	归还时，直接将指针以头插法插入链表

	链表的实现是类的static成员维护内存池头节点
	自身作为链表节点，有next指针成员，因此类的大小膨胀了一个指针的大小
	当内存在池内时，它被 上一个节点的next所指，next指向下一块内存
	池的头节点被取出时，头节点后移，返回原头节点的内存
*/

class Screen
{
private:
	double var;
	Screen* next = nullptr;
	static Screen* freeStore;
	enum { screenChunk = 24 };
public:
	void* operator new(size_t);
	void operator delete(void*, size_t);
};