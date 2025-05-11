#pragma once

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#define MAXSIZE 20
#define INF 32676
/*
	1.ͼG��һ������ͨͼ����28���ߣ����ͼ�����м������㡣
		�������ͨͼ�����ٶ����������������俴����ȫͼ����ôn(n-1)/2=28��n=8�����ڷ���ͨ�������ټ�1��
			����n=9
	3.���ڳ���ͼ��ϡ��ͼ�������ڽӾ�����ڽӱ��ĸ�����
		���ڽӾ����Ѿ����������бߣ�����û�еı���INF��ʾ���ڽӱ�ֻ������ڵıߡ�
			���Գ���ͼ���ڽӾ���ϡ��ͼ���ڽӱ�
	4

	11.Dijkstra�㷨������Դ���·����������һ��ͼ�����ж����֮������·�����Զ�ÿ��������Dijkstral�㷨��
		Floyd�Ա������ʽ�к�����
		��Dijkstra�㷨ֻ�ܴ���û�и�Ȩ������ͼ����Floyd���Դ���Ȩ��
*/

/*
	���ϻ�ʵ���⡿
*/

//1.ʵ��ͼ���ڽӾ�����ڽӱ�Ĵ洢


struct GraphMatrix
{
	int edges[MAXSIZE][MAXSIZE];
	int n, e;
};

struct ANode
{
	ANode* next_node;
	int adj_vex;
	int weight;
};

struct HeadNode
{
	ANode* first_node;
};

struct GraphList
{
	HeadNode adj_list[MAXSIZE];
	int n, e;
};

//2.ʵ��ͼ�����ֱ���
int visited[MAXSIZE];

void DFS(const GraphList& g, int v)
{
	visited[v] = 1;
	ANode* p = g.adj_list[v].first_node;
	while (p)
	{
		if (visited[p->adj_vex] == 0)
			DFS(g, p->adj_vex);
		p = p->next_node;
	}
}

void DFS2(const GraphList& g, int v)
{
	int i, top = -1;
	int st[MAXSIZE];
	ANode* p;
	for (i = 0; i < g.n; ++i)
	{
		visited[i] = 0;
	}

	visited[v] = 1;
	++top;
	st[top] = v;
	while (top >= 0)
	{
		p = g.adj_list[st[top]].first_node;
		while (p && st[p->adj_vex] != 0)
			p = p->next_node;

		if (p)
		{
			visited[p->adj_vex] = 1;
			st[++top] = p->adj_vex;
		}
		else
			--top;
	}
}

void BFS(const GraphList& g, int v)
{
	int q[MAXSIZE];
	ANode* p;
	int i, j;
	int front = -1;
	int rear = 0;
	q[rear] = v;
	visited[v] = 1;
	while(front <= rear)
	{
		p = g.adj_list[q[++front]].first_node;
		while (p)
		{
			if (visited[p->adj_vex] == 0)
			{
				visited[p->adj_vex] = 1;
				q[++rear] = p->adj_vex;
			}
			p = p->next_node;
		}
	}
}

//3.����ͨͼ������������ȱ�������
//��


//4.����ͨͼ����������������͹������������
//��

//5.��prim˨������С������
void Prim(const GraphMatrix& g, int v)
{
	int weight[MAXSIZE];
	int closest[MAXSIZE];
	int i, j, k, min;

	for(i=0;i<g.n;++i)
	{
		weight[i] = g.edges[v][i];
		closest[i] = v;
	}

	for (i = 0; i < g.n; ++i)
	{
		min = INF;
		k = v;
		for (j = 0; j < g.n; ++j)
		{
			if (weight[j] != 0 && weight[j] < min)
			{
				min = weight[j];
				k = j;
			}
		}

		weight[k] = 0;

		for (j = 0; j < g.n; ++j)
		{
			if (weight[j] != 0 && weight[j] > g.edges[k][j])
			{
				closest[j] = k;
				weight[j] = g.edges[k][j];
			}
		}
	}
}

//6.��Kruskral����С������
struct Edge
{
	int start,end,weight;

	void operator=(Edge& e) { start = e.start; end = e.end; weight = e.weight; }
};

void Kruskral(const GraphMatrix& g)
{
	Edge e[MAXSIZE];
	int vset[MAXSIZE];
	int i, j, k;
	for (i = 0, k = 0; i < g.n; ++i)
	{
		vset[i] = 0;
		for (j = i + 1; j < g.n; ++j)
			if (i != j && g.edges[i][j] != INF)
			{
				e[k].start = i;
				e[k].end = j;
				e[k].weight = g.edges[i][j];
			}
	}

	//�����б߽��в�������
	for (i = 1; i < g.e; ++i)
	{
		Edge ce = e[i];
		j = i - 1;
		while (j >= 0 && e[j].weight < ce.weight)
		{
			e[j + 1] = e[j];
			--j;
		}

		e[j] = ce;
	}

	vset[e[0].start] = 1;
	for (i = 0, k = 0; k < g.n;++i)//������ֻ��Ҫn-1����
	{
		if (vset[e[i].start] == 0 || vset[e[i].end] == 0)
		{
			vset[e[i].start] = 1;
			vset[e[i].end] = 1;
			++k;
		}
	}
}

//7.��dijkstra�㷨���Ȩ����ͼ�����·��

void Dijkstra(const GraphMatrix& g, int v)
{
	int Path[MAXSIZE];
	int S[MAXSIZE];
	int dist[MAXSIZE];

	int i, j,k, min;
	for (i = 0; i < g.n; ++i)
	{
		Path[i] = v;
		S[i] = 0;
		dist[i] = g.edges[v][i];
	}

	S[v] = 1;

	for (i = 0; i < g.n; ++i)
	{
		min = INF;
		k = v;
		for (j = 0; j < g.n; ++j)
		{
			if (S[j] == 0 && min > dist[j])
			{
				min = dist[j];
				k = j;
			}
		}

		S[k] = 1;

		for (j = 0; j < g.n; ++j)
		{
			if (S[j] == 0 && dist[j] > dist[k] + g.edges[k][j])
			{
				dist[j] = dist[k] + g.edges[k][j];
				Path[j] = k;
			}
		}
	}
}


//8.��дfloyd�㷨
void Floyd(const GraphMatrix& g)
{
	int A[MAXSIZE][MAXSIZE];
	int Path[MAXSIZE][MAXSIZE];
	int i,j;
	for (i = 0; i < g.n; ++i)
	{
		for (j = 0; j < g.n; ++j)
		{
			A[i][j] = g.edges[i][j];
			if (A[i][j] != 0 && A[i][j] != INF)
			{
				Path[i][j] = i;
			}
			else
			{
				Path[i][j] = -1;
			}
		}
	}

	for (i = 0; i < g.n; ++i)
	{
		for (j = 0; j < g.n; ++j)
		{
			if (j == i)continue;
			for (int k = 0; k < g.n; ++k)
			{
				if (k == j || k == i)continue;
				if(A[j][k] > A[j][i] + A[i][k])
				{
					A[j][k] = A[j][i] + A[i][k];
					Path[j][k] = Path[i][k];
				}
			}
		}
	}
}

//9.��AOE�������йؼ��
//��

//10.������ͼ��·��
//��1��������·����ʹ��������ȱ���
void FindAllPath(const GraphList& g, int v, int u)
{
	visited[v] = 1;
	if (v == u)
	{
		//ouput
	}
	ANode* p = g.adj_list[v].first_node;

	while (p)
	{
		if (visited[p->adj_vex] == 0)
			FindAllPath(g, p->adj_vex, u);
		p = p->next_node;
	}

	visited[v] = 0;
}

//��2�������г���Ϊn·����ʹ�÷ǵݹ���ȱ���
void FindAllPath(const GraphList& g, int v, int u, int len)
{
	int st[MAXSIZE];
	int s[MAXSIZE];

	for (int i = 0; i < g.n; ++i)
	{
		s[i] = 0;
	}
	s[v] = 1;
	int top = 0;
	st[top] = v;

	ANode* p;
	while (top >= 0)
	{
		if (st[top] == u && top == len)
		{
			//output;
		}
		else if(top == len)
		{
			top--;//��ջ
		}
		else 
		{
			p = g.adj_list[st[top]].first_node;


			while (p && s[p->adj_vex] != 0)
				p = p->next_node;

			if (p)
				st[++top] = p->adj_vex;
		}
		
	}
}

//��3�� �����·��

//11.������������·��
//��1�������յ�
//��2�����辭��һ���
//��3������ܿ�һ���
void FindAllPath(const GraphList& g, int v, int u,int* goTrough,int* noGoTrough)
{
	int st[MAXSIZE];
	int top = 0;
	st[top] = v;
	int i, j;


	while (top >= 0)
	{
		if (st[top] == u)
		{
			for (i = 0; i < g.n; ++i)
			{
				if (goTrough[i] == 1)
				{
					
				}
			}
		}
	}
}

//12.�����������֮��ͨ�ŵ����ٷ�������
void ShortPath()
{

}

void AnimalTranslate()
{
	int Matrix[5][5] = {0};

	int n, m;
	int i, j;
	for (i = 0; i < 5; ++i)
	{
		for (j = 0; j < 5; ++j)
			Matrix[i][j] = 0;
	}

	cout << "input animal num(2<<n<5):";
	do
	{
		cin >> n;
		if (n > 5)
			cout << endl << "too large" << endl;
		else if (n < 2)
			cout << endl << "too small" << endl;
	} while (n > 5 || n < 2);

	cout << "animal communicate record:";
	do
	{
		cin >> m;
		if (m > 25)
			cout << endl << "too large" << endl;
	} while (m > 25);

	cout << endl;
	for (int i = 1; i <= m; ++i)
	{
		cout << "no." << i << " record" << endl;
		do 
		{
			cin >> i;
			if (i > 5)
				cout << endl << "too large" << endl;
		}while (i > 5);
		do
		{
			cin >> j;
			if (j > 5)
				cout << endl << "too large" << endl;
		} while (j > 5);

		if (i <= 5 && j <= 5)
			Matrix[i - 1][j - 1] = Matrix[j - 1][i - 1] = 1;
	}



}