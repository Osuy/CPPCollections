#pragma once

/*
	�����ɾ��о����ԵûҪ�󻥳�ʹ��ĳһ������Դ��
	���ѡ��������ݻ�ļ���

	������n����Ҫʹ�û���ĳ��Դ�ĻS={a1,a2,...,an}
	ÿ���ai��һ����ʼʱ��si�ͽ���ʱ��fi����ʽΪ�뿪������[si,fi)
	����ai�ͻaj�Ļ���䲻�ص�����ai��aj�ɼ���

	��Ҫѡ��һϵ�л�����ݵĻ������Դ��ռ��ʱ�����

	����������ö�̬�滮��⣬��������ͬʱ�־���̰��ѡ������
	����ʹ��̰���㷨���

	���弯��Sij={ak|fi<=sk<fk<=sj}
	ͷ�i��β�j������S
	����Sij������һ����Ŀ�ʼʱ���ͷ��Ľ���ʱ����
	����ʱ���β��Ŀ�ʼʱ����

	��������Sij����am�ǵ�һ�������
	1.am��Sij���Ӽ���
	2.SimΪ�գ�SmjΪ�ǿգ�Smj������Ϊ������
*/

int OptimalSubset[100];

/*
	�ݹ��
	s�����ʼʱ���
	f�������ʱ���
	index����ǰ����ĳ���
	n�������
	���ܣ��ҵ���һ�����ݵĻ���Ͱѻд�룬Ȼ��ݹ�����һ���
*/
int RecursiveActivitySelector(int* s, int* f, int index, int n)
{
	int m = index + 1;//m��ʾ��һ���
	static int activity_num = 0;

	//m������n����m�Ŀ�ʼʱ��С�ڵ�ǰ��Ľ���ʱ�䣬˵��m�����ݣ�������
	while (m <= n && s[m] <= f[index])
		m++;

	//ѭ���˳����ҵ���һ�����ݵ�m������Խ��
	if (m <= n)//��ûԽ��
	{
		OptimalSubset[activity_num++] = m;
		RecursiveActivitySelector(s, f, m, n);
	}
}

/*
	�ǵݹ��
	�ҵ�һ�����ݻm����д�룬Ȼ��indexָ��m��m��������
*/
int GreedyActivitySelector(int* s, int* f, int n)
{
	int activity_num = 0;
	OptimalSubset[activity_num++] = 1;
	int index = 1;
	int m;

	for (m = 2; m <= n; ++m)
	{
		if (s[m] >= f[index])
		{
			OptimalSubset[activity_num++] = m;;
			index = m;
		}
	}
	return activity_num;
}
