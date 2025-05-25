#pragma once

namespace case45
{
/*
	���ó�Ա����ģ��������м�������

	һ����ĳ�Ա����ͨ�����ؿ��Խ��ܲ�ͬ���ͣ����в�ͬ�Ĵ���

	����Щ���д������и߶ȹ���ʱ������������͵��������࣬���ػ��ǿ���ʤ��
	������ϣ���ܼ��ݾ����ܶ�����ͣ���ôÿ������һ�ּ��ݣ�����Ҫ����һ��
	���õ�������һ����ģ���������������ͣ���ģ�������ʡȥ���صĹ�����һ������
	ǰ���Ƕ���ÿ�ּ������Ͷ����й��ԣ������Ҫ���⴦����Ȼ��Ҫ����


	������shared_ptr��˵��Ϊ���ܹ����õ�ģ��ԭ��ָ�룬��Ҫ����ת�͵Ĺ���
	Ȼ����shared_ptr<Base>��shared_ptr<Derived>����Ϊ���������ͣ�֮��û���κ���ϵ
	Ϊ��ģ��ָ�������ת�ͣ���Ҫ�õ�����ģ����ܼ������ͣ�����������copy����
*/

template<class T>
class smart_ptr
{
public:
	explicit smart_ptr(T* raw) : _handle{ raw } {} // ԭ��ָ���ʼ��ʱ��ͨ��������ʽ

	// ע�⣺�����Ŀ��������ϸ������ϲ��ǿ������죬��Ϊ��ͬ����ģ��ʵ���ǲ�ͬ���࣬�൱����һ���޹ز���������
	// ��˿�������ͷ�����������û�й���������Ӱ��
	smart_ptr(const smart_ptr& other); // �������죬�ͷ����Ŀ����������ͬʱ���ڣ��������������Ҫ�õ�����������Ȼ���ṩ

	template<class U>
	smart_ptr(const shared_ptr<U>& other) //������copy���죬��explicit����Ϊ����ת������ʽ��
		: _handle{ other.get() } // ����ԭ��ָ���ת�ͣ��������ת�ͣ����ڱ�������������ǰ�ȫ��
	{
		...
	} // �������ü���

  // smart_ptr��������������
	template<class U>
	explicit smart_ptr(U* other);
	template<class U>
	explicit smart_ptr(const weak_ptr<U>& other);
	template<class U>
	explicit smart_ptr(auto_ptr<U>& other);// auto_ptr ������const�������п���Ȩ����ȡָ�����޸�����ָ��Ϊ��
	template<class U>
	smart_ptr(const smart_ptr<U>& other);

	template<class U>
	smart_ptr(const smart_ptr<U>& other);
	template<class U>
	smart_ptr<T>& operator=(const smart_ptr<U>& other); // ������=������

private:
	T* _handle;
};

/*
	ע��ֻ������shared_ptr��ת���Ƿ�explicit�ģ�����ֻ�����Լ���ʽת��
	��Ҫ��������ָ�����ʽת���������ڴ���ʵ�֡���ȫ�ԡ��ɶ��Զ��������
*/
}