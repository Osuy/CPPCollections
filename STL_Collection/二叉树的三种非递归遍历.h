#pragma once

#include <vector>
using std::vector;


struct TreeNode
{
	int val;
	TreeNode* lchild;
	TreeNode* rchild;
};


//先序遍历
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

//中序遍历
void PreOrder(TreeNode* root)
{
	TreeNode* p = root;
	TreeNode* st[20];
	int top = -1;

	while (top >= 0||p)
	{
		while (p)
		{
			//此处访问为先序遍历
			st[++top] = p;
			p = p->lchild;
		}

		if (top >= 0)
		{
			//此处方位为中序遍历
			p = st[top--];
			p = p->rchild;
		}
	}
}

//中序遍历2
void PreOrder2(TreeNode* root)
{
	TreeNode* p = root;
	TreeNode* st[20];
	int top = -1;

	while (p || top >= 0)
	{
		if (p)
		{
			//此处访问为先序遍历
			st[++top] = p;
			p = p->lchild;
		}
		else
		{
			//此处访问为中序遍历
			p = st[top--];
			p = p->rchild;
		}
	}
}

//后序遍历：修改先序遍历的左右子树顺序，然后在将访问次序逆置
void PostOrder(TreeNode* root)
{
	TreeNode* p = root;
	TreeNode* st[20];
	int top = -1;

	//保存序列，最后逆置得到后序遍历序列
	vector<int> res;

	st[++top] = root;

	while (top >= 0)
	{
		p = st[top--];
		res.push_back(p->val);
		if (p->rchild)st[++top] = p->rchild;
		if (p->lchild)st[++top] = p->lchild;
	}

	//逆置res，然后遍历访问即得到后序遍历序列 
	reverse(res.begin(), res.end());
}