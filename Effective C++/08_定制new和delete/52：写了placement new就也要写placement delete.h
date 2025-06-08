#pragma once

namespace case52
{
/*
	�κ�ӵ�ж�������� new/delete ������ placement new/delete
	����ͨ�� placement new/delete ָ���ǵڶ�����Ϊ void* �İ汾

	placement new �����ɳ���Ա����ʹ�ã����Ƕ�Ӧ�汾�� placement delete ͨ�������ڴ����쳣��
	��Ϊ����һ��ָ�룬����Ա�ͱ�������ʵ���޷�֪����������ͨnew����ģ�����placement new�����
	��˸���̸���ϵ��ö�Ӧ�汾�� placement delete�����ǣ�
		1.�ֶ����ã�ǰ���ǳ���Ա֪�������Բ��ɿ�
		2.��ʹ��new�ĵط������캯���׳��쳣�����̵���delete����ʱ����֪���������ĸ�new������Ҳ�ܾ��������ĸ�delete

	����֮�� placement delete ���Զ����õģ��ҽ��ڹ��캯���쳣ʱ������

	placement new �� new �����أ����һ��class������placement new ��û�ж������ new��
	��placement new�Ḳ�� ::new �������classֻ��ʹ��placement������
*/

// ::new �ĵ��͸�ʽ
void* operator new(size_t size);

// ::delete �ĵ��͸�ʽ
void operator delete(void* data);

// placement new �ĵ��͸�ʽ
void* operator new(size_t size, void* data);

class Base
{
public:
	// ��� new ���͸�ʽ
	static void* operator new(size_t size);
	
	// ��� placement new �����������ĵ���new�������ֻ����placement new���죬��Ϊ::new�ѱ�placement new ����
	static void* operator new(size_t size, void* data); 

	// ��� ::delete �еڶ�����size
	static void operator delete(void* data, size_t size);
};

class Derived :public Base
{
public:
	// ������Base������new����case33��
	static void* operator new(size_t size);
};

}