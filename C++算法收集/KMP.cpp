#include <string.h>
#include <string>


void Getnext(int next[], String t)
{
    int j = 0, k = -1;
    next[0] = -1;
    while (j < t.length - 1)//j指针遍历字符串，
    {
        //三种情况：
        //只有k=-1，说明字符串不重复，k++后为0，next[j]=0，说明重复长度为0。在kmp中匹配失败，模式字符串就退回0的位置
        //只有t[j]=t[k]，说明字符串重复，于是k和j都向前，记录这两段重复的字符串。next[j]=k，如果kmp匹配失败就退回到k的位置，不必回到0
        //k == -1 且 t[j] == t[k]，说明字符串第一个第二元素相同，下一循环中，j和k都会继续向前
        if (k == -1 || t[j] == t[k])
        {
            j++; k++;
            next[j] = k;//记录重复前缀的位置，用于退回
        }
        //如果k != -1且t[j] != t[k]，说明重复子串结束，或者k=0且没有重复子串
        //重复子串结束，把k退回到前缀的位置
        //没有重复子串且k=0，把k取next[0]=-1，方便下一循环，j前进，k来回于[-1,0]之间
        else k = next[k];
    }
}

int KMP(String s, String t)
{
    int next[MaxSize], i = 0; j = 0;
    Getnext(t, next);//获取了模式字符串t的重复前缀位置，用于退回
    while (i < s.length && j < t.length)
    {
        //如果匹配，都向前；不匹配，i不动，j退回到next[j]，也就是重复前缀的位置
        //特别的，如果j=-1，说明模式字符串的第一个元素不匹配，j退到next[0]=-1
        //s[i] == t[j]，说明当前匹配，i和j都向前
        if (j == -1 || s[i] == t[j])
        {
            i++;
            j++;
        }
        else j = next[j];//j回退到重复前缀的位置
    }
    if (j >= t.length)//匹配成功，返回子串的位置
        return (i - t.length);         
    else
        return (-1);//没找到
}