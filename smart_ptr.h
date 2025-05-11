#pragma once

/*
	����ָ������Ҫ��sharedptr��weakptr
		�������ǻ������ü����ĵ�����ָ�롣���ü������Ǽ�¼�ж��ٸ�����ָ��ָ�����������Դ
		����ʱ���ü�����ʼ��Ϊ1������ʱ�����ü�����1�������Ϊ0�����ͷ���Դ��
		������Ҫ�Ŀ�������͸�ֵ������
		��������ʱ������ǳ�������������ü�����1
		��ֵʱ�����ж��Ƿ����Ҹ�ֵ��Ȼ���Լ������ü�����1������0���ͷţ�Ȼ�����ǳ�������ٰѶԷ������ü�����1���Լ���ֵ�Է������ü���

	����ʹ��sharedptrʱ�����ܻ����ѭ�����õ����⡣A����B��B����A���ͷ�Aʱ��Ҫ���ͷ�B���ͷ�BʱҪ���ͷ�A����ɱ���
	��ˣ���Ҫһ������������ָ�롣weakptr��������Դʱ������������ü�����һ����Դ���ͷ�ʱ���������ñ�һ��weakptr���ö�����ֹ
	
	���ڳ��й�ϵ��ʹ��sharedptr
	���ڽ��ù�ϵ��ʹ��weakptr
*/
#include <memory>
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;
//����ָ���ʹ��
void use_unique_ptr()
{
	//ʹ��ԭ��ָ���ʼ��
	unique_ptr<int> a(new int(3));

	//ʹ��
	unique_ptr<int> a(new int(3));
	
}

void use_shared_ptr()
{
	shared_ptr<int> a(new int(3));
}

void use_weak_ptr()
{
	//weak_ptr��ʹ��������shared_ptr
	//�޷�ͨ��ԭ��ָ���ʼ��
	//weak_ptr<int> _a(new int(3));

	//ͨ��shared_ptr��ʼ��
	shared_ptr<int> a = make_shared<int>(new int(3));
	weak_ptr<int> b(a);

	//ͨ��weak_ptr��ʼ��
	weak_ptr<int> c(b);

	//ͨ��weak_ptr����ֵ��ʼ��
	weak_ptr<int> d(weak_ptr<int>());
}
