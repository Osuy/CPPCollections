#pragma once

/*
	��n�ֲ�ͬ��ֵ����Ʊ��һ�����������m��
	�����Ʊ����ֵ��
	ʹ������һ�����ϣ��ճ�������{1��2��3��...��max}
	����max�����ܴ�
*/

int n, m;
int maxStamp;
int r;//ǰcur����Ʊ��֧�����������
const int MM = 500;
const int MN = 50;
int x[MN];//�������У������Ʊ��ֵ
int y[MM];//y[j]=k����ʾ����Ϊj����С��Ʊ����Ϊk
int ans[MN];
const int INF = 1000000;//��ʾ���ɴ�
void backtrace(int cur)
{
	//cur=n��˵������0~cur-1����n����Ʊ��
	//��r���ڵ�ǰ�������ֵ��
	//���浽ans�����ҪѰ�ң�return����һ���ݹ�
	if (cur >= n && r > maxStamp)
	{
		maxStamp = r;
		for (int i = 0; i < n; ++i)
			ans[i] = x[i];

		return;
	}

	//����r��y
	int backup_r = r;
	int backup_y[MM];
	for (int i = 0; i < MM; i++)
	{
		backup_y[i] = y[i];
	}
	
	//����ֵ����Ϊǰһ����Ʊ��ֵ+1����������
	//����Ϊr+1����Ϊ
	//ǰn-1����ֵm������r���ղ���r+1
	//����ֵ�������r+1����ô��r+1��Ȼ��ǰn-1��ȥ�գ����Ǵղ���
	//��������ֵ���r+1
	for (int i = x[cur - 1] + 1; i <= r + 1; ++i)
	{
		x[cur] = i;//ѭ��ʹx[cur]ȡÿһ��i��Ȼ��ʵ���Ƿ�������
		//k��0��ǰһ����Ʊ��ֵ*(m-1)
		//��ʾ������������ֵ��
		for (int k = 0; k <= x[cur - 1] * (m - 1); ++k)
		{
			if (y[k] >= m)continue;//��������������m��˵���ճ�k����ϴﵽn�ţ������ټ�
			//����ճ�k����ϲ���n�ţ��ͱ������������1~m-y[k]
			for (int num = 1; num <= m - y[k]; ++num)
			{
				if (k + x[cur] * num < MN)//����num������ֵû����
				{
					//�򽫶�Ӧ��ֵ�͵���������
					y[k + x[cur] * num] = min(y[k + x[cur] * num], y[k] + num);
				}
			}
		}

		//���������ѭ���󣬲���y�����ֵ����
		//�ҵ���һ������������ֵ�ͣ���Ϊr
		//˵����ǰn-1����ֵ������ֵi���������ܴճ�r
		while (y[r + 1] < INF)r++;

		//Ȼ��ݹ���һ����ֵ
		backtrace(cur + 1);

		//�ָ�r��y
		r = backup_r;
		for (int j = 0; j < MM; j++)y[j] = backup_y[j];
	}
	free(backup_y);
}

void backtrace2(int cur)
{
	if (cur >= m && r > maxStamp)
	{
		maxStamp = r;
		for (int i = 0; i < cur; ++i)
			ans[i] = x[i];
	}

	int backup_r = r;
	int backup_y[MN];
	for (int i = 0; i < MN; ++i)
		backup_y[i] = y[i];

	for (int i = x[cur - 1] + 1; i <= r + 1; ++i)
	{
		x[cur] = i;
		for (int k = 0; k < x[cur - 1] * (m - 1); k++)
		{
			for (int num = 1; num < m - y[k]; num++)
			{
				if (k + num * i < MM)
					y[k + num * i] = min(y[k] + num, y[k + num * i]);
			}

		}

		while (y[r + 1] < INF)r++;

		backtrace2(cur + 1);

		r = backup_r;
		for (int j = 0; j < MN; ++j)
			y[j] = backup_y[j];
	}

	free(backup_y);
}

/*
	������n�ʺ���񣬸Ľ�����ջ��ѭ���Ľ����ʽ
	������stamp[n]��ʾ��Ʊ����ֵ���ǵ�������
	acc[]
*/

