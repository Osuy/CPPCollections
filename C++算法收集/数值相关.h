//��һ������1�ĸ���
int cnt1OfNum(int num) 
{
	int count=0;
	while (num > 0) {
		num = num & (num - 1);//�����λ��1��Ϊ0�������ٸ�1��ѭ�����ٴΣ�ֱ��num=0
		++count;
	}
	return count;
}

//��һ������1�ĸ���,O(1)��
int cnt1OfNum_2(int num) {
	register int n = num;
	int tmp = n - ((n >> 1) & 033333333333) - ((n >> 2) & 011111111111);
	return((tmp + (tmp >> 3)) & 030707070707) % 63;
}

