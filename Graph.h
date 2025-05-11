#pragma once
#include <stdlib.h>
#include "Stack and Queue.h"
/*
	图的基本术语
		端点和邻接点

		顶点的入度和出度

		完全图：
			无向图：每两个顶点之间存在边
			有向图：每两个顶点之间存在两条方向不同的边

		稠密图和稀疏图

		子图

		路径：
			路径长度：路径经过的边的数目
			简单路径：路径无重复点
			环/回路：路径的起点和终点相同
			简单环：简单路径且无重复点

		连通：
			连通：两点之间存在路径，则两点连通
			连通图：无向图内任意两点都是连通的
			强连通图：有向图中任意两点都是连通的
			连通分量：无向图或有向图（不要求连通）的极大连通子图。连通图的连通分量就是自身。非连通图的连通分量是某个子图

		权/网：
			图的每条边附带一个对应的数值，数值称为权，带权图称为网
			带权有向图
			带权无向图
*/

/*
	图的存储方式：邻接矩阵
	对于二维数组arr，其元素arr[i][j]值表示点i和点j的关系

*/

#define MAXV 7
#define INF 32767
typedef struct
{
	int no;
	//info
} VetexType;//顶点结构体，存放顶点编号的信息

typedef struct
{
	int edges[MAXV][MAXV];//邻接矩阵
	int n, e;			  //定点数和边数
	VetexType vexs[MAXV]; //存放顶点
} MatGraph;

typedef struct ANode
{
	int adjvex;
	struct ANode* nextarc;
	int weight;
} ArcNode;//边

typedef struct
{
	//info
	ArcNode* firstarc;
} VNode;//邻接表头

typedef struct
{
	VNode adjlist[MAXV];
	int n, e;
} AdjGraph;//邻接表

//创建邻接表
void CreateAdj(AdjGraph*& G, int A[MAXV][MAXV], int n, int e)
{
	int i, j;
	ArcNode* p;
	G = (AdjGraph*)malloc(sizeof(AdjGraph));
	for (i = 0; i < n; ++i)
		G->adjlist[i].firstarc = nullptr;//将表置空

	for (i = 0; i < n; ++i)
	{
		for (j = n - 1; j >= 0; --j)
		{
			if (A[i][j] != 0 && A[i][j] != INF)
			{
				//新建节点
				p = (ArcNode*)malloc(sizeof(ArcNode));
				p->adjvex = j;
				p->weight = A[i][j];
				//头插法插入到G->adjlist[i]中
				p->nextarc = G->adjlist[i].firstarc;
				G->adjlist[i].firstarc = p;
			}
		}
	}
	G->n = n;
	G->e = e;
}

//根据邻接矩阵创建连接表（几乎和CreateAdj相同）
void MatToList(const MatGraph& g, AdjGraph*& G)
{
	int i, j;
	ArcNode* p;
	G = (AdjGraph*)malloc(sizeof(AdjGraph));
	for (i = 0; i < g.n; ++i)
		G->adjlist[i].firstarc = nullptr;

	for (i = 0; i < g.n; ++i)
	{
		for (j = g.n - 1; j >= 0; --j)
		{
			if (g.edges[i][j] != 0 && g.edges[i][j] != INF)
			{
				p = (ArcNode*)malloc(sizeof(ArcNode));
				p->adjvex = j;
				p->weight = g.edges[i][j];
				p->nextarc = G->adjlist[i].firstarc;
				G->adjlist[i].firstarc = p;
			}
		}
	}
	G->n = g.n;
	G->e = g.e;
}

//根据邻接表创建邻接矩阵
void ListToMat(const AdjGraph& g, MatGraph*& G)
{
	int i,j;
	ArcNode* p;
	G = (MatGraph*)malloc(sizeof(MatGraph));

	for (i = 0; i < g.n; ++i)//遍历邻接表的表头数组
	{
		p = g.adjlist[i].firstarc;//取得表头指向的第一个边节点
		while (p != nullptr)//遍历该表头的所有节点
		{
			G->edges[i][p->adjvex] = p->weight;//p>adjvex就是与它相连的顶点的下标
			p = p->nextarc;
		}
	}
	G->n = g.n;
	G->e = g.e;
}

/*
	图的遍历
		深度优先遍历DFS
			从图的某个初始点v开始，访问v，然后选择一个与v相邻且没有被访问过的点w，继续重复深度优先遍历，直至图的所有点都被访问
		广度优先遍历BFS
			从图的某个初始点v开始，访问v，然后访问所有与v相邻且没有被访问的点S，然后从S依次取一个点重复广度优先遍历，直至图的所有点都被访问
*/
int visited[MAXV] = {0};

//深度优先遍历，从顶点v开始
void DFS(AdjGraph* G, int v)
{
	ArcNode* p;
	visited[v] = 1;//将v设为已访问
	p = G->adjlist[v].firstarc;//从邻接表里取得第一个与v相邻得边节点
	while (p != nullptr)
	{
		if (visited[p->adjvex] == 0)//该点未访问过
			DFS(G, p->adjvex);
		p = p->nextarc;
	}
}

//深度优先遍历的非递归算法，基于栈。
//起点v入栈
//循环：获取栈顶，找到第一个访问的点，将其入栈；如果找不到，则将栈顶退出
void DFS2(AdjGraph* G, int v)
{
	ArcNode* p;
	visited[v] = 1;//将v设为已访问
	int st[MAXSIZE];
	
	//v入栈
	int top = 0;
	st[top] = v;

	//从邻接表里取得第一个与v相邻得边节点
	while (top >= 0)
	{
		p = G->adjlist[st[top]].firstarc;
		while (p && visited[p->adjvex] != 0)//找到一个未访问的点
		{
			p = p->nextarc;
		}

		if (p)
		{
			visited[p->adjvex] = 1;
			st[++top] = p->adjvex;
		}
		else
		{
			--top;
		}
	}
}

//广度优先遍历，从顶点v开始。
//和树的层次遍历横向，依赖环形队列
void BFS(AdjGraph* G, int v)
{
	SqQueue* q;
	InitQueue(q);
	visited[v] = 1;
	enQueue(q, v);
	ArcNode* p;
	while (!QueueEmpty(q))
	{
		int e;
		deQueue(q, e);
		p = G->adjlist[e].firstarc;
		while (p != nullptr)
		{
			if (visited[p->adjvex] == 0)
			{
				visited[p->adjvex] = 1;
				enQueue(q, p->adjvex);
			}
			p = p->nextarc;
		}
	}
}

/*
	图遍历算法得应用
	深度优先算法：是否存在路径，寻找一条路径，寻找所有路径
	广度优先算法：寻找最短路径，寻找最远点

*/

//判断u，v之间是否存在路径，使用深度优先算法
//u表示当前点，v表示终点
void ExistPath(AdjGraph* G, int u, int v, bool& has)
{
	int w;
	ArcNode* p;
	visited[u] = 1;
	if (u == v)
	{
		has = true;
		return;
	}

	p = G->adjlist[u].firstarc;
	while (p != nullptr)
	{
		w = p->adjvex;
		if (visited[w] == 0)
		{
			ExistPath(G, w, v, has);
		}
		p = p->nextarc;
	}
}

//寻找所有路径
void FindAllPath(AdjGraph* G, int u, int v )
{
	int w;
	ArcNode* p;
	visited[u] = 1;
	if (u == v)
	{
		return;
	}

	p = G->adjlist[u].firstarc;
	while (p != nullptr)
	{
		w = p->adjvex;
		if (visited[w] == 0)
		{
			FindAllPath(G, w, v);
		}
		p = p->nextarc;
	}
	visited[u] = 0;
}

typedef struct
{
	int data;
	int parent;//用于寻找前驱
} QUERE;

//最短路径，基于非环形队列和广度优先遍历
void ShortPath(AdjGraph* G, int u, int v)
{
	ArcNode* p;
	int w,i;
	QUERE qu[MAXSIZE];
	int front = -1, rear = -1;

	//将u入栈
	++rear;
	qu[rear].data = u;
	qu[rear].parent = -1;

	//访问u
	visited[u] = 1;

	while (front != rear)
	{
		++front;
		w = qu[front].data;
		if (w == v)
		{
			//找到
			return;
		}

		//广度优先遍历
		p = G->adjlist[u].firstarc;
		while (p != nullptr)
		{
			if (visited[p->adjvex] == 0)
			{
				visited[p->adjvex] = 1;
				++rear;
				qu[rear].data = p->adjvex;
				qu[rear].parent = front;
			}
			p = p->nextarc;
		}
	}
}

//寻找最远点的距离，基于环形队列和广度优先遍历（多个最远点，返回最后一个）
int MaxDist(AdjGraph* G, int v)
{
	ArcNode* p;
	int qu[MAXSIZE];
	int front = -1;
	int rear = 0;
	qu[rear] = v;
	int k;

	visited[v] = 1;
	while (front != rear)
	{	
		//队首出队，赋给k
		front = (front + 1) % MAXSIZE;
		k = qu[front];
		p = G->adjlist[k].firstarc;
		while (p)
		{
			if (visited[p->adjvex] == 0)
			{
				visited[p->adjvex] = 1;
				rear = (rear + 1) % MAXSIZE;
				qu[rear] = p->adjvex;
			}
			p->nextarc;
		}
	}
}



/*
	【生成树】
	生成树就是连通图的极小连通子图。包含连通图的所有顶点（n个顶点）和n-1条边

	对于n个顶点的生成树，有且仅有n-1条边，多一条就不是树，少一条就不是连通图

	对于带权无向连通图，不同的生成树所有边权值之和也不同。权值和最小的生成树称为最小生成树

	对于深度优先遍历，一旦所有的点被访问过，则递归退出。但并非所有边都会经过。
	因此，如果记录深度优先遍历访问顶点时，经过哪些边，没经过哪些边，就能
	得到一个生成树，称为深度优先生成树。

	相应地，由广度优先遍历得到的树就是广度优先生成树
*/

/*
	【带权图的生成树算法】
	普里姆（Prim）算法
		设带权连通图的顶点集为V，边集为E。
		1.初始化边集TE为空，点集U={v}，v为生成树的根节点，可任意指定。将v到V-U的所有点的边作为侯选边（若没有边，则权值为INF）
		2.从候选边中选择权值最小的边加入TE，再将边在V-U的顶点k加入U。
		3.对于V-U的每一个顶点j，如果原来到j侯选边权值大于(k,j)的权值，则将(k,j)更新为到j的侯选边
			即，之前到j点的权值大于k点到j的权值，那么k加入U之后，肯定考虑从k走到j而不是其他
		4.重复2，3步骤，直到所有顶点都加入U

	普里姆算法，将点集分为两个U和V-U，每次循环找到连接两个点集的最小权值的边，把边在V-U的顶点加入U


*/	

void Prim(const MatGraph& g, int v)
{
	int Weights[MAXV];//保存侯选边的权值,下标是到V-U的对应的点
	int min, i, j, k = 0;
	int closest[MAXV];//保存到对应点最近的是U中的哪个点

	for (i = 0; i < g.n; ++i)
	{
		Weights[i] = g.edges[v][i];//v到所有点的权
		closest[i] = v;            //U到V-U的每个点的边权值最小的点，目前全是v
	}

	for (i = 1; i < g.n; ++i)
	{
		min = INF;
		for (j = 0; j < g.n; ++j)//找到权值最小的点，并赋给k
		{
			if (Weights[j] != 0 && Weights[j] < min)
			{
				min = Weights[j];
				k = j;
			}
		}
		Weights[k] = 0;//将到k的权设为0，表示k已经加入U

		//k加入U后，把原来的候选边，和k到每个点的候选边比较，如果k到j的权更小，就替换
		for (j = 0; j < g.n; ++j)
		{
			if (Weights[j] != 0 && g.edges[k][j] < Weights[j])
			{
				Weights[j] = g.edges[k][j];
				closest[j] = k;
			}
		}
	}
}

//抄写
void Prim(const MatGraph& g, int v)
{
	int Weights[MAXV];
	int closest[MAXV];

	int MIN, k;

	for (int i = 0; i < g.n; ++i)
	{
		Weights[i] = g.edges[v][i];
		closest[i] = v;
	}

	for (int i = 0; i < g.n; ++i)
	{
		MIN = INF;
		for (int j = 0; j < g.n; ++j)
		{
			if (Weights[j] != 0 && Weights[j] < MIN)
			{
				MIN = Weights[j];
				k = j;
			}
		}

		Weights[k] = 0;

		for (int j = 0; j < g.n; ++j)
		{
			if (Weights[j] != 0 && Weights[j] > g.edges[k][j])
			{
				Weights[j] = g.edges[k][j];
				closest[j] = k;
			}
		}
	}
}


/*
	【带权图的生成树算法】
	克鲁斯卡尔（Kruskal）算法
		设带权连通图的顶点集为V，边集为E。
		1.初始化边集TE为空，点集U=V，即包含了连通图中的所有点。
		2.从E中由小到大依次选取边，如果改变没有使U形成回路，则加入TE，否则舍弃
		3.重复2步骤，直到TE中有n-1条边
*/



/*
	狄克斯特拉（Dijkstra）算法
	对于带权图，求一个顶点到其他所有顶点的最短路径

	基本思想：
		将有向带权图的顶点分为两个集合，已求出v到其最短路径的顶点的集合S，和剩余顶点U

		1.初始时，S仅包含v。v到U中任一顶点i的最短路径为到i的边的权值（没有边则路径长度为INF）
		2.从U中选取到S权值最小的边，其另一顶点为u，将u加入S
		3.以u为中间点，若v到u再到其余点的路径的权值和更小，则修改v到对应点最短路径的值
		4.重复2和3

	一般而言，存放一条路径需要一个数组，存放路径上的所有边。那么存放所有最短路径需要一个二维数组。
	但是狄克斯特拉算法存放所有的最短路径只用了一个一维数组。
	其原理是。如果v,...,a,j是v到j的最短路径，其中a为j的前一个顶点。那么v,...,a就是v到a的最短路径。
	那么对于j，只需要在数组里保存a即可。即path[j]=a；


*/

void Dijkstra(const MatGraph& g,int v)
{
	int Path[MAXV];
	int dist[MAXV];
	int S[MAXV];
	int i,j,k;
	int min;
	for (i = 0; i < g.n; ++i)
	{
		dist[i] = g.edges[v][i];//直接将v到所有点的边作为最短路径
		S[i] = 0;
		if (g.edges[v][i] < INF)//如果存在边，则v为前驱节点，否则为-1
			Path[i] = v;
		else
			Path[i] = -1;
	}
	//将v加入S集，v到v的前驱为-1；
	S[v] = 1;
	Path[v] = -1;

	for (i = 0; i < g.n; ++i)//每次循环就加入一个到S集，循环次数<n
	{
		min = INF;
		for (j = 0; j < g.n; ++j)//寻找S集到剩余点的最小权值边
		{
			if (S[j] == 0 && dist[j] < min)//如果j不属于S，且到j的距离更小
			{
				min = dist[j];//到到j的距离赋给dist
				k = j;        //j赋给k 
			}
		}
		//退出循环后，找到的k是S集到剩余点的最小距离的点，将k加入S集
		S[k] = 1;

		//因为k加入S，更新其他最短路径，如果v先到k再到j的路径小于v直接到j，那么更新dist[j]
		//并把k作为v到j的前驱节点
		for (j = 0; i < g.n; ++j)
		{
			if (S[j] == 0)
			{
				if (g.edges[k][j] < INF && dist[k] + g.edges[k][j] < dist[j])
				{
					dist[j] = dist[k] + g.edges[k][j];
					Path[j] = k;
				}
			}
		}
	}
}

void Dijkstra(const MatGraph& g, int v)
{
	int  path[MAXSIZE];
	int dist[MAXSIZE];
	int s[MAXSIZE];
	int i, j, k;
	int min;
	
	for (i = 0; i < g.n; i++)
	{
		s[i] = 0;
		path[i] = v;
		dist[i] = g.edges[v][i];
	}
	s[v] = 1;
	path[v] = -1;

	for (i = 0; i < g.n; i++)
	{
		min = INF;
		for(j=0;j<g.n;j++)
			if (s[j] == 0 && dist[j] < min)
			{
				min = dist[j];
				k = j;
			}

		s[k] = 1;
		
		for (j = 0; j < g.n; j++)
			if (s[j] == 0 && dist[j] > dist[k] + g.edges[k][j])
			{
				dist[j] = dist[k] + g.edges[k][j];
				path[j] = k;
			}
	}
}

/*
	弗洛伊德（Floyd）算法
	求带权有向图之间，每对顶点的最短路径。

	在考虑求每对顶点的最短路径之前，回顾一下狄克斯特拉算法。它是求点v到所有点的最短路径。
	换言之，弗洛伊德算法就是对每个点做一次狄克斯特拉算法。
	从需求上将，需要一个二维数组来存储最短路径。
	和狄克斯特拉算法一样。数组Path[i][j]存放的是点i到点j最短路径的上一个点。
	只要追溯就可以获得完整的最短路径。
	但弗洛伊德的逻辑并不是对每个点做狄克斯特拉。
	而是通过对邻接矩阵不断迭代得到路径数组

	具体思路是：
		1.创建两个二维数组，一个拷贝邻接矩阵，另一个初始化做：
			遍历邻接矩阵，若G[i][j]!=(0|INF)，则Path[i][j]=i，否则=-1；
			即点i到点j的最短路径就是它们的连线，如果没有，就设为-1；
		2.循环：取一点，
*/

void Floyd(const MatGraph& g)
{
	int A[MAXV][MAXV];//记录了定点对之间最短路径的权值和
	int Path[MAXV][MAXV];//记录顶点对最短路径的前驱节点
	int i, j, v;

	//初始化两个数组
	for (i = 0; i < g.n; ++i)
	{
		for (j = 0; j < g.n; ++j)
		{
			A[i][j] = g.edges[i][j];
			if (A[i][j] != INF)
				Path[i][j] = i;
			else
				Path[i][j] = -1;
		}
	}
	
	//以下就是floyd算法的主要逻辑，是一个很简单的3重循环
	//外循环遍历用v所有顶点sSA
	//里面的2重循环遍历矩阵，如果i到j的长度大于i到v再到j的长度，就修改路径长度和Path（设v到j的前一个点为a）
	for (v = 0; v < g.n; ++v)
	{
		for (i = 0; i < g.n; ++i)
		{
			if (i != v && A[i][v] != INF)
				for (j = 0; j < g.n; ++j)
				{
					if (i != j && v != j && A[i][v] + A[v][j] < A[i][j])//如果i，j，v互不相等，且i到v再到j的距离小于i直接到j，那么更新
					{
						A[i][j] = A[i][v] + A[v][j];
						Path[i][j] = Path[v][j];
					}
				}
		}
	}
}

void Floyd(const MatGraph& g)
{
	int path[MAXSIZE][MAXSIZE];
	int A[MAXSIZE][MAXSIZE];
	int i, j, k;

	for (i = 0; i < g.n; i++)
	{
		for (j = 0; j < g.n; j++)
		{
			A[i][j] = g.edges[i][j];
			if (g.edges[i][j] != 0)
				path[i][j] = i;
			else
				path[i][j] = -1;
		}
	}

	for (k = 0; k < g.n; k++)
	{
		for (i = 0; i < g.n; i++)
		{
			for (j = 0; j < g.n; j++)
			{
				if (A[i][j] > A[i][k] + A[k][j])
				{
					A[i][j] = A[i][k] + A[k][j];
					path[i][j] = path[k][j];
				}
			}
		}
	}
}

void Floyd2(const MatGraph& g)
{
	int path[MAXSIZE][MAXSIZE];
	int A[MAXSIZE][MAXSIZE];
	int i, j, k;

	for (i = 0; i < g.n; ++i)
	{
		for (j = 0; j < g.n; ++j)
		{
			A[i][j] = g.edges[i][j];
			if (g.edges[i][j] != 0)
			{
				path[i][j] = i;
			}
			else
			{
				path[i][j] = -1;
			}
		}
	}

	for (k = 0; k < g.n; ++k)
	{
		for (i = 0; i < g.n; ++i)
		{
			if (i != k && A[i][k] != INF)
			{
				for (j = 0; j < g.n; ++j)
				{
					if (i != j && k != j && A[i][k] + A[k][j] < A[i][j])
					{
						A[i][j] = A[i][k] + A[k][j];
						path[i][j] = path[k][j];
					}
				}
			}
		}
	}
}


/*
	拓扑排序

		逐个找到有向图中入读为0的顶点，将其取出，再将其出边的另一顶点的入度减1，直到所有顶点都被取出

	主要步骤
		1.将所有入度为0的顶点入队
		2.出队一个顶点，将其所有出边所指的顶点入度减1，
*/

void TopSrot(MatGraph& g)
{
	int st[MAXSIZE];
	int top = -1;

	int i,j;

	//先将所有入度为0的点入栈
	for (i = 0; i < g.n; ++i)
	{
		int count = 0;
		for (j = 0; j < g.n; ++j)
		{
			if (i != j && g.edges[j][i] != INF)
			{
				++count;
			}
		}

		if (count == 0)
		{
			++top;
			st[top] = i;
		}
	}

	while (top >= 0)
	{
		//出栈一个点，将其所有
		i = st[top];
		--top;

		for (j = 0; j < g.n; ++j)
		{
			if (i != j && g.edges[i][j] != INF)
			{
				++top;
				st[top] = j;
			}
		}
	}
}


/*
	AOE网与关键路径
	若用一个有向无环图来描述一个工程。
	通常每个工程只有一个入度为0的点，称为源点；只有一个出入为0的点，称为汇点

	若图有多个入度为0的点，则新建一个点，由它引出一条出边指向所有入度为0的点，再让它作为新的源点
	若图由多个出度为0的点，同理可以新建一个汇点

	通常，把图的顶点称为事件，把边称为活动。

	对于两条首位相接的边e1和e2，称e1为e2的前驱活动，e2是e1的后继活动

	边的起点称为活动的触发事件，终点称为活动的结束事件

	一个事件的所有前驱活动（一个顶点的所有入边）完成时，才能触发。





*/