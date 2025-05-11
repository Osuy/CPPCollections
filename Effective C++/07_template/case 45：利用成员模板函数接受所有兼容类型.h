#pragma once
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

	template<class T>
	class shared_ptr
	{
	explicit shared_ptr(T* raw) : _handle{ raw } {} // ԭ��ָ���ʼ��ʱ��ͨ��������ʽ

	template<class U>
	shared_ptr(const shared_ptr<U>& other) //������copy��������explicit����Ϊ����ת������ʽ��
		: _handle{ other.get() } // ����ԭ��ָ���ת�ͣ��������ת�ͣ����ڱ�������������ǰ�ȫ��
	{ ... } // �������ü���

	private:
		T* _handle;
	}

	// std::shared_ptr��������������
	template<class U>
	explicit shared_ptr(U* other);
	template<class U>
	explicit shared_ptr(const weak_ptr<U>& other);
	template<class U>
	explicit shared_ptr(auto_ptr<U>& other);// auto_ptr ������const�������п���Ȩ����ȡָ�����޸�����ָ��Ϊ��
	template<class U>
	shared_ptr(const shared_ptr<U>& other);

	template<class U>
	shared_ptr(const shared_ptr<U>& other);
	template<class U>
	shared_ptr<T>& operator=(const shared_ptr<U>& other); // ������=������

	ע��ֻ������shared_ptr��ת���Ƿ�explicit�ģ�����ֻ�����Լ���ʽת��
	��Ҫ��������ָ�����ʽת���������ڴ���ʵ�֡���ȫ�ԡ��ɶ��Զ��������
*/