#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <stdlib.h>
using namespace std;
/*
	二叉树的主要题目都是用递归来求解
	除非题目要求用非递归
	那样则使用栈。
	没有其他方法
*/

struct BTNode
{
	int data;
	char cdata;
	BTNode* lchild;
	BTNode* rchild;
};

//判断是否为二叉排序树
bool childBST(BTNode* p, long lower, long upper)
{
	if (!p)return true;

	if (p->data <= lower || p->data >= upper)return false;

	return childBST(p->lchild, lower, p->data) && childBST(p->rchild, p->data, upper);
}

bool isValidBST(BTNode* root)
{
	return childBST(root, LONG_MIN, LONG_MAX);
}

//将二叉树链化为只有右孩子的不平衡树
BTNode* Listify(BTNode* p)
{
	if (!p)return nullptr;

	BTNode* l = Listify(p->lchild);
	BTNode* r = Listify(p->rchild);

	if (!l && !r)return p;
	else if (!l) {
		return r;
	}
	else if (!r) {
		p->rchild = p->lchild;
		p->lchild = nullptr;
		return l;
	}
	else {
		l->rchild = p->rchild;
		p->rchild = p->lchild;
		p->lchild = nullptr;
		return r;
	}
}

void flatten(BTNode* root) {
	Listify(root);
}

//求二叉树是否中轴对称
bool _isSymmetric(BTNode* p, BTNode* q)
{
	if (!q && !p)return true;
	if (!p || !q)return false;
	return p->data == q->data && _isSymmetric(p->lchild, q->rchild) && _isSymmetric(p->rchild, q->lchild);
}

bool IsSymmetric(BTNode* root)
{
	return _isSymmetric(root->lchild, root->rchild);
}

//求a树中是否存在子树与b树相同
string dfs(BTNode* root)
{
	if (!root)return "";
	string str = dfs(root->lchild) + to_string(root->data) + dfs(root->rchild);
	return str;
}

string dfs(BTNode* root, const string& b, vector<BTNode*>& res)
{
	if (!root)return "";
	string str = dfs(root->lchild, b, res) + to_string(root->data) + dfs(root->rchild, b, res);

	if (str == b)
	{
		res.push_back(root);
	}
	return str;
}

bool HasSameSubTree(BTNode* a, BTNode* b)
{
	vector<BTNode*>res;
	string b_str = dfs(b);

	dfs(a, b_str, res);

	if (res.size() > 0)
		return true;

	return false;
}

//寻找重复的子树，返回包含所有重复子树序列字符串的数组
//使用深度优先遍历
//每遍历一个点，就将起左右子树和自身的值拼接成一个字符串
//并将这个字符串添加到mp
//如果mp对于的值等于1，说明重复，添加到数组，
//否则，要么为0，设为1，要么大于1，已经添加了，不做事
string dfs(BTNode* root, vector<BTNode*>& res, unordered_map<string, int>& mp)
{
	if (!root)return "";
	string str = dfs(root->lchild, res, mp) + root->cdata + dfs(root->rchild, res, mp);

	if (mp[str] == 1)
	{
		res.push_back(root);
	}

	mp[str]++;
	return str;
}

vector<BTNode*> findDuplicateSubtrees(BTNode* root)
{
	vector<BTNode*>res;
	unordered_map<string, int>mp;

	dfs(root, res, mp);
	return res;
}

//判断一棵树是否是满二叉树
int _IsFull(BTNode* b, int i)
{
	if (!b)
		return i;
	else
	{
		int height1 = _IsFull(b->lchild, i + 1);
		int height2 = _IsFull(b->rchild, i + 1);
		return height1 == height2 ? height1 : 0;
	}

}

bool IsFullTree(BTNode* b)
{
	return _IsFull(b, 1) > 0;
}

//判断一棵树是否是完全二叉树
//层次遍历，遇到第一个空节点，判断队列中剩余的节点是否都为空
bool IsComplete(BTNode* b)
{
	BTNode* q;
	BTNode* queue[50];
	int front = -1;
	int rear = 0;
	queue[rear] = b;

	int LevelStart = 0;
	int NumOfLevel = 1;
	int count = 0;
	while (front < rear)
	{
		q = queue[++front];

		if (q)
		{
			queue[++rear] = q->lchild;
			queue[++rear] = q->rchild;
		}
		else
			break;
	}

	while (front < rear)
	{
		q = queue[++front];
		if (q) return false;
	}
	return true;
}


//求树节点的个数
int NumOfTree(BTNode* b)
{
	if (!b)
		return 0;
	else
		return 1 + NumOfTree(b->lchild) + NumOfTree(b->rchild);
}

//求树的叶子节点的个数
int NumOfLeave(BTNode* b)
{
	if (!b)
		return 0;
	else
	{
		int leave = NumOfLeave(b->lchild) + NumOfLeave(b->rchild);
		return leave == 0 ? 1 : leave;
	}	
}

//求树在k层的节点个数
int NumOfLevel(BTNode* b, int k)
{
	if (k == 0)
		return 1;
	else
		return NumOfLevel(b->lchild, k - 1) + NumOfLevel(b->rchild, k - 1);
}

//求树的深度
int _depthOfTree(BTNode* b, int n)
{
	if (!b)return n;
	return max(_depthOfTree(b->lchild, n + 1), _depthOfTree(b->rchild, n + 1));
}

int DepthOfTree(BTNode* b)
{
	return _depthOfTree(b, 0);
}