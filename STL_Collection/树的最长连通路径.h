#pragma once
#include <algorithm>
#include <stdlib.h>
using std::max;
/*
	�����������������ڵ㣬����֮���Ȼ����һ��·��
	·���ϵĽڵ��Ȩֵ��Ϊ·���ĳ���
	���������·������

    ���ȣ������ڵ����ͨ·����Ȼ�������ǵĹ�ͬ���ȡ�
    ���϶���ȥ�����ݹ鷽ʽ���

        ����һ���ڵ㣬���Ϊ�գ�����0
        �����Ϊ�գ�
            �õ����������������֦����0ȡ�ϴ��ߣ�����l
            �õ����������������ֵ����0ȡ�ϴ��ߣ�����r

            �����ֵ+l+r>maxlength ��maxlengthΪ���ò�����
            ���޸�maxlength

        Ȼ�󷵻������ֵ+l��r�Ľϴ��ߣ����㸸�׽ڵ㴦��

    �ܽ᣺�����漰���������⣬�õݹ�ȥ��⡣��Ϊ�������Ķ�����ǵݹ��
*/

struct TreeNode
{
    int val;
    TreeNode* left;
    TreeNode* right;
};


int childLength(TreeNode* p, int& maxRoute)
{
    if (!p)return 0;

    int leftLength = max(0, childLength(p->left, maxRoute));
    int rightLength = max(0, childLength(p->right, maxRoute));

    maxRoute = max(p->val + leftLength + rightLength, maxRoute);

    return max(0,p->val + max(leftLength, rightLength));
}

int maxPathSum(TreeNode* root) {
    int maxRoute = INT_MIN;
    childLength(root, maxRoute);
    return maxRoute;
}