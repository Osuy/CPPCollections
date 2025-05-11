#pragma once
#include <vector>
#include <unordered_map>
#include <stdlib.h>
using namespace std;

/*
	树的术语

	度
		节点的子树个数称为该节点的度。树中所有节点的度的最大值称为树的度

	叶子节点
		度为零的树节点称为叶子节点

	路径与路径长度
		树中任一两个节点i和j，若存在以i和j为头尾的一系列节点，使其中每一个节点都是下一个节点的前驱节点，
		则称i与j之间存在一条路径，即该序列，而路径程度就是该序列节点数-1；

	孩子节点、父节点、兄弟节点
		节点的所有后继节点为孩子，前驱节点为父节点，父节点的其他后继节点为兄弟

	层次和树高
		节点的层次是从根节点为起点到该节点的路径长度。
		树的高度，也称深度，是根节点到所有叶子节点中路径最长的数值

	有序树和无序树
		对树中每个节点，其孩子节点的顺序不能随意更换，则该树为有序树
		而孩子节点的次序无意义，可以更换的树称为无序树
		一般讨论的树都是有序树

	森林
		两颗以上的树的集合
*/

/*
	树的性质
	
	树的节点数 = 树的所有节点的度之和 + 1

	m次树的第i层至多有m的i-1次个节点

	高度为h的m次树至多有(m^h - 1)/(m-1)个节点  计算方法是求等比数列

	具有n个节点的m次数的最小高度为logm(n(m-1)+1)（向下取整）  计算方法是求上式的反函数


*/

/*
	二叉树
		所有节点的度至多为2的树

	满二叉树
		除最底层的节点外，其余节点都有两个孩子。视觉上是完整的三角形

	完全二叉树
		除最下面两层，其余节点都有两个孩子，且最底层的所有节点都挤在树的左边。视觉上是三角形的右下角缺

	记二叉树节点n0、n1、n2，分别表示度为0、度为1、度为2的节点，则
		节点数 = n0 + n1 + n2
		度总和 = n1 + 2 * n2
		由于根节点没有入边，因此：
		节点数 - 1 = 度总和
		于是有：
			n0 = n2 + 1
		即 叶子节点数量 = 双孩子节点数量 + 1
	
*/

/*
	二叉树的存储结构

	顺序存储
		将所有树节点的元素，以数组的形式储存，用其下标访问
		对于下标为i的节点，其左孩子的下标为2*i+1，右孩子为2*i+2
		根节点下标为0
		如果2*i+1超过了数组长度，则i为叶子节点

	链式存储，也称二叉链
		树的节点结构体包括：元素、左孩子指针、右孩子指针（、父节点指针）
*/

/*
	二叉树的遍历

	先序遍历：先访问自身，先序遍历左子树，先序遍历右子树

	中序遍历：中序遍历左子树，访问自身，中序遍历右子树

	后序遍历：后序遍历左子树，后序遍历右子树，访问自身

	层次遍历：逐层访问树的节点
*/

struct BTNode
{
	int data;
	char cdata;
	BTNode* lchild;
	BTNode* rchild;
};

//遍历的递归算法
//三种遍历的定义本身就是递归的，因此递归的遍历很好写
//只需要在递归函数里改变访问的位置即可
void PreOrder(BTNode* p)
{
	if (p)
	{
		//访问p
		PreOrder(p->lchild);
		PreOrder(p->rchild);
	}
}

void InOrder(BTNode* p)
{
	if (p)
	{
		InOrder(p->lchild);
		//访问p
		InOrder(p->rchild);
	}
}

void PostOrder(BTNode* p)
{
	if (p)
	{
		PostOrder(p->lchild);
		PostOrder(p->rchild);
		//访问p
	}
}

//层次遍历需要借用队列
void LevelOrder(BTNode* p)
{
	BTNode* queue[20];
	BTNode* q;
	int front = -1, rear = 0;
	queue[rear] = p;
	while (front < rear)//当队不空
	{
		//出队并访问
		q = queue[++front];

		//将q的孩子入队
		queue[++rear] = q->lchild;
		queue[++rear] = q->rchild;
	}
}

//遍历的非递归算法
//非递归遍历的算法都需要借助栈，都采用回溯法

//先序遍历
/*
	思路：先访问自身，再访问左右孩子，但是自身还需要用到。
		 因为先访问左孩子，然后对左孩子先序遍历，遍历完需要去遍历自身的右子树
		 需求是先进后出
		 而其实现方法有两种：
			1.	先将根节点入栈。
				循环体：出栈顶元素p，先将右孩子入栈，再将左孩子入栈
			2.	访问一个点，循环将其所有左孩子入栈，循环结束后，访问出栈元素的右孩子，直到栈空
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
		//输出q
		//先进右孩子，再进左孩子
		if (q->rchild)
			st[++top] = q->rchild;
		if (q->lchild)
			st[++top] = q->lchild;
	}
}

//抄写
void PreOrder3(BTNode* p)
{
	if (!p)return;
	BTNode* st[20];
	int top = -1;
	st[++top] = p;

	while (top >= 0)
	{
		p = st[top--];
		//输出p

		if (p->rchild)
			st[++top] = p->rchild;
		if (p->lchild)
			st[++top] = p->lchild;
	}
}

//思路：总是不断访问左孩子，如果没有左孩子，访问右孩子
//版本1
void InOrder2(BTNode* p)
{
	BTNode* st[20];
	int top = -1;

	while (top >= 0 || p)//
	{
		//对每一个左子树遍历前，先保存他的父节点
		while (p)
		{
			//访问p
			st[++top] = p;
			p = p->lchild;
		}

		if (top >= 0)
		{
			p = st[top--];
			//访问p
			p = p->rchild;
		}
	}
}

//版本2
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
	后序遍历

	先把左子树保存起来，访问完右子树再出栈访问左子树
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
		//到达该节点的左子树的最左端q，接着访问q的右子树

		r = nullptr;
		while (top >= 0)
		{
			q = st[top];//q取栈顶
			if (q->rchild == r)//如果q没有右孩子，或者右孩子被访问过
			{
				//访问q
				//出栈并赋给r
				r = st[top--];//把栈顶出给r，下一个循环的q = st[top];q就取到了r的父亲。那时q会继续出栈。
			}
			else
			{
				p = r->rchild;
				break;//存在右子树，指向它，然后退出内循环，让下一个外循环对q的左子树进栈
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
	根据先序、中序、后序中的任意两个序列构造二叉树
*/


/*	
	先中构造
	先序序列的第一个元素就是树根，然后在后序序列中找到根，其下标为i
	那么其左子树元素为i，右子树为序列长度-1-i
	然后递归，传入对应得元素即可
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
	后中构造
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
	树得其他算法
*/












