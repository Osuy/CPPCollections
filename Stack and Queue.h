#pragma once

#include <stdlib.h>
#define MAXSIZE 20

typedef struct
{
	int top;
	int col[MAXSIZE];
} StType;

typedef struct
{
	int data[MAXSIZE];//��Ԫ��
	int front, rear;//��ͷ�Ͷ�β
} SqQueue;

void InitQueue(SqQueue*& q)
{
	q = (SqQueue*)malloc(sizeof(SqQueue));
	q->front = q->rear = 0; //ʹ��Ϊ��
}

void DestroyQueue(SqQueue*& q)
{
	free(q);
}

bool QueueEmpty(SqQueue* q)
{
	return q->front == q->rear;
}

bool enQueue(SqQueue*& q, int e)
{
	if ((q->rear + 1) % MAXSIZE == q->front) //�����rear��һλ����front����Ϊ�ǻ��ζ��У�����MAXSIZE��ȡ��
		return false;
	q->rear = (q->rear + 1) % MAXSIZE;
	q->rear = e;
	return true;
}

bool deQueue(SqQueue*& q, int& e)
{
	if (q->front == q->rear)//�ӿ�
		return false;
	q->front = (q->front + 1) % MAXSIZE;//��ͷ�Ƕӵ�һ��Ԫ�ص�ǰһ�������front����1����ȡ��front����λ�õ�Ԫ�ظ�e
	e = q->data[q->front];
}


//ջ��Ӧ��

/*
	��׺���ʽת��׺���ʽ

	while(��exp��ȡ�ַ�ch��ch!='\0')
	{
		chΪ���֣�ѭ����ȡֱ�����������֣�д��postexp
		chΪ'('������optrջ
		chΪ')'��ѭ������ջoptr����д��postexp��ֱ��������һ��'('���ٽ�'('��ջ
		chΪ������������
			���optrջ�գ�����ջ��Ϊ'('������ch�����ȼ�����ջ����ֱ����ջ
			���򣬽�optr��ջ��д��postexp��ֱ��ch���ȼ�����ջ��������ջ��Ϊ'('����ch��ջ
	}
	//�˳�ѭ���󣬻���Ҫ���optrջ
	while(optrջ��Ϊ��)
	{
		��ջ��д��postexp
	}
*/

/*
	���ݷ�
	���飺
	���ڽ����Ԫ��������ݽṹ������++���������Ӷ����Կ���ȡ��һ����ѡ��ֵ��
	Ȼ���ʼֵӦ��Ϊ��һ��ȡֵ��ǰһ��ֵ����������ʼֵ++���Ϳ���ȡ��һ��ֵ

	�ȳ�ʼ��һ��Ԫ����ջ
	while(st����)
	{	
		ȡջ��el�����Լӣ����ջ����һ����ʼ����ֵ��������Ч�ģ��Լ�����ȡ��һ����Чֵ��
		while(el��Ч)el++

		if(el�Ϸ�)//el++ֱ�����磬�㲻�Ϸ�
		{
			if(�Ѿ��ҵ���)
			{
				����⣬������������н⣬return
			}
			else
			{
				����һ����Ԫ����ջ
			}

		}
		else
		{
			��ջ
		}
	}
	
	
*/