#pragma once
#include <stack>

/*
	K��������
	��m*n�������Ϸ���k��������ʹ�����Ż��಻����
	������з������
*/

int n, m, k, ans;
int hash[5][5];

//tot��ʾ���ù����ĸ�������0��ʼ
void work(int x, int y, int tot)
{
	int i, j;
	if (tot == k)//�Ѿ�������k����ans����Ȼ��return
	{
		ans++;
		return;
	}

	do {
		while (hash[x][y])//�ҵ�һ��λ��(x,y)����ֵΪ0��˵���ɷ���
		{
			y++;
			if (y == m)
			{
				x++;
				y = 0;
			}
			if (x == n)return;
		}

		for (i = x - 1; i <= x + 1; ++i)//����λ�þŹ����ڵ�ֵ��1
		{
			if (i >= 0 && i < n)
				for (j = y - 1; j <= y + 1; ++j)
					if (j >= 0 && j < m)
						hash[i][j]++;
		}

		work(x, y, tot + 1);//�ݹ飬������һ������

		for (i = x - 1; i <= x + 1; ++i)//����λ�þŹ����ڵ�ֵ��1���ظ���δ����״̬
		{
			if (i >= 0 && i < n)
				for (j = y - 1; j <= y + 1; ++j)
					if (j >= 0 && j < m)
						hash[i][j]--;
		}

		//��λ����һ��λ�ã��������������return
		y++;
		if (y == m)
		{
			x++; y = 0;
		}
		if (x == n)
			return;
	} while (1);
}

//ʹ��ջ��ѭ����ʵ��k����

struct Point
{
	int x, y;

	Point() :x(0), y(-1) {}//Ĭ�Ϲ��캯������������ĳ�ʼ��

	Point(int _x, int _y) :x(_x), y(_y) {};

	bool operator==(const Point& other)
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(const Point& other)
	{
		return x != other.x || y != other.y;
	}
};

//panel����ά���飬���ֵ��ʾ�����Ĺ�����Χ���ɵ���
//n�����������
//m�����������
class Plus
{
public:
	template<class T>
	void operator()(T& val) { val++; }
};

class Minus
{
public:
	template<class T>
	void operator()(T& val) { val--; }
};

//panel��n��m����ά���������������
//p����
//op������
//���ܣ��ڶ�ά�����У��Ե�pΪ���ĵľŹ����ֵ����op�������Լӻ��Լ�
template<class Op>
void ModiyRange(int** panel, int n, int m, Point p, Op&& op)
{
	for (int i = p.x - 1; i <= p.x + 1; ++i)
		for (int j = p.y - 1; j <= p.j + 1; ++j)
			if (i >= 0 && i < n && j >= 0 && j < m)
				op(panel[i][j]);
}

//panel��n��m����ά���������������
//p����
//���ܣ�Ѱ�ҵ�p����һ���ɷ��õ㣬������
Point FindNextValidPoint(int** panel, int n, int m, Point p)
{
	for (int i = p.x; i < n + 1; ++i)
		for (int j = p.y + 1; j < m + 1; ++j)
			if (panel[i][j] == 0)
				return Point(i, j);
	return Point(0, -1);
}

int KKing(int** panel, int n, int m,int k)
{
	Point st[20];
	int top = -1;
	bool find;
	int count;

	//�����һ����(0,-1)
	st[++top] = Point();

	while (top >= 0)
	{
		st[top] = FindNextValidPoint(panel, n, m, st[top]);
		
		if (st[top].y < m)
		{
			ModiyRange(panel, n, m, st[top], Plus());
			if (top >= n)
			{
				count++;
				//���
				ModiyRange(panel, n, m, st[top], Minus());
			}
			else
			{
				top++;
				st[top] = Point();
			}
		}
		else
		{
			top--;
			ModiyRange(panel, n, m, st[top], Minus());	
		}
	}
	return count;
}