#pragma once

/*
	remove_reference��������ƫ�ػ���traits������ʵ�ֵ�
	�����ǳ��桢���á���ֵ���ã�ͨ��remove_reference<ClassXXX>::Type���ܻ�ȡ��ԭ������������
*/
template <class T>
struct remove_reference
{
	typedef T Type;
};

//��ֵ����ƫ�ػ�
template <class T>
struct remove_reference<T&>
{
	typedef T Type;
};

//��ֵ����ƫ�ػ�
template <class T>
struct remove_reference<T&&>
{
	typedef T Type;
};