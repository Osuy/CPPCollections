#include <string>
using std::string;
/*
	KMP算法

	kmp算法的重点在于获取模式传的next数组。
	而求这个数组的代码实现很反人类。
	首先要理解k和j：
		j负责遍历串，但是0~len-2，因为每次循环判断t[k]==t[j]，而将k值赋给next[++j]，因此要在len-2处停下
		k负责记录j前元素序列与串头序列相同的最大长度。一般而言，k因为要回退，经常停留在头部，或等于-1

	一旦 t[j] != t[k]，就需要k回退。
	回退的理解是：
		某次循环时，t0~tk-1 与 tj-k~tj-1相等
		现在t[j] != t[k]，对于t0~tk-1而言，next[k]记录的值k'表示t0~tk'-1 与 tk-k'~tk-1相等
		那么在tj-k~tj-1的末端就存在一小段与tk-k'~tk-1相等
		于是，那一小段也就与t0~tk'-1，即j前有一小段与串头长度为k'的序列相等
		于是k回退为k'，即k=next[k]

*/

//求Next数组
void GetNext(const string& t, int* next)
{
	int j, k;
	j = 0, k = -1;
	next[j] = -1;

	while (j < t.length())//注意j小于len-1，因为循环体内需要
	{
		if (k == -1 || t[j] == t[k])
		{
			//下标为i，则它前方有i个元素，k记录的是字符串t开头的k个元素与j前k个元素完全相等
			//因此，既然t[j] == t[k]，那么就有k+1个元素相等，于是先自加，在把k赋给next
			++j, ++k;
			next[j] = k;
		}
		else//如果t[j]与t[k]一直不等，k会退回到-1
			k = next[k];
	}
}

void GetNext2(const string& t, int* next)
{
	int i = 0, j = -1;
	next[i] = j;

	while (i < t.length())
	{
		if (j == -1 || t[i] == t[j])
		{
			next[++i] == ++j;
		}
		else
			j = next[j];
	}
}

//KMP算法
int KMPIndex(const string& s, const string& t)
{
	int next[20], i = 0, j = 0;
	GetNext(t, next);

	while (i < s.length() && j < t.length())
	{
		if (j == -1 || s[i] == t[j])
		{
			++i, ++j;
		}
		else
			j = next[j];
	}

	if (j >= t.length())
		return i - t.length();
	else
		return -1;
}

int KMPIndex2(const string& s, const string& t)
{
	int next[20], i = 0, j = 0;
	GetNext(t, next);

	while (i < s.length() && j < t.length())
	{
		if (j == -1 || s[i] == t[j])
			++i, ++j;
		else
			j = next[j];
	}

	if (j >= t.length())
		return i - t.length();
	else
		return -1;

}

//改进的KMP算法
//主要是修改GetNext的缺陷
void GetNext_sec(const string& t, int* next)
{
	int j = 0, k = -1;
	next[j] = k;

	while (j < t.length())
	{
		if (k == -1 || t[j] == t[k])
		{
			++j, ++k;
			if (t[j] != t[k])
				next[j] = k;
			else//此处多了一步：j和k自加后，如果t[j] == t[k]，说明t[j]与s不匹配，那么k也一样，所以直接做一次回退
				next[j] = next[k];
		}
		else
			k = next[k];
	}
}

void GetNext_sec2(const string& t, int* next)
{
	int i = 0, j = -1;
	next[i] = j;

	while (i < t.length())
	{
		if (j == -1 || t[i] == t[j])
		{
			i++, j++;
			if (t[i] != t[j])
				next[i] = j;
			else
				next[i] = next[j];
		}
		else
			j = next[j];
	}
}

//求子串在主串中出现的次数
int ModelCount(const string& s, const string& t)
{
	int count = 0, j = 0, k = 0;
	int next[20];
	GetNext_sec(t, next);

	while (j < s.length())
	{
		if (k == -1 || s[j] == t[k])
		{
			++j, ++k;
		}
		else
			k = next[k];

		if (k == t.length())
		{
			k = 0, j -= t.length();
			++count;
		}
	}

	return count;
}

//求最大重复子串
void MaxRepeatSubString(const string& s, int& index,int& len)
{
	string sub;

	int count = 0;
	len = 0;
	for (int i = 1; i < s.length(); ++i)
	{
		count = 0;
		for (int j = 0; i < s.length(); ++j)
		{
			if (s[i + j] == s[j])
				count++;
			else
			{
				if (count > len)
				{
					len = count;
					index = j - len;
				}
				count = 0;
			}
		}
	}

	if (count > len)
	{
		len = count;
		index = s.length() - len;
	}
}