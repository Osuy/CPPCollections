/*
	概述：
	STL的deque像是一个优化了头部添加的vector。注意deque和queue的区别
	STL的deque不仅支持头尾操作，也支持任意位置的操作。
	一旦头部或尾部的缓存区已满，deque会申请新的缓存区，因此deque不提供capacity和reserve方法。
	deque的分为中控器和缓存区。
	中控器是包含指向缓存区的指针的数组，在内部类型声明种如下：
	typedef T** map_pointer;   换言之，就是元素的二级指针
	缓存区是一段连续的空间，可以认为是数组或vector，在内部为T*

	优点：
	头部添加比vector高效
	缺点：
    只支持头尾添加，中间不能插入，因此没有insert
	排序和遍历更复杂，因此比vector慢（排序建议将deque复制到vector，利用stl的sort排好在复制回deque）
*/

//deque的迭代器（deque在空间上分段连续，因此++和--的重载尤为重要）
template <class T, class Ref, class Ptr, size_t BufSiz>
struct __deque_iterator {
    //内嵌类型声明
    typedef random_access_iterator_tag iterator_category;
    typedef T                          value_type;
    typedef Ptr                        pointer;
    typedef Ref                        reference;
    typedef size_t                     size_type;

    typedef ptrdiff_t                                          difference_type;
    typedef T**                                                map_pointer;
    typedef __deque_iterator<T, T&, T*, BufSiz>                iterator;
    typedef __deque_iterator<T, const T&, const T*, BufSiz>    const_iterator;
    typedef __deque_iterator                                   self;

    //三个指针，指向当前元素地址、所在缓存区的头尾地址
    //所在缓存区的指针是为了在添加时知道是否溢出
    T* cur;
    T* first;
    T* last;

    //指向中控器
    map_pointer node;


    //返回一个缓冲区能装下元素的个数
    static size_t buffer_size() {
        return __deque_buf_size(BufSiz, sizeof(T));  
    }

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
    reference operator*()const { return *cur; }

    pointer operator->() const { return &(operator*(); }

    //计算自身与另一迭代器it之间的偏移量，it在后面，偏移量时负
    difference_type operator-(const self& x) const {
        return difference_type(buffer_size()) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
    }

    self& operatpr++() { //因为last指向最后一个元素的下一个，所以先++再判断
        ++cur;
        if (cur == last) {
            set_node(node + 1);
            cur = first;
        }
        return *this;
    }

    self operator++(int) {
        self tmp = *this;
        ++* this;
        return tmp;
    }

    self& operatpr--() {
        if (cur == first) { //first指向第一个元素，所以先判断再--
            set_node(node - 1);
            cur = first;
        }
        --cur;
        return *this;
    }

    self operator--(int) {
        self tmp = *this;
        --* this;
        return tmp;
    }

    //实现随机存取
    self& operator+=(difference_type n) {
        difference_type offset = n + (cur - first);//当前位置减去首元素再加上n，如果数值大于buffsize，说明需要跳转
        if (offset >= 0 && offset < difference_type(buffer_size()))
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
};

//deque的数据结构
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

    void push_back(const value_type& t) {
        if (finish.cur != finish.last - 1) {//这里是不等与，所以要求至少还有两个备用空间
            construct(finish.cur, t)
        }
        else {//只剩一个备用空间，插入后，就创建一个新的缓冲区
            push_back_aux(t);
        }
    }

    void push_back_aux(const value_type& t) {
        value_type t_copy = t;
        reserve_map_at_back();
        *(finish.node + 1) = allcate_node();
        __STL_TRY{
            construct(finish.cur,t_copy);
            finish.set_node(finish.node + 1);
            finish.cur = finish.first;
        }
        __STL_UNWIND(deallocate_node(*(finish.node + 1)));

    }

    void push_front(const value_type& t) {
        if (start.cur != start.first) {
            construct(start.cur - 1, t);
            --start.cur;
        }
        else {
            push_front_aux(t);
        }
    }
};

template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::fill_initialize(size_type n, const value_type& value) {
    create_map_and_nodes(n);
    map_pointer cur;
    __STL_TRY{
        for (cur = start.node; cur < finish.node; ++cur)
            uninitialized_fill(*cur, *cur + buffer_size(), value);
        uninitialized_fill(finish.first, finish.cur, value);
    }
        catch (...) {
        ...
    }
}

//
template <class T, class Alloc, size_t BufSize>
void void deque<T, Alloc, BufSize>::create_map_and_nodes(size_type num_elements) {
    //需要中控器节点个数=元素个数/缓冲区尺寸+1
    size_type num_nodes = num_elements / buffer_size() + 1;

    //在指定个数和需要个数+2中取最大值，以便扩展
    map_size = max(initial_map_size(), num_node + 2);
    map = map_allocator::allocate(map_size);

    map_pointer nstart = map + (map_size - num_nodes) / 2;
    map_pointer nfinish = nstart + num_nodes - 1;
    mappointer cur;

    __STL_TRY{
        for (cur = nstart; cur <= nfinish; ++cur) {
            *cur = allocate_node();
        }
    }

        //设置中控器指向头尾缓冲区
    start.set_node(nstart);
    finish.set_node(nfinish);

    //设置迭代器指向起始和末尾元素
    start.cur = start.finish;
    finish.cur = finish.first + num_elements % buffer_size();


}
