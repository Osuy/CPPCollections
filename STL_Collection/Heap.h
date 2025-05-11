/*
	heap本质上来将是一个Vector+一系列heap算法。使其能对插入的元素自动排序
	heap并不对外公开，而是作为priority queue的底层容器。
	heap在结构上是一颗完全二叉树，其树根节点索引为0
	对一个节点，若其索引为i，则其左子节点索引为2i+1，右子节点索引为2i+2。
*/


/*
	heap算法中尤为关键的是插入函数push_heap。
	由于要满足完全二叉树的条件，新插入的元素必须位于最后一层最左的空节点，然后再进行上溯。
	将新节点与父节点比较，如果新节点较大（或根据自定义compare），就进行互换。

	使用push_heap
	配合vector的push_back使用，将新值放入vector的尾部，然后传入新区间
*/

template<class RandomAccessIterator>//接受随机访问迭代器
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last)//对heap进行插入需要整个容器的头尾区间
{
	//转交给底层函数实现，传参：区间、距离单位和first的值
	__push_heap_aux(first, last, distance_type(first), value_type(first));
}

template <class RandomAccessIterator,class Distance ,class T>
inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*)//Distance*, T*为占位参数
{
	__push_heap(first, Distance(last - first -1), Distance(0), T*(last - 1));//first、区间长度、0距离，尾节点的值
}

//插入函数，对元素value在holeIndex处插入，会进行上溯，上限是topIndex
template <class RandomAccessIterator, class Distance, class T>
void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value)//试图再holeIndex的位置插入value，在那之前先上溯找到正确的位置。
{
	Distance parent = (holeIndex - 1) / 2;//父节点索引
	while (holeIndex > topIndex && *(first + parent) < value)//*(first + parent)是父节点的值，如果小于value，则互换，再上溯，直到根节点
	{
		*(first + holeIndex) = *(first + parent);//父节点的值赋给此节点
		holeIndex = parent;//此节点的索引改为父节点
		parent = (holeIndex - 1) / 2;//重新找到父节点
	}
	//退出循环，说明找到了第一个不比父节大的节点，也可能是根节点，将value赋给此节点
	*(first + holeIndex) = value;
}


/*
	pop_heap是提取最大节点的函数，即提取根节点，将它放在vector的最后一个元素的位置。最后一个元素的值另外寻找位置
	然后在根节点进行下溯：找到较大的子节点，将值拷贝给根节点，然后重复，继续找较大的孙子节点 。。。。。
	直到找到一个节点没有子节点，作为插入点。
	最后将一个元素的值在插入点插入（用__push_heap，会进行上溯）。
	如此能保证heap整体的有序性。

*/

template <class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last)//传入区间
{
	__pop_heap_aux(first, last, value_type(first));
}

template <class RandomAccessIterator, class T>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*)//传入区间和首节点的值
{
	__pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first));//传入[first,last-1)区间（因为最后一个结点要被删除），区间尾迭代器，尾元素值
	//T(*(last - 1))这一项是尾元素值，会在__adjust_heap里进行插入。
}

template <class RandomAccessIterator, class T,class Distance>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*)//
{
	*result = *first;//将首节点的值（根值）保存在result（最后一个元素）的位置
	__adjust_heap(first, Distance(0), Distance(last - first), value);//传入首元素迭代器，根索引，区间长度和尾元素值
}

/*
	堆调整函数，要求传入的迭代器尾随机访问迭代器
	参数：
		first：指向整个容器的第一个元素的迭代器
		holeIndex：调整范围的起点
		len：容器长度
		value：
	作用：忽略起点的值，将范围内的元素调整尾大顶堆，然后在找到的位置插入value
*/
template <class RandomAccessIterator, class Distance, class T>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value)
{
	//分别指向范围起点元素和其左孩子
	Distance topIndex = holeIndex;
	Distance secondChild = 2 * holeIndex + 2;
	
	while (secondChild < len)//如果右孩子有效
	{
		if (*(first + secondChild) < *(first + secondChild - 1)
		{
			secondChild--;//左孩子更大就用让secondChild指向左孩子
		}
		*(first + holeIndex) = *(first + secondChild);//将孩子的值赋给起点
		
		//更新索引
		holeIndex = secondChild;
		secondChild = 2 * holeIndex + 2;
	}

	//退出循环后，存在secondChild == len的情况，此时当前节点没有右节点，但还有左节点，还需单独与左节点互换一次
	if (secondChild == len)//如果找到的节点没有右节点，
	{
		*(first + holeIndex) = *(first + secondChild - 1);
		holeIndex = secondChild - 1;
	}

	//找到了当前节点，用push将value插入到heap中，保证了value插入holeIndex的位置，同时自动上溯
	__push_heap(first, holeIndex, topIndex, value);
}

/*
	sort算法
	由于每次pop都取出heap中的最大元素，放在vector的末尾，因此只要将所有元素都取出，
	就能自动的到一个从小到大的有序序列
*/

template <class RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	while(last - first > 1)//直到只剩一个就不需要pop了
	{
		pop_heap(first, last--);
	}
}


/*
	make_heap 
	将现有的数据转化尾一个heap

*/

template <class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	__make_heap(first, last, value_type(first), distance_type(first));
}

template <class RandomAccessIterator,class T,class Distance>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*)
{
	if (last - first < 2) return;//0或1个元素的区间无需排序

	//取区间长度和最后一个非叶子节点。
	Distance len = last - first;
	Distance parent = (len - 2) / 2;


	//parent向前遍历到0，相当于遍历每一个子树，对该子树调整为大顶堆。
	//退出后，所有子树都是大顶堆，因而整个数组是大顶堆
	while (true)
	{
		__adjust_heap(first, parent, len, T(*(first + parent)));
		if (parent == 0)return;
		parent--;
	}
}