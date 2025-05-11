#pragma once
#include <algorithm>
#include <stdlib.h>
using std::max;
/*
	对于树的任意两个节点，它们之间必然存在一条路径
	路劲上的节点的权值和为路径的长度
	求树的最大路径长度

    首先，两个节点的连通路径必然经过它们的共同祖先。
    自上而下去看，递归方式解决

        对于一个节点，如果为空，返回0
        如果不为空：
            得到他的左子树的最长树枝，和0取较大者，赋给l
            得到他的右子树的最长数值，和0取较大者，赋给r

            自身的值+l+r>maxlength （maxlength为引用参数）
            则修改maxlength

        然后返回自身的值+l与r的较大者，方便父亲节点处理

    总结：单凡涉及二叉树的题，用递归去求解。因为二叉树的定义就是递归的
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