#pragma once
/*
	����malloc��new�����ԣ�������һ�����⣺
		Ƶ��ʹ��malloc����ɴ���cookie���˷��ڴ�

	��Ҳ���ڴ����Ҫ���������

	Ϊ�˼���malloc��ʹ�ô�����������������������
		һ���Է���һ����ڴ棬
		ÿ�����ڴ���������ʱ������һ�����ȡ��һС�鷵��
		�ڴ�黹ʱ��������յ�����ڴ��У��Ա��´μ���ʹ��

	����˼��һ�����⣺
		�ڴ����͹黹����˳�����
	��Ȼ���ڴ����������Զ���ģ�����ܹ�����˳��
	�����ڴ�黹��һ����˳���
	��ˣ�ʹ������ + ͷ�巨��������黹���ڴ��ǱȽ�ʡ�µ�
	ʹ������Ҳ���ڴ����Ĺ���������stlҲ�ǲ�������

	�������ʹ���������ڴ����Ҳ������༼�ɵ�
	���������硢�������Ҳ��ÿ���о��ڴ������˶����뵽������

		����������̬��Ա��1.�����ڴ������ͷ�ڵ㣻2.ÿ���ڴ���������

		�����ڴ�ʱ��������п��õ��ڴ棬��ֱ��ȡͷ�ڵ�
			���û���ˣ��͸��ݾ�̬��Ա�����㹻�������ڴ棬Ȼ�󴮳�����
			��ȡ�������ͷ�ڵ�
		�黹�ڴ�ʱ����������ͷ�巨ֱ�ӽ��黹���ڴ��������ͷ��
*/

class Screen
{
public:
	Screen(int x) : i(x) {}
	int get()const { return i; }

	void* operator new(size_t);
	void operator delete(void*);

private:
	Screen* next;//ָ����һ�������ڴ��
	static Screen* freeStore;//�����ڴ������ͷ�ڵ�
	static const int screenChunk;//ÿ�η����ڴ������

private:
	int i;
};

Screen* Screen::freeStore = 0;
const int Screen::screenChunk = 24;

void* Screen::operator new(size_t size)
{
	Screen* p;
	if (!freeStore)//freeStore�Ѿ��ľ�
	{
		//����Ҫ�����ֽ��ڴ棬������
		size_t chunk = screenChunk * size;
		freeStore = p = (Screen*) new char[chunk];
		
		//ѭ������������
		for (; p != &freeStore[screenChunk - 1]; ++p)
			p->next = p + 1;
		p->next = 0;
	}

	//pȡ����һ���ڴ棬freeStoreָ����һ��
	p = freeStore;
	freeStore = freeStore->next;
	return p;
}

void Screen::operator delete(void* p)
{
	//ͷ�巨����p���ڴ�
	((Screen*)p)->next = freeStore;
	freeStore = (Screen*)p;
}

/*
	��������������union����ʡ��һ��ָ��Ŀռ䣬��˸�Ϊ����
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

	//pȡ�����ͷ�ڵ㣬���pΪ�գ����ٷ���һ����ڴ�
	Airplane* p = headOfFreeList;
	if (p)
		headOfFreeList = p->next;
	else
	{
		//�������ڴ�
		Airplane* newBlock = (Airplane*)::operator new(BLOCK_SIZE * sizeof(Airplane));

		//��������
		for (int i = 1; i < BLOCK_SIZE; ++i)
			newBlock[i].next = &newBlock[i + 1];

		//���һ����next�ÿգ���һ������p���ڶ�����Ϊ����ͷ
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
	���������඼��������new��delete��ʵ������������ڴ����
	���ǣ�����������ؽ����������⣺
		1.��������ڴ棨һ����������黹����Ȼ�����������û���ͷŵ������
			��ϵͳ���ԣ��ڴ���Ȼ��ռ��
		2.��ÿһ���඼Ҫдһ���ڴ������ƣ���������

	����ϣ������������һ���ڴ������ƣ�
		�����Ҳ���ã��Ǹ���Ҳ���ã�����ÿ����дһ��
		�������ڴ����ʱ����̬�ؽ��ڴ�黹�ڴ��ϵͳ
*/