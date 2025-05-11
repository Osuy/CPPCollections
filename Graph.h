#pragma once
#include <stdlib.h>
#include "Stack and Queue.h"
/*
	ͼ�Ļ�������
		�˵���ڽӵ�

		�������Ⱥͳ���

		��ȫͼ��
			����ͼ��ÿ��������֮����ڱ�
			����ͼ��ÿ��������֮�������������ͬ�ı�

		����ͼ��ϡ��ͼ

		��ͼ

		·����
			·�����ȣ�·�������ıߵ���Ŀ
			��·����·�����ظ���
			��/��·��·���������յ���ͬ
			�򵥻�����·�������ظ���

		��ͨ��
			��ͨ������֮�����·������������ͨ
			��ͨͼ������ͼ���������㶼����ͨ��
			ǿ��ͨͼ������ͼ���������㶼����ͨ��
			��ͨ����������ͼ������ͼ����Ҫ����ͨ���ļ�����ͨ��ͼ����ͨͼ����ͨ����������������ͨͼ����ͨ������ĳ����ͼ

		Ȩ/����
			ͼ��ÿ���߸���һ����Ӧ����ֵ����ֵ��ΪȨ����Ȩͼ��Ϊ��
			��Ȩ����ͼ
			��Ȩ����ͼ
*/

/*
	ͼ�Ĵ洢��ʽ���ڽӾ���
	���ڶ�ά����arr����Ԫ��arr[i][j]ֵ��ʾ��i�͵�j�Ĺ�ϵ

*/

#define MAXV 7
#define INF 32767
typedef struct
{
	int no;
	//info
} VetexType;//����ṹ�壬��Ŷ����ŵ���Ϣ

typedef struct
{
	int edges[MAXV][MAXV];//�ڽӾ���
	int n, e;			  //�������ͱ���
	VetexType vexs[MAXV]; //��Ŷ���
} MatGraph;

typedef struct ANode
{
	int adjvex;
	struct ANode* nextarc;
	int weight;
} ArcNode;//��

typedef struct
{
	//info
	ArcNode* firstarc;
} VNode;//�ڽӱ�ͷ

typedef struct
{
	VNode adjlist[MAXV];
	int n, e;
} AdjGraph;//�ڽӱ�

//�����ڽӱ�
void CreateAdj(AdjGraph*& G, int A[MAXV][MAXV], int n, int e)
{
	int i, j;
	ArcNode* p;
	G = (AdjGraph*)malloc(sizeof(AdjGraph));
	for (i = 0; i < n; ++i)
		G->adjlist[i].firstarc = nullptr;//�����ÿ�

	for (i = 0; i < n; ++i)
	{
		for (j = n - 1; j >= 0; --j)
		{
			if (A[i][j] != 0 && A[i][j] != INF)
			{
				//�½��ڵ�
				p = (ArcNode*)malloc(sizeof(ArcNode));
				p->adjvex = j;
				p->weight = A[i][j];
				//ͷ�巨���뵽G->adjlist[i]��
				p->nextarc = G->adjlist[i].firstarc;
				G->adjlist[i].firstarc = p;
			}
		}
	}
	G->n = n;
	G->e = e;
}

//�����ڽӾ��󴴽����ӱ�������CreateAdj��ͬ��
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

//�����ڽӱ����ڽӾ���
void ListToMat(const AdjGraph& g, MatGraph*& G)
{
	int i,j;
	ArcNode* p;
	G = (MatGraph*)malloc(sizeof(MatGraph));

	for (i = 0; i < g.n; ++i)//�����ڽӱ�ı�ͷ����
	{
		p = g.adjlist[i].firstarc;//ȡ�ñ�ͷָ��ĵ�һ���߽ڵ�
		while (p != nullptr)//�����ñ�ͷ�����нڵ�
		{
			G->edges[i][p->adjvex] = p->weight;//p>adjvex�������������Ķ�����±�
			p = p->nextarc;
		}
	}
	G->n = g.n;
	G->e = g.e;
}

/*
	ͼ�ı���
		������ȱ���DFS
			��ͼ��ĳ����ʼ��v��ʼ������v��Ȼ��ѡ��һ����v������û�б����ʹ��ĵ�w�������ظ�������ȱ�����ֱ��ͼ�����е㶼������
		������ȱ���BFS
			��ͼ��ĳ����ʼ��v��ʼ������v��Ȼ�����������v������û�б����ʵĵ�S��Ȼ���S����ȡһ�����ظ�������ȱ�����ֱ��ͼ�����е㶼������
*/
int visited[MAXV] = {0};

//������ȱ������Ӷ���v��ʼ
void DFS(AdjGraph* G, int v)
{
	ArcNode* p;
	visited[v] = 1;//��v��Ϊ�ѷ���
	p = G->adjlist[v].firstarc;//���ڽӱ���ȡ�õ�һ����v���ڵñ߽ڵ�
	while (p != nullptr)
	{
		if (visited[p->adjvex] == 0)//�õ�δ���ʹ�
			DFS(G, p->adjvex);
		p = p->nextarc;
	}
}

//������ȱ����ķǵݹ��㷨������ջ��
//���v��ջ
//ѭ������ȡջ�����ҵ���һ�����ʵĵ㣬������ջ������Ҳ�������ջ���˳�
void DFS2(AdjGraph* G, int v)
{
	ArcNode* p;
	visited[v] = 1;//��v��Ϊ�ѷ���
	int st[MAXSIZE];
	
	//v��ջ
	int top = 0;
	st[top] = v;

	//���ڽӱ���ȡ�õ�һ����v���ڵñ߽ڵ�
	while (top >= 0)
	{
		p = G->adjlist[st[top]].firstarc;
		while (p && visited[p->adjvex] != 0)//�ҵ�һ��δ���ʵĵ�
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

//������ȱ������Ӷ���v��ʼ��
//�����Ĳ�α��������������ζ���
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
	ͼ�����㷨��Ӧ��
	��������㷨���Ƿ����·����Ѱ��һ��·����Ѱ������·��
	��������㷨��Ѱ�����·����Ѱ����Զ��

*/

//�ж�u��v֮���Ƿ����·����ʹ����������㷨
//u��ʾ��ǰ�㣬v��ʾ�յ�
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

//Ѱ������·��
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
	int parent;//����Ѱ��ǰ��
} QUERE;

//���·�������ڷǻ��ζ��к͹�����ȱ���
void ShortPath(AdjGraph* G, int u, int v)
{
	ArcNode* p;
	int w,i;
	QUERE qu[MAXSIZE];
	int front = -1, rear = -1;

	//��u��ջ
	++rear;
	qu[rear].data = u;
	qu[rear].parent = -1;

	//����u
	visited[u] = 1;

	while (front != rear)
	{
		++front;
		w = qu[front].data;
		if (w == v)
		{
			//�ҵ�
			return;
		}

		//������ȱ���
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

//Ѱ����Զ��ľ��룬���ڻ��ζ��к͹�����ȱ����������Զ�㣬�������һ����
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
		//���׳��ӣ�����k
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
	����������
	������������ͨͼ�ļ�С��ͨ��ͼ��������ͨͼ�����ж��㣨n�����㣩��n-1����

	����n������������������ҽ���n-1���ߣ���һ���Ͳ���������һ���Ͳ�����ͨͼ

	���ڴ�Ȩ������ͨͼ����ͬ�����������б�Ȩֵ֮��Ҳ��ͬ��Ȩֵ����С����������Ϊ��С������

	����������ȱ�����һ�����еĵ㱻���ʹ�����ݹ��˳������������б߶��ᾭ����
	��ˣ������¼������ȱ������ʶ���ʱ��������Щ�ߣ�û������Щ�ߣ�����
	�õ�һ������������Ϊ���������������

	��Ӧ�أ��ɹ�����ȱ����õ��������ǹ������������
*/

/*
	����Ȩͼ���������㷨��
	����ķ��Prim���㷨
		���Ȩ��ͨͼ�Ķ��㼯ΪV���߼�ΪE��
		1.��ʼ���߼�TEΪ�գ��㼯U={v}��vΪ�������ĸ��ڵ㣬������ָ������v��V-U�����е�ı���Ϊ��ѡ�ߣ���û�бߣ���ȨֵΪINF��
		2.�Ӻ�ѡ����ѡ��Ȩֵ��С�ı߼���TE���ٽ�����V-U�Ķ���k����U��
		3.����V-U��ÿһ������j�����ԭ����j��ѡ��Ȩֵ����(k,j)��Ȩֵ����(k,j)����Ϊ��j�ĺ�ѡ��
			����֮ǰ��j���Ȩֵ����k�㵽j��Ȩֵ����ôk����U֮�󣬿϶����Ǵ�k�ߵ�j����������
		4.�ظ�2��3���裬ֱ�����ж��㶼����U

	����ķ�㷨�����㼯��Ϊ����U��V-U��ÿ��ѭ���ҵ����������㼯����СȨֵ�ıߣ��ѱ���V-U�Ķ������U


*/	

void Prim(const MatGraph& g, int v)
{
	int Weights[MAXV];//�����ѡ�ߵ�Ȩֵ,�±��ǵ�V-U�Ķ�Ӧ�ĵ�
	int min, i, j, k = 0;
	int closest[MAXV];//���浽��Ӧ���������U�е��ĸ���

	for (i = 0; i < g.n; ++i)
	{
		Weights[i] = g.edges[v][i];//v�����е��Ȩ
		closest[i] = v;            //U��V-U��ÿ����ı�Ȩֵ��С�ĵ㣬Ŀǰȫ��v
	}

	for (i = 1; i < g.n; ++i)
	{
		min = INF;
		for (j = 0; j < g.n; ++j)//�ҵ�Ȩֵ��С�ĵ㣬������k
		{
			if (Weights[j] != 0 && Weights[j] < min)
			{
				min = Weights[j];
				k = j;
			}
		}
		Weights[k] = 0;//����k��Ȩ��Ϊ0����ʾk�Ѿ�����U

		//k����U�󣬰�ԭ���ĺ�ѡ�ߣ���k��ÿ����ĺ�ѡ�߱Ƚϣ����k��j��Ȩ��С�����滻
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

//��д
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
	����Ȩͼ���������㷨��
	��³˹������Kruskal���㷨
		���Ȩ��ͨͼ�Ķ��㼯ΪV���߼�ΪE��
		1.��ʼ���߼�TEΪ�գ��㼯U=V������������ͨͼ�е����е㡣
		2.��E����С��������ѡȡ�ߣ�����ı�û��ʹU�γɻ�·�������TE����������
		3.�ظ�2���裬ֱ��TE����n-1����
*/



/*
	�ҿ�˹������Dijkstra���㷨
	���ڴ�Ȩͼ����һ�����㵽�������ж�������·��

	����˼�룺
		�������Ȩͼ�Ķ����Ϊ�������ϣ������v�������·���Ķ���ļ���S����ʣ�ඥ��U

		1.��ʼʱ��S������v��v��U����һ����i�����·��Ϊ��i�ıߵ�Ȩֵ��û�б���·������ΪINF��
		2.��U��ѡȡ��SȨֵ��С�ıߣ�����һ����Ϊu����u����S
		3.��uΪ�м�㣬��v��u�ٵ�������·����Ȩֵ�͸�С�����޸�v����Ӧ�����·����ֵ
		4.�ظ�2��3

	һ����ԣ����һ��·����Ҫһ�����飬���·���ϵ����бߡ���ô����������·����Ҫһ����ά���顣
	���ǵҿ�˹�����㷨������е����·��ֻ����һ��һά���顣
	��ԭ���ǡ����v,...,a,j��v��j�����·��������aΪj��ǰһ�����㡣��ôv,...,a����v��a�����·����
	��ô����j��ֻ��Ҫ�������ﱣ��a���ɡ���path[j]=a��


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
		dist[i] = g.edges[v][i];//ֱ�ӽ�v�����е�ı���Ϊ���·��
		S[i] = 0;
		if (g.edges[v][i] < INF)//������ڱߣ���vΪǰ���ڵ㣬����Ϊ-1
			Path[i] = v;
		else
			Path[i] = -1;
	}
	//��v����S����v��v��ǰ��Ϊ-1��
	S[v] = 1;
	Path[v] = -1;

	for (i = 0; i < g.n; ++i)//ÿ��ѭ���ͼ���һ����S����ѭ������<n
	{
		min = INF;
		for (j = 0; j < g.n; ++j)//Ѱ��S����ʣ������СȨֵ��
		{
			if (S[j] == 0 && dist[j] < min)//���j������S���ҵ�j�ľ����С
			{
				min = dist[j];//����j�ľ��븳��dist
				k = j;        //j����k 
			}
		}
		//�˳�ѭ�����ҵ���k��S����ʣ������С����ĵ㣬��k����S��
		S[k] = 1;

		//��Ϊk����S�������������·�������v�ȵ�k�ٵ�j��·��С��vֱ�ӵ�j����ô����dist[j]
		//����k��Ϊv��j��ǰ���ڵ�
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
	�������£�Floyd���㷨
	���Ȩ����ͼ֮�䣬ÿ�Զ�������·����

	�ڿ�����ÿ�Զ�������·��֮ǰ���ع�һ�µҿ�˹�����㷨���������v�����е�����·����
	����֮�����������㷨���Ƕ�ÿ������һ�εҿ�˹�����㷨��
	�������Ͻ�����Ҫһ����ά�������洢���·����
	�͵ҿ�˹�����㷨һ��������Path[i][j]��ŵ��ǵ�i����j���·������һ���㡣
	ֻҪ׷�ݾͿ��Ի�����������·����
	���������µ��߼������Ƕ�ÿ�������ҿ�˹������
	����ͨ�����ڽӾ��󲻶ϵ����õ�·������

	����˼·�ǣ�
		1.����������ά���飬һ�������ڽӾ�����һ����ʼ������
			�����ڽӾ�����G[i][j]!=(0|INF)����Path[i][j]=i������=-1��
			����i����j�����·���������ǵ����ߣ����û�У�����Ϊ-1��
		2.ѭ����ȡһ�㣬
*/

void Floyd(const MatGraph& g)
{
	int A[MAXV][MAXV];//��¼�˶����֮�����·����Ȩֵ��
	int Path[MAXV][MAXV];//��¼��������·����ǰ���ڵ�
	int i, j, v;

	//��ʼ����������
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
	
	//���¾���floyd�㷨����Ҫ�߼�����һ���ܼ򵥵�3��ѭ��
	//��ѭ��������v���ж���sSA
	//�����2��ѭ�������������i��j�ĳ��ȴ���i��v�ٵ�j�ĳ��ȣ����޸�·�����Ⱥ�Path����v��j��ǰһ����Ϊa��
	for (v = 0; v < g.n; ++v)
	{
		for (i = 0; i < g.n; ++i)
		{
			if (i != v && A[i][v] != INF)
				for (j = 0; j < g.n; ++j)
				{
					if (i != j && v != j && A[i][v] + A[v][j] < A[i][j])//���i��j��v������ȣ���i��v�ٵ�j�ľ���С��iֱ�ӵ�j����ô����
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
	��������

		����ҵ�����ͼ�����Ϊ0�Ķ��㣬����ȡ�����ٽ�����ߵ���һ�������ȼ�1��ֱ�����ж��㶼��ȡ��

	��Ҫ����
		1.���������Ϊ0�Ķ������
		2.����һ�����㣬�������г�����ָ�Ķ�����ȼ�1��
*/

void TopSrot(MatGraph& g)
{
	int st[MAXSIZE];
	int top = -1;

	int i,j;

	//�Ƚ��������Ϊ0�ĵ���ջ
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
		//��ջһ���㣬��������
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
	AOE����ؼ�·��
	����һ�������޻�ͼ������һ�����̡�
	ͨ��ÿ������ֻ��һ�����Ϊ0�ĵ㣬��ΪԴ�㣻ֻ��һ������Ϊ0�ĵ㣬��Ϊ���

	��ͼ�ж�����Ϊ0�ĵ㣬���½�һ���㣬��������һ������ָ���������Ϊ0�ĵ㣬��������Ϊ�µ�Դ��
	��ͼ�ɶ������Ϊ0�ĵ㣬ͬ������½�һ�����

	ͨ������ͼ�Ķ����Ϊ�¼����ѱ߳�Ϊ���

	����������λ��ӵı�e1��e2����e1Ϊe2��ǰ�����e2��e1�ĺ�̻

	�ߵ�����Ϊ��Ĵ����¼����յ��Ϊ��Ľ����¼�

	һ���¼�������ǰ�����һ�������������ߣ����ʱ�����ܴ�����





*/