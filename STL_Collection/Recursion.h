#pragma once


/*
	hanoi�����⣬�򵥵ĵݹ����
	���Ƚ����ݹ��ģ��
	����2��������A���ϣ����Ծ���B���ƶ���C��
	����n��������A���ϣ����ϵ�n-1�����Ӿ���C���ƶ���B�����ٰ����һ�������Ƶ�C��
		���B���ϵ�n-1�����Ӿ���A���ƶ���C����

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
	β�ݹ飺�ں��������һ���������ĵݹ麯����β�ݹ�
	β�ݹ�һ�������ѭ��ת��

	��β�ݹ�һ�������ջת��
*/

/*
	0/1��������
	���в�ͬ�����ͼ�ֵ��n����Ʒ�ѡȡ������������W����ֵ���Ĳ�����Ʒ

	�ݹ�ģ�ͣ�
		��Ȼ��ÿ��ѡһ����Ʒ��Ҫ�����¶���һ���ģ����ڴ���Ƶݹ��ѭ��
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

//log2(n)���Ӷȼ���x^n
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

//��������k�Ľڵ�
struct ListNode
{
	ListNode* next;
};

//��������һ���򵥵ĵݹ飬��������β����ʼ��k�ݼ������أ����ص���1���֡���ǵ�����k���ڵ�
int LastK(ListNode* h, ListNode*& result,int k)
{
	if (!h)return k;
	else
	{
		int tmp = LastK(h->next, result, k);
		if (tmp == 1)//����õ���ֵΪ1����õ���ǵ�����k���ڵ�
		{
			result = h;
		}
		return tmp - 1;//���õ���ֵ��1����
	}
}


//�ݹ鷽�����õ�����
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
