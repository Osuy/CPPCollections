#pragma once
#include <stdlib.h>
#include <xmemory>
#include <vector>
#include <iostream>

/*
	�����ڴ�ķ�����
		1.C �� malloc ������ϵͳapi��
		2.C++�� operator new ������malloc���������ڴ档�����ڴ沢���������new���ʽ��
		3.stl �� allocator ������new��
		4.ֱ��ʹ�ò���ϵͳ��api��heapalloc��virtualalloc

	malloc
		C �Ŀ⺯�������ɸ���

	new
		C++ ���ʽ ���ɸ��ǣ��ᱻ���������������Ĵ���

	operator new/new[]/new()
		C++���������ɸ��ǣ���new���ʽʹ��
		new ���ʽʹ�� operator new
		new ����[] ���ʽʹ�� operator new[]
		new(...) ���ʽʹ�� placement new ��������������� operator new ��
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
		1.����operator new ��ʹ�� sizeof ��ȡ��Ĵ�С
		2.����static_castת��Ϊ��ָ��
		3.�����๹�캯��
	����ʵ�ϻ���һ�� try-catch ���ڲ����쳣��
*/

void example2()
{
	// new ���ʽ�ķ���
	ClassA* ptr = nullptr;
	try {
		void* mem = operator new(sizeof(ClassA));
		ptr = static_cast<ClassA*>(mem);
		//ptr->ClassA(); // ��һ��ֻ�б���������ɣ���placement new��
	}
	catch (std::bad_alloc)
	{
	}

	// ��֤�ֶ����ù��������Ŀ�����
	{
		using std::vector;
		vector<int>* vec = new vector<int>;
		//vec->vector<int>(); // ������ָ����ù��캯��
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
	�����ҪΪÿ��������÷�Ĭ�Ϲ��죬����
	ѭ����ÿ������ĵ�ַʹ��placement new
*/

/* 
	��������ʹ��new[] / delete[]
	���ʹ��new[] ����Ӧʹ��delete
	����������ֻ�����һ�Σ�ǰ�������������壩
	���������ڴ���Ȼ�ᱻ���գ������ڴ�й¶�ĵط����Ǳ����ڴ�
	���Ƕ����ڲ��Ķ�̬�ڴ�
	���ڽ���һ�������ĵ��ã��������Ķ�̬�ڴ�ͻ�й¶

	delete[]����������˳�����ɺ���ǰ����new[]���ù����˳���෴��Ϊ��ʹdelete��new������̣�
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
	try {
		void* mem = operator new(sizeof(ClassA), buf); // ��buf���� operator new���������أ����Զ����ö�Ӧ�İ汾
		ptr = static_cast<ClassA*>(mem);
		//ptr->ClassA();
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

/*
	����operator new
	��Ȼnew���ʽ�����operator new
	���л�Ե�����ɳ���Ա������operator new�Կ����ڴ�����ϸ�ڣ�������ڴ����

	���ص�Ŀ����������
		1.global�����ռ��µ�operator new ���� ::operator new
		2.ÿ���������ռ��µ�operator new�� �� XX::operator new

	�������еĻ������ͺ�û�ж����Աoperator new���඼��ʹ�� ::operator new ��C++�ĺ���ƥ����ƣ�
	��������::operator new��Ӱ��ʮ����Զ���ǳ��ټ�
	һ�����������������س�Աoperator new����
*/

/*
	����::operator new
*/

void* operator new(size_t size)
{
	std::cout << "::" << __func__ << std::endl;
	return malloc(size);
}

// inline void* operator new(int size); // ��һ����������size_t

void* operator new[](size_t size)
{
	std::cout << "::" << __func__ << std::endl;
	return malloc(size);
}

void operator delete(void* mem)
{
	std::cout << "::" << __func__ << std::endl;
	return free(mem);
}

void operator delete[](void* mem)
{
	std::cout << "::" << __func__ << std::endl;
	return free(mem);
}

/*
	���Աoperator new
	������static����Ϊnew����֮ǰ���󲻴��ڣ��޷��ù������� ֮thisָ��������
	���ǿ���ʡ��
*/
class ClassB
{
	int var = 0;
public:
	ClassB()
	{
		std::cout << "ClassB::" << __func__ << ". this = " << this << " var = " << var << std::endl;
	}

	ClassB(int v) : var{ v }
	{
		std::cout << "ClassB::" << __func__ << ". this = " << this << " var = " << var << std::endl;
	}

	~ClassB()
	{
		std::cout << "ClassB::" << __func__ << " this = " << this << " var = " << var << std::endl;
	}

	inline void* operator new(size_t size)
	{
		std::cout << "ClassB::" << __func__ << ". size = " << size << std::endl;
		auto mem = ::operator new(size);
		std::cout << "return = " << mem << std::endl;
		return mem;
	}

	inline void* operator new[](size_t size)
	{
		std::cout << "ClassB::" << __func__ << ". size = " << size << std::endl;
		auto mem = ::operator new[](size);

		std::cout << "return = " << mem << std::endl;
		return mem;
	}

	inline void operator delete(void* mem, size_t)
	{
		std::cout << "ClassB::" << __func__ << ". mem = " << mem << std::endl;
		return ::operator delete(mem);
	}

	inline void operator delete[](void* mem, size_t)
	{
		std::cout << "ClassB::" << __func__ << ". mem = " << mem << std::endl;
		return ::operator delete[](mem);
	}
};

void example5()
{
	std::cout << "part 1" << std::endl;
	auto pb = new ClassB{ 1 };

	std::cout << std::endl << "part 2" << std::endl;
	delete pb;

	std::cout << std::endl << "part 3" << std::endl;
	auto pba = new ClassB[3];
	auto tmp = pba;
	for (int i = 0; i < 3; ++i)
	{
		::new(tmp++) ClassB{ i }; // ����ClassB ������operator new ��û������placement new �����ں������ǹ���������ʹ��placement new ���˴���ȫ�ֵ�placement new ���
	}

	std::cout << std::endl << "part 4" << std::endl;
	delete[] pba;
}

/*
	placement new
	��������operator new�����ء���˻ᱻ�����ص�operator new����
	��Ȼ�����أ�������Ҫ������ͬ������İ汾�Ƕ���һ��void*��������Ĭ���ṩ
	���ǲ�������placement newֻ�ܴ�void*
	���Ը�����Ҫ������������� placement new
*/

class ClassC
{
public:
	ClassC()
	{
		std::cout << "ClassC::" << __func__ << std::endl;
	}
	ClassC(int)
	{
		std::cout << "ClassC::" << __func__ << std::endl;
		throw std::exception();
	}

	inline void* operator new(size_t size)
	{
		std::cout << "ClassC::" << __func__ << "(size_t). size = " << size << std::endl;
		auto mem = ::operator new(size);

		std::cout << "return = " << mem << std::endl;
		return mem;
	}

	// placement new �ĳ���汾���ڶ�������void*
	inline void* operator new(size_t size, void* mem)
	{
		std::cout << "ClassC::" << __func__ << "(size_t, void*). size = " << size << std::endl;
		std::cout << "return = " << mem << std::endl;
		return mem;
	}

	// placement new �ڶ�������size_t
	inline void* operator new(size_t size, long extra_size) // �����extra_size������ Ϊʲô��дΪsize_t��
	{
		std::cout << "ClassC::" << __func__ << "(size_t, size_t). size = " << size + extra_size << std::endl;
		auto mem = ::operator new(size + extra_size);
		std::cout << "return = " << mem << std::endl;
		return mem;
	}

	// placement new �ڶ�������size_t
	inline void* operator new(size_t size, long extra_size, char c) // �����extra_size������ Ϊʲô��дΪsize_t��
	{
		std::cout << "ClassC::" << __func__ << "(size_t, size_t, char). size = " << size + extra_size << std::endl;
		auto mem = ::operator new(size + extra_size);
		std::cout << "return = " << mem << std::endl;
		return mem;
	}

	inline void operator delete(void* mem, size_t)
	{
		std::cout << "ClassC::" << __func__ << "(void*, size_t). mem = " << mem << std::endl;
		return ::operator delete(mem);
	}

	// ��Ӧplacement��Ҳ��Ҫд����Ӧ�� placement delete�������Ǿ������κ�delete���ʽ���á���Effective C++ case 52
	inline void operator delete(void* mem, void*)
	{
		std::cout << "ClassC::" << __func__ << "(void*, void*). mem = " << mem << std::endl;
		return ::operator delete(mem);
	}

	inline void operator delete(void* mem, long)
	{
		std::cout << "ClassC::" << __func__ << "(void*, long). mem = " << mem << std::endl;
		return ::operator delete(mem);
	}

	inline void operator delete(void* mem, long, char)
	{
		std::cout << "ClassC::" << __func__ << "(void*, long, char). mem = " << mem << std::endl;
		return ::operator delete(mem);
	}

	/*
		Ϊʲôplacement delete �ĵڶ����������Ͳ������� size_t��
		��Ϊoperator delete �Ĳ����б��� (void* mem, size_t)�������� placement delete�� size_t ���Ϊ placement new �Ķ����������
		���placement new �ĵڶ���������� size_t������ڰ汾��placement delete�ͺ�operator delete�ĸ�ʽ��ȫһ��
		Ҳ�������ڶ��ڵ�placement delete
	*/
};

void example6()
{
	ClassC c1;
	auto pc1 = new ClassC;
	auto pc2 = new(&c1) ClassC;
	auto pc3 = new(100) ClassC;
	auto pc4 = new(100, 'a') ClassC;
	// ���¶���ʹ��operator delete ȥ�ͷ� placement new������ڴ棨����pc2����û������
	delete pc1;
	delete pc3;
	delete pc4;

	// ��֤�쳣�Ƿ��ܵ���placement delete
	// ���ô��׳��쳣�Ĺ���
	auto pc5 = new ClassC{ 1 };
	auto pc6 = new(&c1) ClassC{ 1 };
	auto pc7 = new(100) ClassC{ 1 };
	auto pc8 = new(100, 'a') ClassC{ 1 };
	// ���´���������Ϊ�ڴ��Ѿ���placement delete����
	delete pc5;
	delete pc7;
	delete pc8;
}

/*
	placement deleteΨ����ʹ���� placement new ���캯���׳��쳣ʱ�Żᱻ����
	ҲΨ�������������ֱ��ʹ�õ������� placement new ����Ϊ������֪��
	��� ������  placement new ȴ�������Ӧ�汾��  placement delete����ζ��Ҫ�����쳣����
*/

/*
	��׼���string�ڲ������ַ������⣬������һ�����ü�������
	���Ҳʹ��replacement new���ɣ�Ϊÿ��string�����������Ĵ�С���Թ������ü�������
*/