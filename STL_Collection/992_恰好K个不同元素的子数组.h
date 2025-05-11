#include<vector>
using std::vector;
/*
	����һ�����������������K
	���������飨��ԭ�����ͷβɾȥ0�����ϵ�Ԫ�أ�����ǡ����K����ͬ��Ԫ��
	������������������ڵĸ���

	�����ǡ�ð���K����ͬԪ�ص�������
	�����������K����ͬԪ�أ��ǳ��ý�
	����ת��һ��
	ǡ��K����ͬԪ�� = ���K����ͬԪ�� - ���(K-1)����ͬԪ��
	�����ĿҲ���Ǻܼ�

	�ǳ���Ҫ�ĵ��ǣ���������K��Ԫ�ص������飬�����ҵ�����K��Ԫ�ص������飬��������������ϵĸ���
	���ǣ��������������������ϣ����ܻ�����һ���������������ظ�����
	�����Ҫ�̶�ס�ұ߽硣��Ϊ��һ��������ұ߽��Ȼ�ǲ�ͬ�ġ�
	��ô����Ҫ��������ұ߽�Ϊ����ĩ�˵�������ĸ�������Ȼ��Ŀ=right-left
*/



int subArrayWithMaxKDistinct(int* arr, int len,int K)
{
	//˫ָ�룬����ҿ���rightָ����һ��δ������Ԫ��
	int left = 0;
	int right = 0;
	//��¼��ͬԪ�صĸ���
	int count = 0;
	//��¼Ԫ�س��ֵ�Ƶ��
	vector<int> freq(len, 0);
	//��¼������ĸ���
	int res = 0;

	while (left < right)
	{
		if (freq[arr[right]] == 0)
		{
			count++;
		}
		freq[arr[right]]++;
		right++;

		//��count <= Kʱ�˳���������������K��Ԫ�ص�������
		while (count > K)
		{
			freq[arr[left]]--;
			if (freq[arr[left]] == 0)
			{
				count--;
			}
			left++;
		}

		//��ʱleft��right��Ȧ�����������K����ͬԪ��
		//��ô������ĸ������������
		//�����������������ϣ�������һ������
		//���ܻ��ظ������ֻ����̶���߽��������
		//������֮��
		res += right - left;
	}
	return res;
}
int subArrayWithKDistinct(int* arr, int len, int K)
{
	return subArrayWithMaxKDistinct(arr, len, K) - subArrayWithMaxKDistinct(arr, len, K - 1);
}