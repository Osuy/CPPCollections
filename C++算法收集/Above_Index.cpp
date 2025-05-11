#include <vector>

//����target�������������������Ϊtarget��Ԫ�ص��±�
vector<int> twoSum(vector<int>& nums, int target) {
    unordered_map<int, int> hash;
    vector<int> result;
    int numsSize = int(nums.size());

    for (int i = 0; i < numsSize; i++)
    {
        int numberToFind = target - nums[i];//ȡ��ǰԪ�ؚG������

        //�����������hash�ҵ����ͰѲ������Ե�Ԫ��(�±�)����result���ٰ�i����result
        if(hash.find(numberToFind) != hash.end())
        {
            result.push_back(hash[numberToFind]);
            result.push_back(i);
            return result;
        }
        //���������hash�ҵ��������Ͱ����Ԫ�ش���hash���Ա����Ԫ��Ѱ��
        hash[nums[i]] = i;//��Ԫ��ֵΪ��ֵ���±�Ϊʵֵ����hash
    }
    //�Ҳ����ͷ��ؿ�result
    return result;
}

//��λ��
double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
	int n = nums1.size();
	int m = nums2.size();

	if (n > m)  //��֤����1һ�����
	{
		return findMedianSortedArrays(nums2, nums1);
	}

	// Ci Ϊ��i������ĸ�,����C1Ϊ2ʱ��ʾ��1������ֻ��2��Ԫ�ء�LMaxiΪ��i�����������Ԫ�ء�RMiniΪ��i�����������Ԫ�ء�
	int LMax1, LMax2, RMin1, RMin2, c1, c2, lo = 0, hi = 2 * n;  //����Ŀǰ���������'#'��������1��2*n����

	while (lo <= hi)   //����
	{
		c1 = (lo + hi) / 2;  //c1�Ƕ��ֵĽ��
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

//Ѱ�����ظ��Ӵ�����󳤶�
int lengthOfLongestSubstring(string s)
{
    //s[start,end) ǰ����� ���治����
    int start(0), end(0), length(0), result(0);
    int sSize = int(s.size());
    unordered_map<char, int> hash;
    while (end < sSize)//��end�����ַ���
    {
        char tmpChar = s[end];
        //����s[start,end) �д���s[end]ʱ����start
        if (hash.find(tmpChar) != hash.end() && hash[tmpChar] >= start)//�ڹ�ϣ�������ҵ���˵���ظ�
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