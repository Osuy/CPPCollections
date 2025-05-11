#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <stdlib.h>
using namespace std;
/*
	����������Ҫ��Ŀ�����õݹ������
	������ĿҪ���÷ǵݹ�
	������ʹ��ջ��
	û����������
*/

struct BTNode
{
	int data;
	char cdata;
	BTNode* lchild;
	BTNode* rchild;
};

//�ж��Ƿ�Ϊ����������
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

//������������Ϊֻ���Һ��ӵĲ�ƽ����
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

//��������Ƿ�����Գ�
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

//��a�����Ƿ����������b����ͬ
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

//Ѱ���ظ������������ذ��������ظ����������ַ���������
//ʹ��������ȱ���
//ÿ����һ���㣬�ͽ������������������ֵƴ�ӳ�һ���ַ���
//��������ַ�����ӵ�mp
//���mp���ڵ�ֵ����1��˵���ظ�����ӵ����飬
//����ҪôΪ0����Ϊ1��Ҫô����1���Ѿ�����ˣ�������
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

//�ж�һ�����Ƿ�����������
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

//�ж�һ�����Ƿ�����ȫ������
//��α�����������һ���սڵ㣬�ж϶�����ʣ��Ľڵ��Ƿ�Ϊ��
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


//�����ڵ�ĸ���
int NumOfTree(BTNode* b)
{
	if (!b)
		return 0;
	else
		return 1 + NumOfTree(b->lchild) + NumOfTree(b->rchild);
}

//������Ҷ�ӽڵ�ĸ���
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

//������k��Ľڵ����
int NumOfLevel(BTNode* b, int k)
{
	if (k == 0)
		return 1;
	else
		return NumOfLevel(b->lchild, k - 1) + NumOfLevel(b->rchild, k - 1);
}

//���������
int _depthOfTree(BTNode* b, int n)
{
	if (!b)return n;
	return max(_depthOfTree(b->lchild, n + 1), _depthOfTree(b->rchild, n + 1));
}

int DepthOfTree(BTNode* b)
{
	return _depthOfTree(b, 0);
}