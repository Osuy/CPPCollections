#pragma once
#include <vector>
#include <stack>
#include <algorithm>
using std::vector;
using std::stack;
/*
	对于给出的数组，求数组可以画出的最大矩形
	使用单调栈。因为要画出矩形，所以对元素i，其后面元素若更小，则矩形被限制，若更大，矩形不被限制
	因此要求单调栈为单调增大
	外循环：逐个将元素入栈，保持栈内元素是单调增大的。如果新元素比栈顶小，则需要出栈
	内循环：出栈，直到栈顶比新元素小。
		   每出栈一个元素，就说明该元素所能画出的矩形被新元素截断，于是可以计算矩形
		   但是还要判断出栈后的栈是否为空，如果栈是空的，要么出栈的是第一个元素，要么数组前面部分最小的数
		   两种情况，矩形都会延伸到数组的首部，因此，矩形宽为i，高为出栈元素
		   如果栈非空，说明不能往前延伸，宽为i-出栈的索引
		内循环每计算一个矩形，就和已记录的最大矩形比较，并取较大者。

	当外循环结束时，栈不为空（至少有一个），里面保留了数组的尾部的递增部分。
	于是还需要一个循环，出栈并计算矩形，操作和上面的一样

	单调栈的应用
	但细节方面，需要好好领会矩形的计算。尤其是计算空栈时的矩形
*/

int GetMaxRetangleOfArray(const vector<int>& arr)
{
	int len = arr.size();
	int res = 0;
	int height, weight;
	stack<int> st;


	for (int i = 0; i < len; i++)
	{
		while (!st.empty() && arr[st.top()] > arr[i])
		{
			height = arr[st.top()];
			weight = i;
			if (!st.empty())//如果栈不为空，则宽不能向前延伸
			{
				weight = i - st.top() - 1;
			}

			res = max(res, height * weight);
		}
		st.push(i);
	}

	while (!st.empty())
	{
		height = arr[st.top()];
		weight = len;
		if (!st.empty())//如果栈不为空，则宽不能向前延伸
		{
			weight = len - st.top() - 1;
		}

		res = max(res, height * weight);
	}

	return res;
}