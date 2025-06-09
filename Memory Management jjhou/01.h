#pragma once
#include <stdlib.h>
#include <xmemory>
#include <vector>

/*
	�����ڴ�ķ�����
		1.C��malloc������ϵͳapi��
		2.C++��new��new[]��new()��operator new������malloc��
		3.stl��allocator������new��
		4.ֱ��ʹ�ò���ϵͳ��api��heapalloc��virtualalloc

	malloc
		C�Ŀ⺯�������ɸ���

	new
		C++���ʽ ���ɸ��ǣ��ᱻ���������������Ĵ���

	::new��operator new/new[]/new()
		C++���������ɸ��ǣ���new���ʽʹ��
		new ���ʽʹ�� operator new
		new ����[] ���ʽʹ�� operator new[]
		new(...) ���ʽʹ�� placement new���������������operator new��
*/

class ClassA
{
	int x, y, z;
};

void example1()
{
	void* p1 = malloc(512);
	free(p1);

	ClassA* p2 = new ClassA;
	delete p2;

	void* p3 = ::operator new(512);
	::operator delete(p3);

	int* p4 = std::allocator<int>().allocate(3); // 3��int��С���ڴ�
	std::allocator<int>().deallocate(p4, 3); // �黹ָ�룬���һ�Ҫָ������֮ʱ�������С
/*
	Ե��allocator���ͷ�deallocate����Ҫָ�룬����ָ���黹��size
	���ǲ��ʺ�ֱ��ʹ�á���Ϊ�����Ѿ������û��޴�֪�����׷�����
	������٣��黹Ҳ�ֺ����ٵع黹��Ψ������������
*/
}



/*
	new ���ʽ�����������Ϊ���´��룺
		1.����operator new��ʹ��sizeof��ȡ��Ĵ�С
		2.����static_castת��Ϊnew��������͵�ָ��
		3.�����๹�캯��
	����ʵ�ϻ���һ��try-catch���ڲ����쳣��
*/

void example2()
{
	// new ���ʽ�ķ���
	ClassA* ptr = nullptr;
	try {
		void* mem = operator new(sizeof(ClassA));
		ptr = static_cast<ClassA*>(mem);
		ptr->ClassA(); // ��һ��ֻ�б���������ɣ���placement new��
	}
	catch (std::bad_alloc)
	{
	}

	// ��֤�ֶ����ù��������Ŀ�����
	{
		using std::vector;
		vector<int>* vec = new vector<int>;
		vec->vector<int>(); // ������ָ����ù��캯��
		vec->~vector<int>(); // ������ָ�������������
	}

	// delete ���ʽ�ķ���
	ptr->~ClassA();// �ֶ����������ǿ��Ե�
	operator delete(ptr);
}

void example3()
{
	ClassA* ptr = new ClassA[3];
/*
	����array new û�л��ᴫ�Σ������������Ĭ�Ϲ���
	�����ҪΪÿ��������÷�Ĭ�Ϲ��죬������new���ʽ����
	ѭ����ÿ������ĵ�ַʹ��placement new
*/

/* 
	��������ʹ��new[] / delete[]
	���ʹ��new[] ����Ӧʹ��delete
	����������ֻ�����һ�Σ�ǰ�������������壩
	���������ڴ���Ȼ�ᱻ���գ������ڴ�й¶�ĵط����Ǳ����ڴ�
	���Ƕ����ڲ��Ķ�̬�ڴ�
	���ڽ���һ�������ĵ��ã��������Ķ�̬�ڴ�ͻ�й¶

	delete[]����������˳�����ɺ���ǰ����new[]���ù����˳���෴
*/
	delete[] ptr;

/*
	new/new[]������ڴ棬���˶��������С�������ж������Ϣ����ͨ����
		1.��βcookie��2�ֽ�
		2.debugheader 32�ֽ�
		3.�ܴ�С����Ϊ16�ı������������ֽ�

	����new[]�����������������壬����new������ڴ��ǰ�˻���һ���ֽ����ڼ�¼����ĳ���
	����������Ⱦ���֪�����ö��ٴι��������
	���������������壬�򲻻��������¼���ȵ��ֽڣ�������������ڴ�Сһ�ֽ�
*/
}

void example4()
{
	// placement new
	char* buf = new char[sizeof(ClassA)];
	ClassA* ptr = new(buf) ClassA{}; // placement new�� ��new������ţ�������������ͨ����һ���Ѿ�����õ�ָ�룬��������
	
	// placement new �ķ���
	ClassA* ptr = nullptr;
	try {
		void* mem = operator new(sizeof(ClassA), buf); // ��buf���� operator new���������أ����Զ����ö�Ӧ�İ汾
		ptr = static_cast<ClassA*>(mem);
		ptr->ClassA();
	}
	catch (std::bad_alloc)
	{
	}

	// placement new �İ汾ʵ����ʲô�������������ѵڶ��������ء�������Ҫ�Լ���֤������ڴ��ַ����Ч��
}

/*
	�ܽ�new���ʽ��operator new
	operator new�Ƿ����ڴ�Ĺ��ߣ����漰ת�ͺ͵��ù�������
	����ţ�Ƶ���new���ʽ�����ᱻ����������Ϊ�����Ӿ���Ĵ��룬��
	���ֳ������������������
*/