/*
heap是使用vector加上发二叉树算法的，一般作为其他优先级容器的底层，很少直接拿来使用
heap主要用来实现一些能够“读取权重最高的元素”的容器
换言之，就是对元素有优先级要求的容器
例：
priority queue是一个能够任意插入元素，但取出是以最高优先级为准；
如果用list来实现，为了读取优先级最高的元素，就要遍历容器。
又或者插入时就进行排序，但这样以来，每次插入也需要遍历
为了提高优先级要求容器的效率，stl提供了heap
*/

/*
完全二叉树：
除了叶子之外，其他节点全部有两个子节点。
一个节点是第i个元素，他的左子节点是2i，右子节点是2i+1，父节点是i/2
*/

/*
插入元素
当前节点是末尾
循环：如果新值大于父节点，就把父节点的值赋给当前节点
当前节点调整为父节点，再下一轮循环
直到新值比当前节点的值小，把新值赋给当前节点。
*/

//对新插入的元素进行定位
template <class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first,
                      RandomAccessIterator last){
    __push_heap_aux(first,last,distance_type(first),value_type(first));
}


template <class RandomAccessIterator ,class Distance,class T>
inline void __push_heap_aux(RandomAccessIterator first,
                            RandomAccessIterator last,Distance*,T*){
    __push_heap(first,Distance((last-first)-1),Distance(0),T(*(last-1)));
}

template <class RandomAccessIterator ,class Distance,class T>
void __push_heap(RandomAccessIterator first,Distance holeIndex,
                 Distance topIndex,T value){
    Distance parent =(holeIndex-1)/2;//找出父节点下标
    while(holeIndex>topIndex && *(first+parent)<value){//循环：元素比他的父节点大，就和父节点交换，直到比父节点小。
        *(first+holeIndex)=*(first +parent);
        holeIndex=parent;
        parent=(holeIndex-1)/2;//找到新的父节点
    }
    *(first +holeIndex)=value;//将value赋给定位好的节点
}
/*
删除最大元素，也就是树根节点。当前节点指向树根，secondChild指向右子节点
循环：左右子节点比较大小，secondChild取较大的，把他的值赋给当前节点
      当前节点调整为大子节点，secondChild取当前节点的右子节点
直到secondChild已经超过了末尾=>last。
如果secondChild指向last，就往前移。

*/


//删除最大元素
template<class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first,RandomAccessIterator last){
    __pop_heap_aux(first,last,value_type(first));//传入容器区间，起始元素的值
}

template<class RandomAccessIterator,class T>
inline void __pop_heap_aux(RandomAccessIterator first,RandomAccessIterator last ,T*){
    __pop_heap(first,last-1,last-1,T(*(last-1)),distance_type(first));//传入起始迭代器，末尾元素迭代器，首元素偏移量
}

template<class RandomAccessIterator, class T, class Distance>
inline void __pop_heap(RandomAccessIterator first,RandomAccessIterator last,RandomAccessIterator result,T value ,Distance*){
    *result=*first;
    __adjust_heap(first,Distance(0),Distance(last-first),value);
}

//调整heap
template <class RandomAccessIterator ,class Distance,class T>
void __adjust_heap(RandomAccessIterator first,Distance holeIndex,Distance len,T value){
    Distance topIndex=holeIndex;
    Distance secondChild=2*holeIndex+2;//右子节点偏移量
    while(secondChild<len){//如果当前节点存在右子节点
        if(*(first+secondChild)<*(first+(secondChild-1)))//让secondChild取当前节点的子节点中较大的一个
            secondChild--;
        *(first+holeIndex)=*(first+secondChild);
        holeIndex=secondChild;
        secondChild=2*(secondChild+1);
    }
    //如果不存在右子节点，
    if(secondChild==len){
        *(first+holeIndex)=*(first+(secondChild-1));
        holeIndex =secondChild-1;
    }
}


//pop_heap取走heap的最大元素，并放在vector的末尾，size之外
//如果循环取走heap的所有元素，就会得到一个递增的vector。这样的算法就是sort_heap

template <class RandomAccessIterator >
void sort_heap(RandomAccessIterator first,RandomAccessIterator last){
    while(last-first>1){
        pop_back(first,last--);
    }
}


//将一段区间的元素转化为heap
template <class RandomAccessIterator ,class T ,class Distance>
void make_heap(RandomAccessIterator first,RandomAccessIterator last){
    __make_heap(first,last,value_type(first),distance_type(first));
}

template <class RandomAccessIterator ,class T ,class Distance>
void __make_heap(RandomAccessIterator first,RandomAccessIterator last,T*,Distance*){
    if(last-first<2)return;//就0或1个元素，不能构成二叉树
    Distance len=last-first;
    Distance parent=(len-2)/2;
    while(true){
        __adjust_heap(first,parent)
    }
}
