#pragma once
#include <vcruntime_exception.h>
#include <new.h>
/*
new
	һ������õ���ȷ��ָ�룬����ʱ�����new-handler��û��new-handlerʱ�׳�����
		�����������ṩһ���������ʲô�µ�new-handler����Ȼ��ֻ�ܼ�ϣ���������������ó��ڴ棩
		�����û���õ�new-handler������ͨ������set_new_handlerȡ���ٷŻصķ�ʽ�õ������ٶ��̻߳����±��뿼�Ǽ���

	�������ж�sizeΪ0��������������֡�Ĭ��new��������Ϊ0ʱ����1��С���ڴ�

	�������ڷ�ϣ����size��һ���������֡������ר��Ϊһ���ඨ����operator new�������ִ������ಢ������û����дnew
		��ô�����̳и����new������Base::operator new��ר��Ϊsizeof(Base)׼���ģ�����Derived�����ܱ��ֲ���
		�������ô����
			if(size != sizeof(Base)) return ::operator new(size);
		���赣��sizeΪ0���������ΪBase������1�Ĵ�С������Ϊ0ʱ�ͻ���::new����

	�ġ�����member operator new[]���㲻�ܼٶ��ж���Ԫ�أ���Ϊ���������ڴ�������������飬��ô�䲻��ͨ��size / sizeof(Base)����ȡ����

delete
	һ��ȷ���ͷŶ�������ȷ�ġ���
		if(!p)return;

	��������member operator delete�����size����ȷ�򽻸�::delete���ܹ���һ�����
		if(size != sizeof(Base))
		{
			::operator delete(p);
			return;
		}
		���ǰ�����㽫Derived��������������Ϊvirtual�����򴫸�delete��size��Ȼ��Base�Ĵ�С

		��Ϊ�����deleteһ��ָ���������Ļ���ָ�룬��Base�Ĵ�С����delete�����Ƿ����ڴ�й©
		�����deleteһ��ָ��������������ָ�룬��ôsize��������Ĵ�С��������ΪBase::delete������̳���
		��Ҳ��Ϊɶһ��Base::delete�����size��һ����Base�Ĵ�С��Ե��
		����delete�����յ�һ������Ĵ�С������Ӧ�ý���ȫ��deleteȥ����

*/

namespace case51
{
void* operator new(size_t size)throw (std::bad_alloc)
{
	if (0 == size) size = 1;

	// ������ѭ����Ϊ���ܲ��ϵ��� new_handler ����ͼ�ͷ��ڴ棬�Թ������ڴ�
	while (true)
	{
		if (1/*����ɹ�*/) return 0/*����õ����ڴ�*/;

		// ���� set_new_handler ����ȡ��ǰ�� new_handler
		new_handler globalHandler = set_new_handler(0);
		set_new_handler(globalHandler);

		// ����� new_handler ���ã������´�ѭ��
		// ���û�У��׳��쳣
		if (globalHandler) (*globalHandler)();
		else throw std::bad_alloc();
	}
}

class Base
{
public:
	static void* operator new(size_t size) throw (std::bad_alloc)
	{
		if (size != sizeof(Base))// sizeof(Base) ������0
		{
			// �����operator new ��������newһ��Base��С���ڴ棬�򽻸�ȫ��newȥ��
			// ��Ϊһ�����new��Ӧ�ñ���Ϊ��Ϊ�����Լ�ר��ʹ�õģ�ֻ���Լ���size�����ñ���
			// ����size�����ֲ��ѣ�����Ӧ����ȫ��new
			return ::operator new(size);
		}

		return ::operator new(size);
	}

	static void operator delete(void* memory, size_t size) throw()
	{
		if (0 == memory)return;
		if (size != sizeof(Base))
		{
			::operator delete(memory, size);
			return;
		}

		return;
	}
};
}