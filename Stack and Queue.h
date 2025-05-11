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
	int data[MAXSIZE];//队元素
	int front, rear;//队头和队尾
} SqQueue;

void InitQueue(SqQueue*& q)
{
	q = (SqQueue*)malloc(sizeof(SqQueue));
	q->front = q->rear = 0; //使队为空
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
	if ((q->rear + 1) % MAXSIZE == q->front) //溢出：rear后一位就是front。因为是环形队列，超出MAXSIZE用取余
		return false;
	q->rear = (q->rear + 1) % MAXSIZE;
	q->rear = e;
	return true;
}

bool deQueue(SqQueue*& q, int& e)
{
	if (q->front == q->rear)//队空
		return false;
	q->front = (q->front + 1) % MAXSIZE;//队头是队第一个元素的前一个，因此front先增1，再取出front所在位置的元素给e
	e = q->data[q->front];
}


//栈的应用

/*
	中缀表达式转后缀表达式

	while(从exp读取字符ch，ch!='\0')
	{
		ch为数字：循环读取直到不再是数字，写入postexp
		ch为'('：加入optr栈
		ch为')'：循环：出栈optr，并写入postexp，直到遇到第一个'('。再将'('出栈
		ch为其他操作符：
			如果optr栈空，或者栈顶为'('，或者ch的优先级高于栈顶，直接入栈
			否则，将optr出栈，写入postexp，直到ch优先级高于栈顶，或者栈顶为'('，将ch入栈
	}
	//退出循环后，还需要清空optr栈
	while(optr栈不为空)
	{
		出栈并写入postexp
	}
*/

/*
	回溯法
	建议：
	对于解的子元素项的数据结构，重载++操作符，从而可以快速取下一个候选的值，
	然后初始值应设为第一个取值的前一个值，这样，初始值++，就可以取第一个值

	先初始化一个元素入栈
	while(st不空)
	{	
		取栈顶el，并自加（如果栈顶是一个初始化的值，则还是无效的，自加让其取第一个有效值）
		while(el无效)el++

		if(el合法)//el++直到出界，便不合法
		{
			if(已经找到解)
			{
				输出解，如果不用找所有解，return
			}
			else
			{
				创建一个新元素入栈
			}

		}
		else
		{
			出栈
		}
	}
	
	
*/