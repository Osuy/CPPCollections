#pragma once

#include <vector>
using std::vector;


struct TreeNode
{
	int val;
	TreeNode* lchild;
	TreeNode* rchild;
};


//�������
void PreOrder(TreeNode* root)
{
	TreeNode* p;
	TreeNode* st[20];
	int top = -1;
	st[++top] = root;

	while (top >= 0)
	{
		p = st[top--];
		if (p->rchild)st[++top] = p->rchild;
		if (p->lchild)st[++top] = p->lchild;
	}
}

//�������
void PreOrder(TreeNode* root)
{
	TreeNode* p = root;
	TreeNode* st[20];
	int top = -1;

	while (top >= 0||p)
	{
		while (p)
		{
			//�˴�����Ϊ�������
			st[++top] = p;
			p = p->lchild;
		}

		if (top >= 0)
		{
			//�˴���λΪ�������
			p = st[top--];
			p = p->rchild;
		}
	}
}

//�������2
void PreOrder2(TreeNode* root)
{
	TreeNode* p = root;
	TreeNode* st[20];
	int top = -1;

	while (p || top >= 0)
	{
		if (p)
		{
			//�˴�����Ϊ�������
			st[++top] = p;
			p = p->lchild;
		}
		else
		{
			//�˴�����Ϊ�������
			p = st[top--];
			p = p->rchild;
		}
	}
}

//����������޸������������������˳��Ȼ���ڽ����ʴ�������
void PostOrder(TreeNode* root)
{
	TreeNode* p = root;
	TreeNode* st[20];
	int top = -1;

	//�������У�������õõ������������
	vector<int> res;

	st[++top] = root;

	while (top >= 0)
	{
		p = st[top--];
		res.push_back(p->val);
		if (p->rchild)st[++top] = p->rchild;
		if (p->lchild)st[++top] = p->lchild;
	}

	//����res��Ȼ��������ʼ��õ������������ 
	reverse(res.begin(), res.end());
}