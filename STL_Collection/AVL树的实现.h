#pragma once

template<class T>
struct AVLNode
{
	T data;
	int height;
	AVLNode<T>* lchild;
	AVLNode<T>* rchild;

	AVLNode() :lchild(nullptr), rchild(nullptr), height(0) {}
	AVLNode(T d, AVLNode<T>* l = nullptr, AVLNode<T>* r = nullptr) :
		data(d),
		lchild(l),
		rchild(r),
		height(0) {}
};

//求树高
template<class T>
int Height(AVLNode<T>* cur)
{
	if (cur == nullptr)return 0;

	int i = Height(cur->lchild);
	int j = Height(cur->rchild);
	return i < j ? j + 1 : i + 1;
}

//左旋：向左旋转
template<class T>
AVLNode<T>* LeftRotation(AVLNode<T>* cur)
{
	AVLNode<T>* prchild = cur->rchild;
	cur->rchild = prchild->lchild;
	prchild->lchild = cur;

	//更新每个cur和prchild的高度
	cur->height = max(Height(cur->lchild), Height(cur->rchild)) + 1;
	prchild->height = max(Height(prchild->lchild), Height(prchild->rchild)) + 1;
	
	return prchild;
}

//右旋：向右旋转
template<class T>
AVLNode<T>* RightRotation(AVLNode<T>* cur)
{
	AVLNode<T>* plchild = cur->lchild;
	cur->lchild = plchild->rchild;
	plchild->rchild = cur;

	//更新每个cur和prchild的高度
	cur->height = max(Height(cur->lchild), Height(cur->rchild)) + 1;
	plchild->height = max(Height(plchild->lchild), Height(plchild->rchild)) + 1;

	return plchild;
}

template<class T>
AVLNode<T>* Insert(AVLNode<T>* cur, T key)
{
	if (nullptr == cur)
	{
		cur = new AVLNode<T>(ket);
	}
	else if (key > cur->data)
	{
		cur->rchild = Insert(cur->rchild, key);
		if (Height(cur->rchild) - Height(cur->lchild) == 2)
		{
			AVLNode<T>* pre = cur;;
			if (key < pre->rchild)
			{
				pre->rchild = RightRotation(pre->rchild);
			}

			cur = LeftRotation(pre);
		}
	}
	else if (key < cur->data)
	{
		cur->lchild = Insert(cur->lchild, key);
		if (Height(cur->lchild) - Height(cur->rchild) == 2)
		{
			AVLNode<T>* pre = cur;;
			if (key < pre->lchild)
			{
				pre->lchild = LeftRotation(pre->lchild);
			}

			cur = RightRotation(pre);
		}
	}
	cur->height = max(Height(cur->lchild), Height(cur->rchild)) + 1;
	return cur;
}