#pragma once


/*
	hanoi塔问题，简单的递归调用
	首先建立递归的模型
	对于2个碟子在A柱上，可以经过B柱移动到C柱
	对于n个碟子在A柱上，其上的n-1个碟子经过C柱移动到B柱，再把最后一个碟子移到C柱
		最后，B柱上的n-1个碟子经过A柱移动到C柱。

*/
void Hanoi(int n, char A, char B, char C)
{
	if (n == 1);
	//move n from a through b to c
	else
	{
		Hanoi(n - 1, A, C, B);
		//move n from a c
		Hanoi(n - 1, B, A, C);
	}
}


/*
	尾递归：在函数题最后一句调用自身的递归函数是尾递归
	尾递归一般可以用循环转换

	非尾递归一般可以用栈转换
*/

/*
	0/1背包问题
	在有不同重量和价值的n件物品里，选取总重量不超过W，价值最大的部分物品

	递归模型：
		显然，每次选一件物品所要做的事都是一样的，基于次设计递归或循环
*/

struct stuff
{
	int weight;
	int wealth;
	bool chosed;
};

struct stuffArray
{
	stuff* items[20];
	int len;
};

void chooseMaxWealth(stuff* item, int weight)
{
	if (item->weight < weight)
	{

	}

}

//log2(n)复杂度计算x^n
int exp(int x, int n)
{
	if (n == 1)
		return x;
	else if (n % 2 == 1)
	{
		return x * exp(x * x, n / 2);
	}
	else
	{
		return exp(x * x, n / 2);
	}
		
}

//求链表倒数k的节点
struct ListNode
{
	ListNode* next;
};

//本质上是一个简单的递归，到达链表尾部后开始将k递减并返回，返回的是1则该帧就是倒数第k个节点
int LastK(ListNode* h, ListNode*& result,int k)
{
	if (!h)return k;
	else
	{
		int tmp = LastK(h->next, result, k);
		if (tmp == 1)//如果得到的值为1，则该点就是倒数第k个节点
		{
			result = h;
		}
		return tmp - 1;//将得到的值减1返回
	}
}


//递归方法逆置单链表，
void ReverseList(ListNode* h)
{
	ListNode* p = h->next;
	h->next = nullptr;
	_reverseList(h, p);
}

void _reverseList(ListNode* h, ListNode*& p)
{
	if (p)
	{
		ListNode* q = p->next;
		p->next = h->next;
		h->next = p;
		_reverseList(h, q);
	}
}
