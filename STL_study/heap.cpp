/*
heap��ʹ��vector���Ϸ��������㷨�ģ�һ����Ϊ�������ȼ������ĵײ㣬����ֱ������ʹ��
heap��Ҫ����ʵ��һЩ�ܹ�����ȡȨ����ߵ�Ԫ�ء�������
����֮�����Ƕ�Ԫ�������ȼ�Ҫ�������
����
priority queue��һ���ܹ��������Ԫ�أ���ȡ������������ȼ�Ϊ׼��
�����list��ʵ�֣�Ϊ�˶�ȡ���ȼ���ߵ�Ԫ�أ���Ҫ����������
�ֻ��߲���ʱ�ͽ������򣬵�����������ÿ�β���Ҳ��Ҫ����
Ϊ��������ȼ�Ҫ��������Ч�ʣ�stl�ṩ��heap
*/

/*
��ȫ��������
����Ҷ��֮�⣬�����ڵ�ȫ���������ӽڵ㡣
һ���ڵ��ǵ�i��Ԫ�أ��������ӽڵ���2i�����ӽڵ���2i+1�����ڵ���i/2
*/

/*
����Ԫ��
��ǰ�ڵ���ĩβ
ѭ���������ֵ���ڸ��ڵ㣬�ͰѸ��ڵ��ֵ������ǰ�ڵ�
��ǰ�ڵ����Ϊ���ڵ㣬����һ��ѭ��
ֱ����ֵ�ȵ�ǰ�ڵ��ֵС������ֵ������ǰ�ڵ㡣
*/

//���²����Ԫ�ؽ��ж�λ
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
    Distance parent =(holeIndex-1)/2;//�ҳ����ڵ��±�
    while(holeIndex>topIndex && *(first+parent)<value){//ѭ����Ԫ�ر����ĸ��ڵ�󣬾ͺ͸��ڵ㽻����ֱ���ȸ��ڵ�С��
        *(first+holeIndex)=*(first +parent);
        holeIndex=parent;
        parent=(holeIndex-1)/2;//�ҵ��µĸ��ڵ�
    }
    *(first +holeIndex)=value;//��value������λ�õĽڵ�
}
/*
ɾ�����Ԫ�أ�Ҳ���������ڵ㡣��ǰ�ڵ�ָ��������secondChildָ�����ӽڵ�
ѭ���������ӽڵ�Ƚϴ�С��secondChildȡ�ϴ�ģ�������ֵ������ǰ�ڵ�
      ��ǰ�ڵ����Ϊ���ӽڵ㣬secondChildȡ��ǰ�ڵ�����ӽڵ�
ֱ��secondChild�Ѿ�������ĩβ=>last��
���secondChildָ��last������ǰ�ơ�

*/


//ɾ�����Ԫ��
template<class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first,RandomAccessIterator last){
    __pop_heap_aux(first,last,value_type(first));//�����������䣬��ʼԪ�ص�ֵ
}

template<class RandomAccessIterator,class T>
inline void __pop_heap_aux(RandomAccessIterator first,RandomAccessIterator last ,T*){
    __pop_heap(first,last-1,last-1,T(*(last-1)),distance_type(first));//������ʼ��������ĩβԪ�ص���������Ԫ��ƫ����
}

template<class RandomAccessIterator, class T, class Distance>
inline void __pop_heap(RandomAccessIterator first,RandomAccessIterator last,RandomAccessIterator result,T value ,Distance*){
    *result=*first;
    __adjust_heap(first,Distance(0),Distance(last-first),value);
}

//����heap
template <class RandomAccessIterator ,class Distance,class T>
void __adjust_heap(RandomAccessIterator first,Distance holeIndex,Distance len,T value){
    Distance topIndex=holeIndex;
    Distance secondChild=2*holeIndex+2;//���ӽڵ�ƫ����
    while(secondChild<len){//�����ǰ�ڵ�������ӽڵ�
        if(*(first+secondChild)<*(first+(secondChild-1)))//��secondChildȡ��ǰ�ڵ���ӽڵ��нϴ��һ��
            secondChild--;
        *(first+holeIndex)=*(first+secondChild);
        holeIndex=secondChild;
        secondChild=2*(secondChild+1);
    }
    //������������ӽڵ㣬
    if(secondChild==len){
        *(first+holeIndex)=*(first+(secondChild-1));
        holeIndex =secondChild-1;
    }
}


//pop_heapȡ��heap�����Ԫ�أ�������vector��ĩβ��size֮��
//���ѭ��ȡ��heap������Ԫ�أ��ͻ�õ�һ��������vector���������㷨����sort_heap

template <class RandomAccessIterator >
void sort_heap(RandomAccessIterator first,RandomAccessIterator last){
    while(last-first>1){
        pop_back(first,last--);
    }
}


//��һ�������Ԫ��ת��Ϊheap
template <class RandomAccessIterator ,class T ,class Distance>
void make_heap(RandomAccessIterator first,RandomAccessIterator last){
    __make_heap(first,last,value_type(first),distance_type(first));
}

template <class RandomAccessIterator ,class T ,class Distance>
void __make_heap(RandomAccessIterator first,RandomAccessIterator last,T*,Distance*){
    if(last-first<2)return;//��0��1��Ԫ�أ����ܹ��ɶ�����
    Distance len=last-first;
    Distance parent=(len-2)/2;
    while(true){
        __adjust_heap(first,parent)
    }
}
