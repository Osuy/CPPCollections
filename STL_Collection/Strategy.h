#pragma once


/*
	����ģʽ
	ͨ������������enum+ifelse/switchʵ�ֶ��֧����䣺
		����һ��enum{a,b,c,d}
		��һ���������з�֧�жϻ�switch��
			if(var==a){...}                          switch(var){
			else if(var==b){...}                         case a:...break;
			else if(var==c){...}                         case b:...break;
			else if(var==d){...}                         case c:...break;
								                         case d:...break;
														 default:break;
													 }

		����һ��Ч��ʮ�ֵ������Ը��õĴ��롣
		һ����Ҫ��չ���ͱ�����enum�ͷ�֧ͬʱ�޸ġ�

	����ģʽ���ڶ�̬ʵ��
	Ϊ���з�֧���һ�����࣬����һ�����麯��
	class Father{
	public:
		virtual void func()=0;
	};
	Ȼ��ÿ����֧�̳���Father������дfunc��
	class A{
	public:
		virtual void func(){...}
	}
	class B{...}
	class C{...}

	ͨ������ָ��Ͷ�̬��ʵ�ַ�֧ѡ��
		Father* obj;//obj���Խ���A��B��C��D�����
		obj->func();//���ݶ�������͵��ö�Ӧ��ʵ��
*/