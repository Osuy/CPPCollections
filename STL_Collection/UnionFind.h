#pragma once
#include <vector>
#include <algorithm>
using namespace std;

class UnionFind
{
private:
	size_t _count;//��ͨ�����ĸ���
	vector<int> _parent;//�±꣺�ڵ㣬ֵ���ڵ�ĸ�
	vector<int> _size;//���ĺ�������
	
	//����x�ڵ�ĸ�������·��ѹ��
	int find(int x)
	{
		while (x != _parent[x])
		{
			//���x�ĸ��ڵ㲻���Լ�����˵���и��ף�ʹ���ڵ�Ϊ���׵ĸ��ڵ�
			_parent[x] = _parent[_parent[x]];

			//x��ָ���µĸ��ڵ�
			x = _parent[x];
			//������һ��ѭ���������ǰ��x�����Ǹ��������ڸ�
			//�������x�ĸ�Ϊ���ĸ�
			//x��ָ���¸�
		}
		return x;
	}

public:
	UnionFind(int n) :_count(n), _parent(n), _size(n, 1)
	{
		//��ÿ���ڵ�ĸ������Լ�
		for (int i = 0; i < n; ++i)
			_parent[i] = i;
	}

	//��p��q����һ������
	void Union(int p, int q)
	{
		int rootOfp = find(p);
		int rootOfq = find(q);

		//����Ѿ���һ�����ϣ�ֱ��return
		if (rootOfp == rootOfq)return;
		
		//���p���ڵ����Ľڵ��������q���ڵĽڵ����
		//����С���ӵ������ϣ�����ͷ�ؽ���
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

		//��ͨ��������һ��
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