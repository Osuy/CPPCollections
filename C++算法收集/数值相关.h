//求一个数中1的个数
int cnt1OfNum(int num) 
{
	int count=0;
	while (num > 0) {
		num = num & (num - 1);//将最低位的1变为0，多有少个1就循环多少次，直到num=0
		++count;
	}
	return count;
}

//求一个数中1的个数,O(1)版
int cnt1OfNum_2(int num) {
	register int n = num;
	int tmp = n - ((n >> 1) & 033333333333) - ((n >> 2) & 011111111111);
	return((tmp + (tmp >> 3)) & 030707070707) % 63;
}

