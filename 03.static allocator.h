#pragma once
/*
	Ϊ�˽��ÿ���඼���Թ���һ���ڴ����
	����һ����allocatorר�����ڴ���������
	�ĸ�����Ҫ���������Լ���new/delete����ת����allocator����
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
	// ����new/delete��������ת��allocator��allocate��deallocate
	static void* operator new (size_t size) { return myAlloc.allocate(size); }
	static void operator delete(void* p) { myAlloc.deallocate(p); }
};

/*
	���ǣ������κ��ֻ࣬Ҫ��д����Ĵ��룬��������allocator��ʵ���ڴ����
	�������Լ�ʵ���ڴ����

	����ע�⵽��ÿһ������д�Ĵ��붼����ͬ�ģ����
	������д��һ������ǿ������
*/

//��ͨ�ô���д�ɺ�
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