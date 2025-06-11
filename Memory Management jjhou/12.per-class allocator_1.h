#pragma once
/*
	per-class allocator

	����operator new/delete ÿ�δ�Ԥ����ĳ���ȡ���ڴ�
	��� ��Ϊ�գ���ʹ��new���䣬����������
	�黹ʱ��ֱ�ӽ�ָ����ͷ�巨��������

	�����ʵ�������static��Աά���ڴ��ͷ�ڵ�
	������Ϊ����ڵ㣬��nextָ���Ա�������Ĵ�С������һ��ָ��Ĵ�С
	���ڴ��ڳ���ʱ������ ��һ���ڵ��next��ָ��nextָ����һ���ڴ�
	�ص�ͷ�ڵ㱻ȡ��ʱ��ͷ�ڵ���ƣ�����ԭͷ�ڵ���ڴ�
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