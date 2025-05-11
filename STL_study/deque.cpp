/*
deque是双向开口的伪连续空间容器
vector是单向开口的连续空间容器。
deque可以在头尾两端进行插入和删除操作。
deque的动态扩展并不会像vector那样需要大量的拷贝
在底层上，deque由一个中控器和多个缓冲区组成
中控器是一段连续的内存，类似数组，存放指向缓冲区的指针
缓冲区是由用户定义长度或默认长度的连续内存，存放容器的元素
*/

//map为中控器
template <class T, class Alloc = alloc, size_t BufSiz = 0>
class deque {
public:
    typedef T value_type;
    typedef value_type* pointer;
    ...
protected:
    typedef pointer* map_pointer;

protected:
    map_pointer map;
    size_type map_size;

};

/*
deque的迭代器：
为了实现队列的效果，当迭代器遍历容器时，
到达一个缓冲区的边界，就要跳转到下一目标的缓冲区
一个迭代器具有四个指针：cur、first、last、node
cur指向迭代器目前指向的元素
first和lasr指向一个缓冲区的头尾
node指向中控器，中控器再指向目标缓冲区
当cur==last时，node++，first和last更新为node.first和node.last
*/

//deque的迭代器不继承std::iterator,所以必须自行撰写五个内嵌声明
template <class T, class Ref, class Ptr, size_t BufSiz>
struct __deque_iterator {
    typedef __deque_iterator<T, T&, T*, BufSiz>  iterator;
    typedef __deque_iterator<T, const T&, const T*, BufSiz> const_iterator;
    static size_t buffer_size() {
        return __deque_buf_size(BufSiz, sizeof(T));  //返回一个缓冲区能装下元素的个数
    }

    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T** map_pointer;

    typedef __deque_iterator self;

    T* cur;
    T* first;
    T* last;
    map_pointer node;
};

//__deque_buf_size()是一个全局函数，具体如下
inline size_t __deque_buf_size(size_t n, size_t sz) {
    return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1)); //
}

//实现迭代器跳转缓冲区的行为
//将新的node赋给迭代器的弄得
//node节点指向缓冲区的第一个元素，所以first指向*new_node
//last等于first向后偏移缓冲区容纳元素个数的单位量
void set_node(map_pointer new_node) { //将自身设置为指定map_node，通常参数是node+1
    node = new_node;
    first = *new_node;
    last = first + difference_type(buffer_size());
    //调整cur的操作不在此函数内完成
}

//重要的重载
reference operator*()const {
    return *cur;
}

pointer operator->() const {
    return &(operator*();
}
//计算自身与另一迭代器it之间的偏移量，it在后面，偏移量时负
difference_type operator-(const self& x) const {
    //如果在同一个缓冲区中设bufsz=8，cur=2，x.cur=5
    //偏移量就是-8+2+3=-3
    return difference_type(buffer_size()) * (node - x.node - 1) + (cur - first) + (x.last - x.cur); //妙啊
}

self& operatpr++() { //因为last指向最后一个元素的下一个，所以先++再判断
    ++cur;
    if(cur == last) {
        set_node(node + 1);
        cur = first;
    }
    return *this;
}

self operator++(int) {
    self tmp = *this;
    ++*this;
    return tmp;
}

self& operatpr--() {
    if(cur == first) { //first指向第一个元素，所以先判断再--
        set_node(node - 1);
        cur = first;
    }
    --cur;
    return *this;
}

self operator--(int) {
    self tmp = *this;
    --*this;
    return tmp;
}

//实现随机存取
self& operator+=(difference_type n) {
    difference_type offset = n + (cur - first);//当前位置减去首元素再加上n，如果数值大于buffsize，说明需要跳转
    if(offset >= 0 && offset < difference_type(buffer_size()))
        cur += n; //要跳转就直接+=
    else {
        difference_type node_offset = offset > 0 ?
                                      offset / difference_type(buffer_size()) : //偏移量为正就向后
                                      -difference_type((-offset - 1) / buffer_size()) - 1; //偏移量为负就向前
        //解释，如果刚好偏移到前一个缓冲区的第一个元素的位置
        //offset就是8，不减1再除8就会得到-2的偏移，不和要求
        set_node(node + node_offset);
        cur = first + (offset - node_offset * difference_type(buffer_size()));
    }
    return *this;
}

self operator+(difference_type n)const {
    self tmp = *this;
    return tmp += n;
}

self& operator-=(difference_type n) {
    return *this += -n;
}

self operator-(difference_type n)const {
    self tmp = *this;
    return tmp -= n;
}

reference operator[](difference_type n)const {
    return *(*this + n);
}

bool operator==(const self& x)const {
    return == x.cur;
}

bool operator!=(const self& x) const {
    return !(*this == x);
}

bool operator<(const self& x)const {
    return (node == x.node) ? (cur < x.cur) : (node < x.node);
}


/*
deque包含一个中控器和数个缓冲区，其中中控器的元素是指向缓冲区的指针
此外还有基本的start和finish指针，指向中控器的指针和map_size
*/

template <class T class Alloc = alloc, size_t BufSiz = 0>
class deque {
    typedef T value_type;
    typedef value_type* pointer;
    typedef size_t size_type;

public:
    typedef __deque_iterator<T, T&, T*, BufSiz> iterator;

protected:
    typedef pointer* map_pointer;


protected:
    iterator start;
    iterator finish;
    map_pointer map;
    size_t map_pointer;

public:
    iterator begin() {
        return start;
    }
    iterator end() {
        return finish;
    }
    reference operator[](size_type n) {
        return start[difference_type(n)];
    }
    reference front() {
        return *start;
    }
    reference back() {
        iterator tmp = finish;
        --tmp;
        return *tmp;
    }

    size_type size()const {
        return finish - start;
    }

    size_type max_size()const {
        return size_type(-1);
    }

    bool empty()const {
        return start == finish;
    }

    void push_back(const value_type& t){
        if(finish.cur!=finish.last-1){//这里是不等与，所以要求至少还有两个备用空间
            construct(finish.cur,t)
        }
        else{//只剩一个备用空间，插入后，就创建一个新的缓冲区
            push_back_aux(t);
        }
    }

    void push_back_aux(const value_type& t){
        value_type t_copy=t;
        reserve_map_at_back();
        *(finish.node+1)=allcate_node();
        __STL_TRY{
            construct(finish.cur,t_copy);
            finish.set_node(finish.node+1);
            finish.cur=finish.first;
        }
        __STL_UNWIND(deallocate_node(*(finish.node+1)));

    }

    void push_front(const value_type& t){
        if(start.cur!=start.first){
            construct(start.cur-1,t);
            --start.cur;
        }
        else{
            push_front_aux(t);
        }
    }
};


//deque的数据结构
template<class T, class Alloc = alloc, size_t BufSiz>
class deque {
    //声明两个专属的空间配置器
    typedef simple_alloc<value_type, Allco> data_allocator;
    typedef simple_alloc<pointer, Allco> map_allocator;

    deque(int n const value_type& value): start(), finish(), map(0), map_size(0) {
        fill_initialize(n, value);
    }

};

template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::fill_initialize(size_type n, const value_type& value) {
    create_map_and_nodes(n);
    map_pointer cur;
    __STL_TRY{
        for(cur = start.node; cur < finish.node; ++cur)
            uninitialized_fill(*cur, *cur + buffer_size(), value);
        uninitialized_fill(finish.first, finish.cur, value);
    } catch(...) {
        ...
    }
}

//
template <class T, class Alloc, size_t BufSize>
void void deque<T, Alloc, BufSize>::create_map_and_nodes(size_type num_elements) {
    //需要中控器节点个数=元素个数/缓冲区尺寸+1
    size_type num_nodes=num_elements/buffer_size()+1;

    //在指定个数和需要个数+2中取最大值，以便扩展
    map_size =max(initial_map_size(),num_node+2);
    map=map_allocator::allocate(map_size);

    map_pointer nstart=map+(map_size-num_nodes)/2;
    map_pointer nfinish=nstart+num_nodes-1;
    mappointer cur;

    __STL_TRY{
        for(cur=nstart;cur<=nfinish;++cur){
            *cur=allocate_node();
        }
    }

    //设置中控器指向头尾缓冲区
    start.set_node(nstart);
    finish.set_node(nfinish);

    //设置迭代器指向起始和末尾元素
    start.cur=start.finish;
    finish.cur=finish.first+num_elements % buffer_size();


}
