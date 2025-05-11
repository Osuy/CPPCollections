#pragma once
#include <vector>
#include <unordered_map>
#include <stdlib.h>
using namespace std;

/*
	��������

	��
		�ڵ������������Ϊ�ýڵ�Ķȡ��������нڵ�Ķȵ����ֵ��Ϊ���Ķ�

	Ҷ�ӽڵ�
		��Ϊ������ڵ��ΪҶ�ӽڵ�

	·����·������
		������һ�����ڵ�i��j����������i��jΪͷβ��һϵ�нڵ㣬ʹ����ÿһ���ڵ㶼����һ���ڵ��ǰ���ڵ㣬
		���i��j֮�����һ��·�����������У���·���̶Ⱦ��Ǹ����нڵ���-1��

	���ӽڵ㡢���ڵ㡢�ֵܽڵ�
		�ڵ�����к�̽ڵ�Ϊ���ӣ�ǰ���ڵ�Ϊ���ڵ㣬���ڵ��������̽ڵ�Ϊ�ֵ�

	��κ�����
		�ڵ�Ĳ���ǴӸ��ڵ�Ϊ��㵽�ýڵ��·�����ȡ�
		���ĸ߶ȣ�Ҳ����ȣ��Ǹ��ڵ㵽����Ҷ�ӽڵ���·�������ֵ

	��������������
		������ÿ���ڵ㣬�亢�ӽڵ��˳������������������Ϊ������
		�����ӽڵ�Ĵ��������壬���Ը���������Ϊ������
		һ�����۵�������������

	ɭ��
		�������ϵ����ļ���
*/

/*
	��������
	
	���Ľڵ��� = �������нڵ�Ķ�֮�� + 1

	m�����ĵ�i��������m��i-1�θ��ڵ�

	�߶�Ϊh��m����������(m^h - 1)/(m-1)���ڵ�  ���㷽������ȱ�����

	����n���ڵ��m��������С�߶�Ϊlogm(n(m-1)+1)������ȡ����  ���㷽��������ʽ�ķ�����


*/

/*
	������
		���нڵ�Ķ�����Ϊ2����

	��������
		����ײ�Ľڵ��⣬����ڵ㶼���������ӡ��Ӿ�����������������

	��ȫ������
		�����������㣬����ڵ㶼���������ӣ�����ײ�����нڵ㶼����������ߡ��Ӿ����������ε����½�ȱ

	�Ƕ������ڵ�n0��n1��n2���ֱ��ʾ��Ϊ0����Ϊ1����Ϊ2�Ľڵ㣬��
		�ڵ��� = n0 + n1 + n2
		���ܺ� = n1 + 2 * n2
		���ڸ��ڵ�û����ߣ���ˣ�
		�ڵ��� - 1 = ���ܺ�
		�����У�
			n0 = n2 + 1
		�� Ҷ�ӽڵ����� = ˫���ӽڵ����� + 1
	
*/

/*
	�������Ĵ洢�ṹ

	˳��洢
		���������ڵ��Ԫ�أ����������ʽ���棬�����±����
		�����±�Ϊi�Ľڵ㣬�����ӵ��±�Ϊ2*i+1���Һ���Ϊ2*i+2
		���ڵ��±�Ϊ0
		���2*i+1���������鳤�ȣ���iΪҶ�ӽڵ�

	��ʽ�洢��Ҳ�ƶ�����
		���Ľڵ�ṹ�������Ԫ�ء�����ָ�롢�Һ���ָ�루�����ڵ�ָ�룩
*/

/*
	�������ı���

	����������ȷ�����������������������������������

	���������������������������������������������

	�����������������������������������������������

	��α��������������Ľڵ�
*/

struct BTNode
{
	int data;
	char cdata;
	BTNode* lchild;
	BTNode* rchild;
};

//�����ĵݹ��㷨
//���ֱ����Ķ��屾����ǵݹ�ģ���˵ݹ�ı����ܺ�д
//ֻ��Ҫ�ڵݹ麯����ı���ʵ�λ�ü���
void PreOrder(BTNode* p)
{
	if (p)
	{
		//����p
		PreOrder(p->lchild);
		PreOrder(p->rchild);
	}
}

void InOrder(BTNode* p)
{
	if (p)
	{
		InOrder(p->lchild);
		//����p
		InOrder(p->rchild);
	}
}

void PostOrder(BTNode* p)
{
	if (p)
	{
		PostOrder(p->lchild);
		PostOrder(p->rchild);
		//����p
	}
}

//��α�����Ҫ���ö���
void LevelOrder(BTNode* p)
{
	BTNode* queue[20];
	BTNode* q;
	int front = -1, rear = 0;
	queue[rear] = p;
	while (front < rear)//���Ӳ���
	{
		//���Ӳ�����
		q = queue[++front];

		//��q�ĺ������
		queue[++rear] = q->lchild;
		queue[++rear] = q->rchild;
	}
}

//�����ķǵݹ��㷨
//�ǵݹ�������㷨����Ҫ����ջ�������û��ݷ�

//�������
/*
	˼·���ȷ��������ٷ������Һ��ӣ�����������Ҫ�õ���
		 ��Ϊ�ȷ������ӣ�Ȼ������������������������Ҫȥ���������������
		 �������Ƚ����
		 ����ʵ�ַ��������֣�
			1.	�Ƚ����ڵ���ջ��
				ѭ���壺��ջ��Ԫ��p���Ƚ��Һ�����ջ���ٽ�������ջ
			2.	����һ���㣬ѭ����������������ջ��ѭ�������󣬷��ʳ�ջԪ�ص��Һ��ӣ�ֱ��ջ��
*/
void PreOrder2(BTNode* p)
{
	if (!p)return;

	BTNode* st[20];
	BTNode* q;
	int top = -1;
	st[++top] = p;

	while (top >= 0)
	{
		q = st[top--];
		//���q
		//�Ƚ��Һ��ӣ��ٽ�����
		if (q->rchild)
			st[++top] = q->rchild;
		if (q->lchild)
			st[++top] = q->lchild;
	}
}

//��д
void PreOrder3(BTNode* p)
{
	if (!p)return;
	BTNode* st[20];
	int top = -1;
	st[++top] = p;

	while (top >= 0)
	{
		p = st[top--];
		//���p

		if (p->rchild)
			st[++top] = p->rchild;
		if (p->lchild)
			st[++top] = p->lchild;
	}
}

//˼·�����ǲ��Ϸ������ӣ����û�����ӣ������Һ���
//�汾1
void InOrder2(BTNode* p)
{
	BTNode* st[20];
	int top = -1;

	while (top >= 0 || p)//
	{
		//��ÿһ������������ǰ���ȱ������ĸ��ڵ�
		while (p)
		{
			//����p
			st[++top] = p;
			p = p->lchild;
		}

		if (top >= 0)
		{
			p = st[top--];
			//����p
			p = p->rchild;
		}
	}
}

//�汾2
void InOrder2(BTNode* p)
{
	BTNode* st[20];
	int top = -1;

	while (p != nullptr || top >= 0)
	{
		if (p != nullptr)
		{
			st[++top] = p;
			p = p->lchild;
		}
		else
		{
			p = st[top--];
			p = p->rchild;
		}
	}
}


/*
	�������

	�Ȱ������������������������������ٳ�ջ����������
*/

void PostOrder2(BTNode* p)
{
	BTNode * q, * r;
	BTNode* st[20];
	int top = -1;
	q = p;

	do
	{
		while (q)
		{
			st[++top] = q;
			q = q->lchild;
		}
		//����ýڵ���������������q�����ŷ���q��������

		r = nullptr;
		while (top >= 0)
		{
			q = st[top];//qȡջ��
			if (q->rchild == r)//���qû���Һ��ӣ������Һ��ӱ����ʹ�
			{
				//����q
				//��ջ������r
				r = st[top--];//��ջ������r����һ��ѭ����q = st[top];q��ȡ����r�ĸ��ס���ʱq�������ջ��
			}
			else
			{
				p = r->rchild;
				break;//������������ָ������Ȼ���˳���ѭ��������һ����ѭ����q����������ջ
			}
		}
	} while (top >= 0);
}

void PostOrder3(BTNode* p)
{
	BTNode* st[20];
	BTNode* q, * r;
	int top = -1;
	st[++top] = p;
	q = p->lchild;

	while (top >= 0)
	{
		while (q)
		{
			st[++top] = q;
			q = q->lchild;
		}

		r = nullptr;
		while (top >= 0)
		{
			q = st[top];
			if (q->rchild == r)
			{
				r = st[top--];
				//cout<<
			}
			else
			{
				q = q->rchild;
			}
		}
	}
}

/*
	�����������򡢺����е������������й��������
*/


/*	
	���й���
	�������еĵ�һ��Ԫ�ؾ���������Ȼ���ں����������ҵ��������±�Ϊi
	��ô��������Ԫ��Ϊi��������Ϊ���г���-1-i
	Ȼ��ݹ飬�����Ӧ��Ԫ�ؼ���
*/
BTNode* PreInCreate(char* pre, char* in, int n)
{
	if (n <= 0)return nullptr;

	BTNode* b = nullptr;

	int i = 0;
	while (i++ < n)
		if (in[i] == *pre)break;

	if (i < n)
	{
		b = (BTNode*)malloc(sizeof(BTNode));
		b->data = *pre;
		b->lchild = PreInCreate(pre + 1, in, i);
		b->rchild = PreInCreate(pre + i + 1, in + i + 1, n - i - 1);
	}
	return b;

}

/*
	���й���
*/

BTNode* InPostCreate(char* in, char* post, int n)
{
	if (n <= 0)return nullptr;

	BTNode* b = nullptr;

	int i = n - 1;
	while (i >= 0)
		if (post[n - 1] == in[i])break;

	if (i >= 0)
	{
		b = (BTNode*)malloc(sizeof(BTNode));
		b->data = post[n - 1];
		b->lchild = InPostCreate(in, post, i);
		b->rchild = InPostCreate(in + i + 1, post + i, n - i - 1);
	}

	return b;
}


/*
	���������㷨
*/












