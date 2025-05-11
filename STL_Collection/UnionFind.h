#pragma once
#include <vector>
#include <algorithm>
using namespace std;

class UnionFind
{
private:
	size_t _count;//连通分量的个数
	vector<int> _parent;//下标：节点，值：节点的根
	vector<int> _size;//根的孩子数量
	
	//返回x节点的根，附带路劲压缩
	int find(int x)
	{
		while (x != _parent[x])
		{
			//如果x的根节点不是自己，则说明有父亲，使根节点为父亲的父节点
			_parent[x] = _parent[_parent[x]];

			//x再指向新的父节点
			x = _parent[x];
			//对于下一个循环，如果当前的x还不是根，即存在父
			//则继续让x的父为父的父
			//x再指向新父
		}
		return x;
	}

public:
	UnionFind(int n) :_count(n), _parent(n), _size(n, 1)
	{
		//让每个节点的根都是自己
		for (int i = 0; i < n; ++i)
			_parent[i] = i;
	}

	//将p和q加入一个集合
	void Union(int p, int q)
	{
		int rootOfp = find(p);
		int rootOfq = find(q);

		//如果已经在一个集合，直接return
		if (rootOfp == rootOfq)return;
		
		//如果p所在的树的节点个数大于q所在的节点个数
		//就让小树接到大树上，避免头重脚轻
		if (_size[rootOfp] > _size[rootOfq])
		{
			_parent[rootOfq] = rootOfp;
			_size[rootOfp] += _size[rootOfq];
		}
		else
		{
			_parent[rootOfp] = rootOfq;
			_size[rootOfq] += _size[rootOfp];
		}

		//连通分量减少一个
		--_count;
	}

	bool connected(int p, int q)
	{
		return find(p) == find(q);
	}

	int count() const
	{
		return _count;
	}
};