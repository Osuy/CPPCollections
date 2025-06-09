#pragma once
/*
	vector
		迭代器：
			random access
	list
		迭代器：
			bidirecrtional
	duque
		迭代器：
			random access
	红黑树rbtree
		迭代器：
			bidirecrtional
			红黑树的根节点也有父节点，是隐藏的nil。nil的父亲也是树的根节点
			nil的左孩子是树最左的节点，右孩子是树最右的节点
			begin()返回nil的左孩子，end()返回nil自身。因为能很方便地指向最小和最大，逻辑上遍历又是可逆的
			因此容易实现双向迭代，故迭代器类型是双向类型

		模板参数：
			Key：键值
			Value：实值
			KeyOfValue：从实值提取出键值
			Compare：比较键值大小的函数对象
			从Value里提取Key，在rbtree和hashtable上都有用到。因为容器存放的是Value，而不存放Key。那么为什么不把Key和Value都存放进去呢？
			事实上也是这么做的。在map和unoredered_map里，Value实际上是pair<const Key, T>，而T才是我们认为的实值
			看似多此一举，实际上是在模板参数层面上实现了const key，让map的键值不可以被修改；
			另外，也提供了一层间接层，允许自定义如何从Value里获取Key，以便实现一些更复杂的功能

		特征：
		1.有红黑两种颜色的节点，
		2.根节点为黑色，
		3.父子不能同时为红
		4.任意节点至其子树的叶子节点的路径上黑节点数量相等
		根据这些性质，如果忽略红节点，只看黑节点，那么红黑树是一颗满二叉树，所有叶子严格在同一层（又因为至多有两个红孩子，换言之是一颗4阶B树）
		如果同时看红、黑节点，那么红黑树最长的分支（黑红黑红黑红...）其长度至多是最短的分支（黑黑黑...）的两倍
		以上，相比于严格平衡的avl树，红黑树对黑节点严格平衡，又对红节点十分宽容，允许红节点多的分支更不平衡

		那么红节点有什么特征呢？
		首先对于任何新插入的节点，都先看作红色。因为看作黑色就直接导致红黑树不平衡，看作红色，如果引起了不平衡，再去变色和旋转
		其次，把一个黑节点b和它的红孩子r看作一个4阶B树的节点，有四种情况：b、rb、br、rbr
		那么往其中插入节点，会出现三种情况
			1.插入b、插入rb的右边、插入br的左边：直接插入即可
			2.插入rb的左边、插入br的右边：进行一次旋转，然后变色，成为rbr节点即可
			3.插入到rbr：rbr分裂，父亲和叔叔变黑，新节点直接插入到父亲下。然后祖父变红，向上插入到曾祖父，递归直到根节点

		对于情况3，这是一个递归的过程，最多会插入到根节点。如果根节点是rbr，那么根节点也会分裂，于是整颗红黑树的高度就会提高一层
		假设有一颗红黑树已满：（0为红，1为黑）
			    1
			0        0
		 1    1    1    1
		0 0  0 0  0 0  0 0
		往其中插入一个任意值，例如最右边，首先使最右边的两个00变为1，并让新的值作为最右1的子节点
				1
			0        0
		 1    1    1    1
		0 0  0 0  0 0  1 1
						  0
		然后祖父节点变为0，向上插入
				1
			0        0
		 1    1    1    0
		0 0  0 0  0 0  1 1
						  0
		祖父向上插入的依然是一个rbr节点，于是重复
				1
			1        1
		 1    1    1    0
		0 0  0 0  0 0  1 1
						  0
		根节点为黑，所以不会变0。到此，插入结束，整个红黑树的层数加1，并且多出了一些不满的节点

		可以发现，如果一个节点是红色的，那么它要么是直接插入的，要么是递归插入的。而插入操作恰好是引起红黑树不平衡的主要原因。
		因此，红黑树对红节点宽容，允许有更大的不平衡，就是优化了插入效率。
		avl树，尤其是一颗已经容纳很多元素的avl树，每次插入几乎都要进行旋转，更甚还需双旋
		而红黑树的插入大概率是变色和单旋，很少会递归插入。就算是递归插入，它也允许红节点不平衡，递归大概率会在还未到达根节点时就结束
		这也就是红黑树插入效率高于avl树的原因

		对于删除（待研究）
		对于++和--
			（待研究）参考二叉树的前序遍历
*/
//经过封装后的容器：
//给出rbtree的模板参数：Key、Value、KeyOfValue、Compare
//set
template< class Key, class Compare = less<Key>, class Alloc = alloc>
class set
{
	//Key            传给  Key和Value，即键值就是实值，只需存放键值即可
	//identity<Key>  传给  KeyOfValue，identity直接返回参数，不做其他任何事，即提取键值key就是什么也不做地把Value返回
	//Compare        传给  Compare，用于比较两个key。默认是less<Key>
	typedef rb_tree<Key, Key, identity<Key>, Compare, Alloc> rep_type;
};

//map
template< class Key, class T, class Compare = less<Key>, class Alloc = alloc>
class map
{
	//Key                 传给  Key
	//pair<const Key, T>  传给  Value，表示map实际上存放的是一个pair，即同时存放了键值和元素
	//select1s<Value>     传给  KeyOfValue，提取键值的操作，就是取出pair的first
	//Compare             传给  Compare，用于比较两个key。默认是less<Key>
	typedef pair<const Key, T> Value;
	typedef rb_tree<Key, Value, select1s<Value>, Compare, Alloc> rep_type;
};


/*
	hashtable
		迭代器：
			forward
			hashtable是基于散列的，键值间没有大小关系。用迭代器去迭代容器，并不能得到从小到大的序列
			（散列是对键值哈希化后取模，因此如果桶组是53，而所有元素都小于等于53，则会出现有序的情况）
			在数据结构方面，是使用桶组+单向链表，也就只具备一个方向迭代的能力
			因此只能forward迭代

		模板参数：
			Value：实值
			Key：键值
			HashFcn：哈希化函数
			ExtractKey：从实值提取出键值
			EqualKey：比较键值是否相等的函数对象
			与rbtree对比，多了一个Hash，比较大小的Compare变成了比较是否相等的EqualKey
			ExtractKey的作用等同与KeyOfValue，都是从实值提取出键值。很容易想到，
			hashtable和rbtree一样，只存放Value，并且提供了提取Key的间接层
			这样做同样是为了实现unoredered_map

		特征：
		hashtable使用vector作为桶组，桶组存放的是单链表节点指针，默认为空
		由于散列操作是取模，因此桶组长度最好是质数。stl提供了28个质数来作为桶组的长度，后一个是前一个的两倍左右，第一个质数是53
		hashtable的负载因子是1，即当元素数量大于桶组长度时，就会扩大桶组长度。
		这样是为了保证每个桶内近似只有一个元素，以实现O(1)的访问
		

*/
//hash_set
template <class Value, class HashFcn = hash<Value>, class EqualKey = equal_to<Value>, class Alloc = alloc>
class hash_set
{
	//Value           传给  Value和Key，即键值就是实值
	//HashFcn         传给  HashFcn，默认hash<Value>
	//identity<Value> 传给  ExtractKey，即直接把Value当作key
	//EqualKey        传给  EqualKey，默认equal_to<Value>
	typedef hashtable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc> ht;
};

//hash_map
template<class Key, class T, class HashFcn = hash<Key>, class EqualKey = equal_to<Key>, class Alloc = alloc>
class hash_map
{
	//pair<const Key, T>   传给  Value
	//Key                  传给  Key
	//HashFcn              传给  HashFcn，默认hash<Value>
	//select1st<Value>     传给  ExtractKey，把pair的first当作Key
	//EqualKey             传给  EqualKey，默认equal_to<Value>
	typedef hashtable<pair<const Key, T>, Key, HashFcn, select1st< pair<const Key, T>> EqualKey, Alloc> ht;
};