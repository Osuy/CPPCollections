#pragma once
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
*/