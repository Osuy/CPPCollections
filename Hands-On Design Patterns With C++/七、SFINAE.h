#pragma once
#include <vector>

/*
	��������ƥ�����ȼ���
		1.������ȫƥ���ģ�庯��
		2.����ʽת���¿�ƥ��ĺ���ģ��
		3.����ʽת���´��ڶ����ƥ��ĺ���ģ��ʱ��ѡ��������һ��
		4.û��ƥ��ĺ���ģ��ʱ��������ʽת�ͺ����ƥ��ķǺ���ģ��
		5.�ɱ䳤��������

	��û����ȫƥ�䣬��û�к���ģ��ʱ���ᷢ����ʽת��
	��ʱ������ڶ��ת�ͺ���õ����أ��ͻᷢ��ambiguous�����Ա����ж��ʵ��������б�ƥ��
	��ֵ�����У�double������������Ͳ�����������
	������float��int�βΣ�doubleʵ��ת�������壻����double��int�βΣ�floatʵ��ת�Ͳ��������壩
*/
void func(int) {}
void func(long) {}
void func(float) {}
void test()
{
	//func(5u); // error: func(int) or func(long)
	//func(5.0); // error: func(int) or func(long) or func(float)
}
/*
	����ģ������ȼ�����ת��ƥ�䣬���ǿ���Ϊ���ܴ��ڶ����Դ���ĺ������庯��ģ������
*/
void func2(int) {}
void func2(long) {}
template<typename T> void func2(T) {}

void test2()
{
	func2(5u);// func(T)
}

/*
	����ģ�����ؿ����ж����������ƥ�������İ汾
	������Ϊ������ģ���ƫ�ػ�
*/

template<typename T> void func3(T) {}
template<typename T> void func3(T*) {}
template<typename T> void func3(std::vector<T>) {}

void test3()
{
	int i = 0;
	func3(i);
	func3(&i);// ʵ��Ϊint*��func3(T*) �� func3(T)������
	std::vector<int> arr;
	func3(arr);// ʵ��Ϊstd::vector<int>��func3(std::vector<T>) �� func3(T)������
}

/*
	��ʽת�Ͳ��ᷢ���ں���ģ����
	����ʾָ������ģ������󣬺���ģ��;��廯Ϊ����ͨ����������Ҳ���Է�����ʽת��
*/

template<typename T> void func4(T) {}
void test4()
{
	int i = 0;
	func4<long>(i); // = func4(long). i converseto long
}

/*
	Ĭ�ϲ������ᱻ����ģ���Ƶ�������������ṩ�Ĳ�������û�в��������ܹ��Ƶ�������ģ���������ʧ��
*/
template<typename T> void func5(T = 0) {}
void test5()
{
	func5(1); // ok. T = int
	func5(1l); // ok. T = long
	//func5(); // cannot detect with default param
	func5<long>(); // ok. T = long. then use default param value 0 as long
}

/*
	��ΪSFNIAE��
	Substitution Failure Is Not An Error
	�Ƶ�ģ��ʧ�ܣ������ȥƥ������ģ���ɱ��������
	����в�����Ϊ��ǰģ����Ƶ�ʧ�ܶ������������
	����ʹ����û���κκ���ƥ�䣬�����Ĵ���Ҳ�����Ƶ�ʧ�ܴ��󣬶���δƥ�����
	��������ģ����Ƶ�ʧ�ܲ��Ǳ������
*/
struct A
{
	struct A_in {};
	float a;
	A_in ain;
};
template<typename T> void func6(T t, typename T::A_in) {}

void test6()
{
	A obja;
	func6(obja, obja.ain); // T = A
	//func6(1, 0); // T  = int�����ǲ�������int::A_in����ʱ�ı�������ģ���Ƶ�ʧ�ܣ�����û��ƥ��ĺ���
}

// func6�����⻰
namespace extra6
{
/*
	������һ���޷��ɹ��ĺ���ģ�壬��Ϊ����ͼʹ��T::A_inȥ�Ƶ�T�����������Ƶ����޷�ͨ���ڲ������Ƶ�
*/
template<typename T> void func6(typename T::A_in) {}
void test()
{
	A obja;
	//func6(obja.ain);
}

/*
	���ַ�ʽ��
		1.��ʽָ����ֱ��ָ��T������ ��Ҳ�Ͳ���Ҫ�Ƶ�
		2.�޸�����T::A_in => T����ģ��ֱ��ȥ�Ƶ�T�������������Ƿ����ڲ���
		3.dispatch/���أ�Ҳ���� �����func6(T t, typename T::A_in) ���Ƶ���T����ȥ��֤�Ƿ����T::A_in

	���ڵڶ��ַ�����Tֱ���Ƶ�ΪA::A_in����ʱ�޷�ֱ��ʹ��A��������type traits�ķ�ʽ��ȡ��A
*/
template<typename T>
struct extra_outer;

template<>
struct extra_outer<A::A_in> { using type = A; };// �ػ�A::A_in����ȡA

template<typename T> void func6(T) 
{
	using out_t = typename extra_outer<T>::type;//���ֱ���Ƶ���A::A_in������ extra_outer ��ȡ��A
}
}

/*
	����͵������ǻ�������û���������ͣ�������ģ���ģ�����Ͳ���ȴ������������
*/
template<typename T> void func7(T*) {}// ��ָͨ�룬����ΪT
template<typename T, typename C> void func7(T C::*) {}// C��Աָ�룬����ΪT
void test7()
{
	A obja;
	func7(&obja); // func7(A*) ����ָ�룬T = A
	func7(&obja.a); // func7(int*) �����Աָ�룬����ָͨ��һ�� T = int
	func7(&A::a); // func7(int A::*) ��int��Աָ�� T = int�� C = A
}

template<typename T>
class is_class
{
	template<typename C> static int test(...);//����1��������

	template<typename C> static char test(int C::*);//����2�������г�Ա

public:
	// sizeof ȡ���Ǻ����ķ���ֵ����������ú���
	// ��ʽָ����TΪģ������󣬾���ͨ�����ƥ�����֪����test���ĸ�����
	static constexpr bool value = 1 == sizeof(test<T>(nullptr));
};

void test_is_class()
{
	is_class<int>::value; // false
	is_class<A>::value; // true
}

/*
	������ģ���Ա�����⣬�������´�������
	����ע�⵽ Base  û��func2����
	�����ʹ��Derived<Base>::func2��������Ա���ͨ��������
	�������ʹ���ˣ�����ʱ�ᷢ������
	��Ҳ��SFINAEҪ���������
	���һ�����Ͳ�����������Ϊ ��ģ���ģ���������ô�� ʹ����Ϊ���󣬶���������ֱ������ŷ���
*/
struct Base
{
	void func() {}
};

template<typename T>
struct Derived :public T
{
	void func()
	{
		T::func();
	}

	void func2()
	{
		T::func2();
	}
};

void test8()
{
	Derived<Base> obj;
	obj.func();
	obj.func2();
}


/*
	����sort
	1.����г�Ա����sort����ʹ�ø�sort
	2.�����range����ʹ��std::sort
	3.�޷�sort
*/

struct have_sort { char c; }; // size == 1
struct have_range { char c; have_sort c1; }; // size == 2
struct have_nothing { char c; have_range c1; }; // size == 3

template<typename T, size_t>
struct sort_helper;

template<typename T>
struct sort_helper<T, sizeof(have_sort)>
{
	static void sort(T& t)
	{
		t.sort();
	}
};

template<typename T>
struct sort_helper<T, sizeof(have_range)>
{
	static void sort(T& t)
	{
		std::sort(t.begin(), t.end());
	}
};

template<typename T>
struct sort_helper<T, sizeof(have_nothing)>
{
	static void sort(T& t) {}
};

// ƥ���Աsort��Ϊ�˺�begin endͳһ����������������
template<typename T>
have_sort test_sort(decltype(&T::sort), decltype(&T::sort));

// ƥ��range
template<typename T>
have_range test_sort(decltype(&T::begin), decltype(&T::end));

// ƥ�����⣬���ȼ����
template<typename T>
have_nothing test_sort(...);

template<typename T>
void my_sort(T& t)
{
	sort_helper<T, sizeof(test_sort<T>(nullptr, nullptr))>::sort(t);
}