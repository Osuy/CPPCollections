#include <vector>

//给出target，返回数组中两个相加为target的元素的下标
vector<int> twoSum(vector<int>& nums, int target) {
    unordered_map<int, int> hash;
    vector<int> result;
    int numsSize = int(nums.size());

    for (int i = 0; i < numsSize; i++)
    {
        int numberToFind = target - nums[i];//取当前元素欸但补数

        //如果补数能在hash找到，就把补数所对的元素(下标)传入result，再把i传入result
        if(hash.find(numberToFind) != hash.end())
        {
            result.push_back(hash[numberToFind]);
            result.push_back(i);
            return result;
        }
        //如果不能在hash找到补数，就把这个元素传给hash，以便后续元素寻找
        hash[nums[i]] = i;//以元素值为键值，下标为实值赋给hash
    }
    //找不到就返回空result
    return result;
}

//中位数
double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
	int n = nums1.size();
	int m = nums2.size();

	if (n > m)  //保证数组1一定最短
	{
		return findMedianSortedArrays(nums2, nums1);
	}

	// Ci 为第i个数组的割,比如C1为2时表示第1个数组只有2个元素。LMaxi为第i个数组割后的左元素。RMini为第i个数组割后的右元素。
	int LMax1, LMax2, RMin1, RMin2, c1, c2, lo = 0, hi = 2 * n;  //我们目前是虚拟加了'#'所以数组1是2*n长度

	while (lo <= hi)   //二分
	{
		c1 = (lo + hi) / 2;  //c1是二分的结果
		c2 = m + n - c1;

		LMax1 = (c1 == 0) ? INT_MIN : nums1[(c1 - 1) / 2];
		RMin1 = (c1 == 2 * n) ? INT_MAX : nums1[c1 / 2];
		LMax2 = (c2 == 0) ? INT_MIN : nums2[(c2 - 1) / 2];
		RMin2 = (c2 == 2 * m) ? INT_MAX : nums2[c2 / 2];

		if (LMax1 > RMin2)
			hi = c1 - 1;
		else if (LMax2 > RMin1)
			lo = c1 + 1;
		else
			break;
	}
	return (max(LMax1, LMax2) + min(RMin1, RMin2)) / 2.0;
}

//寻找无重复子串的最大长度
int lengthOfLongestSubstring(string s)
{
    //s[start,end) 前面包含 后面不包含
    int start(0), end(0), length(0), result(0);
    int sSize = int(s.size());
    unordered_map<char, int> hash;
    while (end < sSize)//用end遍历字符串
    {
        char tmpChar = s[end];
        //仅当s[start,end) 中存在s[end]时更新start
        if (hash.find(tmpChar) != hash.end() && hash[tmpChar] >= start)//在哈希表中能找到，说明重复
        {
            start = hash[tmpChar] + 1;
            length = end - start;
        }
        hash[tmpChar] = end;

        end++;
        length++;
        result = max(result, length);
    }
    return result;
}