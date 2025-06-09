#pragma once
#include <iostream>
namespace _crtp
{
/*
	CRTP��Curiously Recurring Template Pattern ����ݹ�ģ��ģʽ����� Do it for me

	��һ����ģ����Ϊ���࣬���Ұ���������Ϊģ�����
	�����κ�ϣ������������ĺ���֮������ static_cast ��thisתΪ������ȥ����
*/
template<typename Derived>
struct Base
{
	void base_func()
	{
		static_cast<Derived*>(this)->derived_func();
	}
};

struct Derived : public Base<Derived>
{
	void derived_func()
	{
		std::cout << "Derived::derived_func executed" << std::endl;
	}
};

/*
	Ϊʲô��������
	CRTP�Ļ���Base��Ȼʹ������Derived��Ϊģ������������ڲ�����ת�ͣ�����ʹ��Derived
	Derived��Base�������൱����һ��ǰ����������
	һ��������δ�����class��������������ָ�룬������������Ա
	ת��ʱҲ��δ��ҪDerived����������

	������Derived�ĺ���derived_func��������ʹ����
	��ģ������г�Ա��������Ϊ����ģ�壬����ģ��ֻ�ڴ��ڱ����õ�����Ż�ȥ����ʵ��
	��������ģ��ʵ���������У������ڳ�Ա������ʹ����Derived��Ҳ����ʵ���ϵ�ʹ��
	���������base_func����Ȼ����һ��Derived���󣬶�һ��Derived������� �ܹ����ڣ���Ȼ�Ѿ�������Derived�Ķ���
	����base_func��ʵ����ʱҲ����֪��derived_func�Ķ���
	
	static_cast ��thisתΪ�����లȫ��
	CRTP��Ҫ����ǰ���������Ϊ�����ģ�������������Զ������ʹ�û��࣬�����ʵ�������У�this��Ȼ��һ��Derived����
	���ǽ�thisתΪDerivedָ�룬ʵ�������ǰ�ȫ�ġ����ǲ���������Լ��
*/

/*
	CRTP������
	CRTP��֧��Ƕ����������
	base_func�ܵ���derived_func���������������Ǻ���ģ�壬��ʹ�ò�ʵ����
	��������������û��ʹ�ã�ҲҪ�Ƶ�����ʱDerived��Base�ǲ������ģ���ΪDerived�Ķ���������Base<Derived>�Ķ���
	��Base<Derived>::Tȴ������Derived::T
*/

template<typename Derived> struct Base2 
{ 
	// using T = typename Derived::T;  // ����Ƕ��T������ʱʧ��
};

struct Derived2 : public Base2<Derived2> { using T = int; };

/*
	����1��Ϊ����֧��==�������Զ�����!=
	ͨ����һ�����!=��������ʵ�ֶ��Ƕ� ==�����������ȡ��������ݴ��뼸������ȫһ���ģ���˿���ֱ�ӽ���baseʵ��
*/
namespace example1
{
	template<typename Derived>
	struct Base
	{
		bool operator!=(const Derived& other)const
		{
			return static_cast<const Derived*>(this)->operator ==(other);
		}
	};

	struct Derived : public Base<Derived>
	{
		bool operator==(const Derived& other)const
		{
			return false;// implementation
		}
	};
}

/*
	C++20��concepts��C++23����ʽthis�βΣ�����ȡ��CRTP�Ĵ󲿷ֹ��ܣ����ҿɶ��Ը���
*/
namespace example2
{
	// ʹ����ʾthis�β�ȡ��crtp
	struct Base
	{
		template<typename T>
		void base_func(this T&& self)  
		{
			self.derived_func();
		}
	};

	struct Derived : public Base
	{
		void derived_func()
		{
			std::cout << "Derived::derived_func executed" << std::endl;
		}
	};
}
}


