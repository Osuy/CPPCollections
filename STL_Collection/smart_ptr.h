#pragma once
#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;

/*
	����ָ���ʹ��
*/

void use_unique_ptr()
{
	unique_ptr<int> a(new int(3));
	cout << *(a.get()) << endl;

	a.reset(new int(4));
	cout << *(a.get()) << endl;
	
	a.reset();
	if (a.get())
		cout << *(a.get()) << endl;
	else
		cout << "a hold nothing" << endl;
	
	unique_ptr<int> b(new int(2));
	cout << *(b.get()) << endl;
	
	a.swap(b);
	cout << *(a.get()) << endl;
	if (b.get())
		cout << *(b.get()) << endl;
	else
		cout << "b hold nothing" << endl;
	
	a.release();
	if (a.get())
		cout << *(a.get()) << endl;
	else
		cout << "a hold nothing" << endl;


	//unique_ptr<int> b = a;
}

void use_shared_ptr()
{
	shared_ptr<int> a;
	cout << a.use_count() << endl;
	if (a.get())
		cout << *(a.get()) << endl;
	else
		cout << "a hold nothing" << endl;

	shared_ptr<int> b(new int(3));
	cout << b.use_count() << endl;
	a.reset(b.get());
	cout << a.use_count() << endl;
	cout << b.use_count() << endl;
}

void use_weak_ptr()
{}

/*
	RAII˼�룺��Դ��ȡʱ�ͽ��г�ʼ��
	C++�У�����Աͨ��new��delete�ڶ��Ϸ�����ͷ���Դ������������̲����Զ��ģ������������ڴ�й©��
	����ָ����һ������ָ����Ϊ�Ķ���
*/

/*
	auto_ptr
	ֻ�������ĳ��С��ͷŵȹ���
	�ṩ��һ�㹹�죬�������죬=���أ���������
	
	���⣺
		auto_ptr��������Ƕ�ռָ��ģ���������һ��ָ������auto_ptr��ʼ��
		�������Ȩ�ĸ���ģ����

		����ת������Ȩ�����ǲ���ʾ��ԭauto_ptr�ڽ�������Ȩ���������ʹ�ý������
		
*/
template<class _Ty>
class auto_ptr
{
public:
	typedef _Ty element_type;

	//ͨ��ԭ��ָ���ʼ��
	//��ʽ���ι��죬��ֹ��ʽ����ת���ĳ�ʼ��
	explicit auto_ptr(_Ty* _Ptr = 0) noexcept
		: _Myptr(_Ptr)
	{
	}

	//ͨ����һ��auto_ptrת������Ȩ��ʼ��
	auto_ptr(auto_ptr& _Right) noexcept
		: _Myptr(_Right.release())
	{
	}

	// ͨ���������͵�auto_ptr����ת������ʼ��
	// ����ģ��û�в�����������ʽָ��ģ�����
	// ����������auto_ptr
	// ��Ϊ�͹��캯��ʮ�����ƣ����ڳ��е�ָ�������ת��
	template <class _Other>
	operator auto_ptr<_Other>() noexcept { // convert to compatible auto_ptr
		return auto_ptr<_Other>(*this);
	}

	//��������
	template <class _Other>
	auto_ptr(auto_ptr<_Other>& _Right) noexcept : _Myptr(_Right.release()) {}

	//=����
	auto_ptr& operator=(auto_ptr& _Right) noexcept {
		reset(_Right.release());
		return *this;
	}

	//return���е�ָ�룬_Myptrָ���
	_Ty* release() noexcept {
		_Ty* _Tmp = _Myptr;
		_Myptr = nullptr;
		return _Tmp;
	}

	//�����������ͷų��е�ָ����ָ����ڴ�
	~auto_ptr() noexcept {
		delete _Myptr;
	}

private:
	element_type _Myptr;
};


/*
	unique_ptr
	��ռָ�룬һ��ָ��ͬһʱ��ֻ�ܱ�һ��unique_ptr����
	�����auto_ptr������Ȩ����

	auto_ptr������release֧��ת������Ȩ��Ϊʲô˵���ܵ������У�
	auto_ptr������Ȩת�������ڳ���Ա�ֶ�������֮������Ա�����˼���ܣ�
	�ǿ����ֶ���auto_ptr��ռһ��ָ��ġ�����unique_ptr�����Զ���ɶ�ռ

	����ʵ�־�����auto_ptr�Ļ����ϣ������˿��������=���أ������ƶ�����

	auto_ptr����ʹ��ָ���ʼ��������unique_ptrֻ��ʹ����ֵ��ʼ��
	��һ����ֵ�ǲ�����ͬʱ��������������еġ�Ҳ����unique_ptr�����������ֵ��
	����unique_ptrֻ��ͨ��������=�Ż������Ȩ������ͬһʱ��ֻ��һ��unique_ptr���������ֵ
*/


/*
	shared_ptr

*/