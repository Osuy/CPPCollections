#pragma once
#include <iostream>

/*
	��һ���߳�Ϊ2^n�����������̣�����һ�񲻿�ռ�ã�
	���ಿ����L��ֽƬ���ص�����ȫ��������
	ÿ��ֽƬ�ı��Ψһ
*/

int board[65][65], tile;

/*
* �ؼ���ʹ�÷��η�
* �����̵ȷ�Ϊ4�ݣ��ص��Ȼ��������һ��
* �������־ͽ����Ľǵ�ֵ��Ϊt������Ϊ�ص�����ݹ�
* ÿ�ε��ã�����ʹ��������������������һ��L��ֽƬ
*/
void chessboard(int tr, int tc, int dr, int dc, int size)
{
	if (size <= 1)return;
	int s, t;

	t = tile++; //����L��ֽƬ���Ψһ�����ÿ�ν��и��ǣ�����Ҫʹtile����
	s = size / 2;

	//��size*size�����̷�Ϊ4�֣�����ÿһ��
	//�ص�ش�������һ�ݣ��������ֽ����Ľ����ֵ��Ϊt��������L��

	if (dr < tr + s && dc < tc + s)//���ص������ϣ��ݹ鴦��
	{
		chessboard(tr, tc, dr, dc, s);
	}
	else
	{
		board[tr + s - 1][tc + s - 1] = t;//�����ֵ����½�ֵ��Ϊt
		chessboard(tr, tc, tr + s - 1, tc + s - 1, s);//��������Ϊ�ص㾵��ݹ�
	}

	if (dr < tr + s && dc >= tc + s)//���ص������ϣ��ݹ�
	{
		chessboard(tr, tc + s, dr, dc, s);
	}
	else
	{
		board[tr + s - 1][tc + s] = t;
		chessboard(tr, tc + s, tr + s - 1, tc + s, s);
	}

	if (dr >= tr + s && dc < tc + s)//���ص������£��ݹ�
	{
		chessboard(tr + s, tc, dr, dc, s);
	}
	else
	{
		board[tr + s][tc + s - 1] = t;
		chessboard(tr + s, tc, tr + s, tc + s - 1, s);
	}

	if (dr >= tr + s && dc >= tc + s)//���ص������£��ݹ�
	{
		chessboard(tr + s, tc + s, dr, dc, s);
	}
	else
	{
		board[tr + s][tc + s] = t;
		chessboard(tr + s, tc + s, tr + s, tc + s, s);
	}
}