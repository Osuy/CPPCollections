#pragma once
#define MAXSIZE 20





struct LNode
{
	int data;
	LNode* next;

	LNode(int x) :data(x), next(nullptr) {}
};

//逆置带头节点的单链表算法
void ReverseList(LNode*& l)
{
	LNode* q = l->next;
	LNode* p = q->next;
	while (nullptr != p)
	{
		q->next = p->next;
		p->next = l->next;
		l->next = p;
		p = q->next;
	}

}

//递归版
void ReverseList_recursion(LNode* h)
{
	LNode* p = h->next;
	h->next = nullptr;
	_reverseList_recursion(h, p);
}

void _reverseList_recursion(LNode* h, LNode* p)
{
	if (p)
	{
		LNode* q = p->next;
		p->next = h->next;
		h->next = p;
		_reverseList_recursion(h, q);
	}
}

//逆置不带头节点的单链表
void ReverseList2(LNode*& l)
{
	LNode* q;

	while (l->next)
	{
		q = l->next;
		l->next = q->next;
		q->next = l;
	}
}

//单链表实现两个大整数相加
int Plus_List(int a, int b)
{
	LNode* ha, * hb, *ta, * tb;

	ha = (LNode*)malloc(sizeof(LNode));
	ha->next = nullptr;
	ta = ha;
	hb = (LNode*)malloc(sizeof(LNode)); 
	hb->next = nullptr;
	tb = hb;

	while (a != 0)
	{
		LNode* q = (LNode*)malloc(sizeof(LNode));
		q->data = a % 10;
		ta->next = q;
		ta = q;
		a /= 10;
	}

	while (b != 0)
	{
		LNode* q = (LNode*)malloc(sizeof(LNode));
		q->data = b % 10;
		tb->next = q;
		tb = q;
		b /= 10;
	}
	ta = ha->next;
	tb = hb->next;
	int sum;

	while (ta && tb)
	{
		sum = ta->data + tb->data;
	}

}

//对于两个逆序保存了正整数的链表
//求它们相加的结果，并返回同样的逆序链表
LNode* addTwoNumbers(LNode* l1, LNode* l2) {
	LNode* p, * q;
	p = q = new LNode(0);
	int extra;
	while (l1 || l2 || extra)
	{
		if (l1) extra += l1->data, l1 = l1->next;
		if (l2)extra += l2->data, l2 = l2->next;
		p->next = new LNode(extra % 10);//不用p指向新一个指针节点，而是直接用p->next指向，节省了
		p = p->next;

		extra /= 10;
	}
	return q->next;
}