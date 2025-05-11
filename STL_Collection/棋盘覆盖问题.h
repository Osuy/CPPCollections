#pragma once
#include <iostream>

/*
	对一个边长为2^n的正方形棋盘，仅有一格不可占用，
	其余部分用L形纸片无重叠地完全覆盖起来
	每个纸片的编号唯一
*/

int board[65][65], tile;

/*
* 关键是使用分治法
* 将棋盘等分为4份，特点必然处于其中一份
* 其余三分就将中心角的值设为t，并作为特点继续递归
* 每次调用，都会使传进来的区域中心生成一个L形纸片
*/
void chessboard(int tr, int tc, int dr, int dc, int size)
{
	if (size <= 1)return;
	int s, t;

	t = tile++; //由于L形纸片编号唯一，因此每次进行覆盖，都需要使tile自增
	s = size / 2;

	//将size*size的棋盘分为4分，处理每一份
	//特点必处于其中一份，其余三分将中心角落的值设为t，正好是L形

	if (dr < tr + s && dc < tc + s)//若特点在左上，递归处理
	{
		chessboard(tr, tc, dr, dc, s);
	}
	else
	{
		board[tr + s - 1][tc + s - 1] = t;//将左手的右下角值设为t
		chessboard(tr, tc, tr + s - 1, tc + s - 1, s);//并将其作为特点镜像递归
	}

	if (dr < tr + s && dc >= tc + s)//若特点在右上，递归
	{
		chessboard(tr, tc + s, dr, dc, s);
	}
	else
	{
		board[tr + s - 1][tc + s] = t;
		chessboard(tr, tc + s, tr + s - 1, tc + s, s);
	}

	if (dr >= tr + s && dc < tc + s)//若特点在左下，递归
	{
		chessboard(tr + s, tc, dr, dc, s);
	}
	else
	{
		board[tr + s][tc + s - 1] = t;
		chessboard(tr + s, tc, tr + s, tc + s - 1, s);
	}

	if (dr >= tr + s && dc >= tc + s)//若特点在右下，递归
	{
		chessboard(tr + s, tc + s, dr, dc, s);
	}
	else
	{
		board[tr + s][tc + s] = t;
		chessboard(tr + s, tc + s, tr + s, tc + s, s);
	}
}