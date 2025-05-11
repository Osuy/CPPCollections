#pragma once
#include <vector>
using std::vector;
/*
	对于0，1，2三种代表不同颜色的数值
	给出一个包含三种颜色的数组，将其排序，使0在1前面，1在2前面
	相同颜色的数值相邻

	暴力解法，第一次遍历，将全部的0放在数组前面，
	第二次遍历将全部的1放在剩余区间的前面


	双指针（滑动窗口）
	p0指向0区间的下一个位置。每次找到0，就交换到p0的位置
	p1指向1区间的下一个位置。每次找到1，就交换到p1的位置
	p0和p1开始都位于0。因为所有0都在1前面
	因此，p0不会超过p1，其中，0~p0为0所在的区间，p0~p1为1所在的区间。
	当交换0的时候，会把1区间的第一个1换到后面，这时就要把1再换到p1的位置，
	然后p0和p1都自增

	而如果找到1，直接交换到p1的位置即可
	
*/

void SortColor(vector<int>colors)
{
	//偷懒，下面这一条语句也完全正确
	//sort(colors.begin(),colors.end());

	int p0 = 0;
	int p1 = 0;

	for (int i = 0; i < colors.size(); ++i)
	{
		if (colors[i] == 1)
		{
			swap(colors[i], colors[p1]);
			p1++;
		}
		else if (colors[i] == 0)
		{
			swap(colors[i], colors[p0]);
			if (p0 < p1)
				swap(colors[i], colors[p1]);

			p0++;
			p1++;
		}
	}
}