#pragma once
#include "Stack and Queue.h"
#include <stdlib.h>
#include <iostream>
using std::cout;
using std::endl;


/*
    中缀表达式到后缀表达式的转换
*/



/*
    n皇后算法
    输入n，计算在n*n的棋盘上放置n个互不相吃的皇后的解法
    终点在于回溯和判断
    判断是否一个位置(i,j)是否可以放置，即：i列之前的皇后是否y值为j，或者与(i,j)的横竖差相等
    
    回溯依赖内循环j读取的值不是0而是下一列的y值+1（st.col[st.top + 1] + 1）
    这样，如果回退，就可以读到上一行的值而不会死循环

    如果本列为n-1，或者在下一列找不到可用位置，则在本列需要更新，往下寻找新的可用位置
        如果找到，更新本列，并把下一列的y值重置为-1，下一个循环找下一列
        如果找不到，需要退回到上一列，保留本列的y值供下一个循环读取。这样上一列在下一列找可用位置不必从-1找起
*/

bool Judge(const StType& st, int i, int j)
{
    for (int k = 0; k < i; ++k)
    {
        if (st.col[k] == j || abs(st.col[k] - j) == abs(k - i))
        {
            return false;
        }
    }

    return true;
}

bool Judge(int* st, int i, int j)
{
    for (int k = 0; k < i; ++k)
    {
        if (st[k] == j || abs(st[k] - j) == abs(k - i))
        {
            return false;
        }
    }
    return true;
}

//抄写
void Queen(int n)
{
    int st[20];
    int top = -1;
    int count = 0;
    bool find;

    for (int i = 0; i < n; ++i)
        st[i] = -1;

    st[++top] = 0;

    while (top >= 0)
    {
        find = false;
        if (top == n - 1)
        {
            ++count;
            //cout<<
        }
        else
        {
            for (int i = st[top + 1] + 1; i < n; ++i)
            {
                if (Judge(st, top + 1, i))
                {
                    st[++top] = i;
                    find = true;
                    break;
                }
            }
        }

        if (!find)
        {
            int m = st[top] + 1;
            while (m < n)
            {
                if (Judge(st, top, m))
                {
                    st[top] = m;
                    st[top + 1] = -1;
                    break;
                }
                ++m;
            }

            if (m >= n)
            {
                top--;
            }
        }
    }
}

void Queen2(int n)
{
    StType st;
    int count = 0;
    st.col[0] = 0;
    st.top = 0;
    bool find = false;
    while (st.top >= 0)
    {
        find = false;       //将find重置
        if (st.top == n - 1)//找到解，输出
        {
            cout << "第" << ++count << "种解：";
            for (int i = 0; i < n; i++)
            {
                cout << "(" << i << "," << st.col[i] << ")  ";
            }
            cout << endl;
        }
        else//未找到解，向下一列找
        {
            //j = st.col[st.top + 1] + 1的原因是：如果下一列是未去过的新列，则j=0；
            //否则，是经过了回溯，读出以前的值，往下继续寻找新的可用位置
            for (int j = st.col[st.top + 1] + 1; j < n; ++j)
            {
                if (Judge(st, st.top + 1, j))//如果j可用，则存入st种，令find=true，退出循环
                {
                    ++st.top;
                    st.col[st.top] = j;
                    find = true;
                    break;
                }
            }
        }
        //如果输出了一个解，或者未能在下一行找到可用位置
        //就在当前列寻找下一个可用位置，如果找到了，就将下一行保存的值重置未-1，方便寻找新的解
        if (!find)
        {
            int m = st.col[st.top] + 1;
            while (m < n)
            {
                if (Judge(st, st.top, m))
                {
                    st.col[st.top] = m;
                    st.col[st.top + 1] = -1;//在此列找到了一个新的可用位置，将下一列的值重置
                    break;
                }
                ++m;
            }

            if (m >= n)//如果此列没有可用位置，回溯到上一列
            {
                //st.col[st.top] = -1;
                --st.top;//回溯
            }
        }
    }
}


//对字符栈进行排序，使最小元素在栈顶，最多使用一个额外栈
void StackSort(char* st, int top)
{
    char st2[20];
    int top2 = -1;
    char tmp;

    int count;

    while (top >= 0)
    {
        count = 0;
        tmp = st[top--];

        while (st2[top2] > tmp && top2 >= 0)
        {
            ++count;
            st[++top] = st2[top2--];

        }

        st2[++top] = tmp;

        while (count > 0)
        {
            --count;
            st2[++top2] = st[top--];
        }

    }

    while (top2 >= 0)
    {
        st[++top] = st2[top2--];
    }
}

