#include <iostream>
using namespace std;

/*
��̬3������������޷�������ģ�徲̬��Աstatic members of template classes
�Ͷ���__STL_STATIC_TEMPLATE_MEMBER_BUG

#if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ <8)
#	define  __STL_STATIC_TEMPLATE_MEMBER_BUG
*/

//�����������֧�־�̬��ģ���Ա���Ͳ��ܶ���_data
template <class T>
class testClass1 {
public:
	static T _data;
};

/*
��̬5��ƫ�ػ�
#ifdef _PARTAL_SPECIALIZATION_OF_CLASSTEMPLATES
#	define __STL_CLASS_PARTAL_SPECIAALIZATION
ƫ�ػ��������ò����б�Զ��δ�������еļ����ر�ָ��
ָ�����͵Ĳ�������������δ�����ͣ�Ҳ�����������ָ�루ָ��ƫ�ػ���
*/

//һ�㻯��ƣ�������δ������
template<class I,class O>
struct testClass {
	testClass() { cout << "<I,O>" << endl; }
};

//ƫ�ػ���ƣ�ֻ��һ��δ�����ͣ���һ��δ�����ͱ�ָ��Ϊint
template <class T>
struct testClass<T,int>{
	testClass() {
		cout << "<T��int>" << endl;
	}
};
//��Ȼ��ָ��ΪTҲ�ǿ��Ե�
template <class T>
struct testClass<T,T> {
	testClass() {
		cout << "<T��T>" << endl;
	}
};

//ָ��ƫ�ػ�:ָ��Ϊ�����ָ�룬��˾���Ҫдȫδ������
template<class I, class O>
struct testClass<I*,O*> {
	testClass() { cout << "<I*,O*>" << endl; }
};

//constƫ�ػ���ָ��Ϊ����������
template<class I, class O>
struct testClass<I const, O const> {
	testClass() { cout << "<I const,O const>" << endl; }
};



//ȫ�ػ���ƣ���ȫ����δ�����Ͷ�ָ����ģ�庯������ƫ�ػ���ֻ��ȫ�ػ�
template <>
struct testClass<int��char> {
	testClass() {
		cout << "<int,char>" << endl;
	}
};

/*

*/