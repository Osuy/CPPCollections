#include <string>
using std::string;
/*
	KMP�㷨

	kmp�㷨���ص����ڻ�ȡģʽ����next���顣
	�����������Ĵ���ʵ�ֺܷ����ࡣ
	����Ҫ���k��j��
		j���������������0~len-2����Ϊÿ��ѭ���ж�t[k]==t[j]������kֵ����next[++j]�����Ҫ��len-2��ͣ��
		k�����¼jǰԪ�������봮ͷ������ͬ����󳤶ȡ�һ����ԣ�k��ΪҪ���ˣ�����ͣ����ͷ���������-1

	һ�� t[j] != t[k]������Ҫk���ˡ�
	���˵�����ǣ�
		ĳ��ѭ��ʱ��t0~tk-1 �� tj-k~tj-1���
		����t[j] != t[k]������t0~tk-1���ԣ�next[k]��¼��ֵk'��ʾt0~tk'-1 �� tk-k'~tk-1���
		��ô��tj-k~tj-1��ĩ�˾ʹ���һС����tk-k'~tk-1���
		���ǣ���һС��Ҳ����t0~tk'-1����jǰ��һС���봮ͷ����Ϊk'���������
		����k����Ϊk'����k=next[k]

*/

//��Next����
void GetNext(const string& t, int* next)
{
	int j, k;
	j = 0, k = -1;
	next[j] = -1;

	while (j < t.length())//ע��jС��len-1����Ϊѭ��������Ҫ
	{
		if (k == -1 || t[j] == t[k])
		{
			//�±�Ϊi������ǰ����i��Ԫ�أ�k��¼�����ַ���t��ͷ��k��Ԫ����jǰk��Ԫ����ȫ���
			//��ˣ���Ȼt[j] == t[k]����ô����k+1��Ԫ����ȣ��������Լӣ��ڰ�k����next
			++j, ++k;
			next[j] = k;
		}
		else//���t[j]��t[k]һֱ���ȣ�k���˻ص�-1
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

//KMP�㷨
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

//�Ľ���KMP�㷨
//��Ҫ���޸�GetNext��ȱ��
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
			else//�˴�����һ����j��k�ԼӺ����t[j] == t[k]��˵��t[j]��s��ƥ�䣬��ôkҲһ��������ֱ����һ�λ���
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

//���Ӵ��������г��ֵĴ���
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

//������ظ��Ӵ�
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