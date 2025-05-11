#pragma once
#include "Stack and Queue.h"
#include <stdlib.h>
#include <iostream>
using std::cout;
using std::endl;


/*
    ��׺���ʽ����׺���ʽ��ת��
*/



/*
    n�ʺ��㷨
    ����n��������n*n�������Ϸ���n��������ԵĻʺ�Ľⷨ
    �յ����ڻ��ݺ��ж�
    �ж��Ƿ�һ��λ��(i,j)�Ƿ���Է��ã�����i��֮ǰ�Ļʺ��Ƿ�yֵΪj��������(i,j)�ĺ��������
    
    ����������ѭ��j��ȡ��ֵ����0������һ�е�yֵ+1��st.col[st.top + 1] + 1��
    ������������ˣ��Ϳ��Զ�����һ�е�ֵ��������ѭ��

    �������Ϊn-1����������һ���Ҳ�������λ�ã����ڱ�����Ҫ���£�����Ѱ���µĿ���λ��
        ����ҵ������±��У�������һ�е�yֵ����Ϊ-1����һ��ѭ������һ��
        ����Ҳ�������Ҫ�˻ص���һ�У��������е�yֵ����һ��ѭ����ȡ��������һ������һ���ҿ���λ�ò��ش�-1����
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

//��д
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
        find = false;       //��find����
        if (st.top == n - 1)//�ҵ��⣬���
        {
            cout << "��" << ++count << "�ֽ⣺";
            for (int i = 0; i < n; i++)
            {
                cout << "(" << i << "," << st.col[i] << ")  ";
            }
            cout << endl;
        }
        else//δ�ҵ��⣬����һ����
        {
            //j = st.col[st.top + 1] + 1��ԭ���ǣ������һ����δȥ�������У���j=0��
            //�����Ǿ����˻��ݣ�������ǰ��ֵ�����¼���Ѱ���µĿ���λ��
            for (int j = st.col[st.top + 1] + 1; j < n; ++j)
            {
                if (Judge(st, st.top + 1, j))//���j���ã������st�֣���find=true���˳�ѭ��
                {
                    ++st.top;
                    st.col[st.top] = j;
                    find = true;
                    break;
                }
            }
        }
        //��������һ���⣬����δ������һ���ҵ�����λ��
        //���ڵ�ǰ��Ѱ����һ������λ�ã�����ҵ��ˣ��ͽ���һ�б����ֵ����δ-1������Ѱ���µĽ�
        if (!find)
        {
            int m = st.col[st.top] + 1;
            while (m < n)
            {
                if (Judge(st, st.top, m))
                {
                    st.col[st.top] = m;
                    st.col[st.top + 1] = -1;//�ڴ����ҵ���һ���µĿ���λ�ã�����һ�е�ֵ����
                    break;
                }
                ++m;
            }

            if (m >= n)//�������û�п���λ�ã����ݵ���һ��
            {
                //st.col[st.top] = -1;
                --st.top;//����
            }
        }
    }
}


//���ַ�ջ��������ʹ��СԪ����ջ�������ʹ��һ������ջ
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

