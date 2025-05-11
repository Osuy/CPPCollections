#pragma once

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#define MAXSIZE 20
#define INF 32676
/*
	1.图G是一个非连通图，有28条边，则该图最少有几个顶点。
		答：求非连通图的最少顶点数量，不妨将其看成完全图，那么n(n-1)/2=28，n=8。由于非连通，所以再加1，
			所以n=9
	3.对于稠密图和稀疏图，采用邻接矩阵和邻接表哪个更好
		答：邻接矩阵已经保存了所有边，不过没有的边用INF表示；邻接表只保存存在的边。
			所以稠密图用邻接矩阵，稀疏图用邻接表
	4

	11.Dijkstra算法用于求单源最短路径，用于求一个图的所有顶点对之间的最短路径可以对每个顶点做Dijkstral算法。
		Floyd对比这个方式有何优势
		答：Dijkstra算法只能处理没有负权的有向图，而Floyd可以处理负权。
*/

/*
	【上机实验题】
*/

//1.实现图的邻接矩阵和邻接表的存储


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

//2.实现图的两种遍历
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

//3.求连通图的所有深度优先遍历序列
//略


//4.求连通图的深度优先生成树和广度优先生成树
//略

//5.用prim栓发求最小生成树
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

//6.用Kruskral求最小生成树
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

	//对所有边进行插入排序
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
	for (i = 0, k = 0; k < g.n;++i)//生成树只需要n-1条边
	{
		if (vset[e[i].start] == 0 || vset[e[i].end] == 0)
		{
			vset[e[i].start] = 1;
			vset[e[i].end] = 1;
			++k;
		}
	}
}

//7.用dijkstra算法求带权有向图的最短路径

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


//8.书写floyd算法
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

//9.求AOE网的所有关键活动
//略

//10.求有向图的路径
//（1）求所有路径，使用深度优先遍历
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

//（2）求所有长度为n路径，使用非递归深度遍历
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
			top--;//退栈
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

//（3） 求最短路径

//11.求满足条件的路径
//（1）起点和终点
//（2）必需经过一组点
//（3）必须避开一组点
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

//12.求解两个动物之间通信的最少翻译问题
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