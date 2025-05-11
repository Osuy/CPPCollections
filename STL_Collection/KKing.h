#pragma once
#include <stack>

/*
	K国王问题
	在m*n的棋盘上放置k个国王，使国王门互相不攻击
	输出所有放置情况
*/

int n, m, k, ans;
int hash[5][5];

//tot表示放置国王的个数，从0开始
void work(int x, int y, int tot)
{
	int i, j;
	if (tot == k)//已经放置了k个，ans自增然后return
	{
		ans++;
		return;
	}

	do {
		while (hash[x][y])//找到一个位置(x,y)，其值为0，说明可放置
		{
			y++;
			if (y == m)
			{
				x++;
				y = 0;
			}
			if (x == n)return;
		}

		for (i = x - 1; i <= x + 1; ++i)//将该位置九宫格内的值加1
		{
			if (i >= 0 && i < n)
				for (j = y - 1; j <= y + 1; ++j)
					if (j >= 0 && j < m)
						hash[i][j]++;
		}

		work(x, y, tot + 1);//递归，放置下一个国王

		for (i = x - 1; i <= x + 1; ++i)//将该位置九宫格内的值减1，回复成未放置状态
		{
			if (i >= 0 && i < n)
				for (j = y - 1; j <= y + 1; ++j)
					if (j >= 0 && j < m)
						hash[i][j]--;
		}

		//定位到下一个位置，如果超出棋盘则return
		y++;
		if (y == m)
		{
			x++; y = 0;
		}
		if (x == n)
			return;
	} while (1);
}

//使用栈和循环来实现k国王

struct Point
{
	int x, y;

	Point() :x(0), y(-1) {}//默认构造函数，用于数组的初始化

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

//panel：二维数组，存放值表示国王的攻击范围，可叠加
//n：数组的行数
//m：数组的列数
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

//panel、n、m：二维数组和它的行列数
//p：点
//op：操作
//功能：在二维数组中，以点p为中心的九宫格的值进行op操作，自加或自减
template<class Op>
void ModiyRange(int** panel, int n, int m, Point p, Op&& op)
{
	for (int i = p.x - 1; i <= p.x + 1; ++i)
		for (int j = p.y - 1; j <= p.j + 1; ++j)
			if (i >= 0 && i < n && j >= 0 && j < m)
				op(panel[i][j]);
}

//panel、n、m：二维数组和它的行列数
//p：点
//功能：寻找点p的下一个可放置点，并返回
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

	//加入第一个点(0,-1)
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
				//输出
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