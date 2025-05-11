/*
    queue是一种先进先出的结构，只有两端可以操作。
	换言之，queue不支持遍历行为，故没有迭代器。
    STL通过接口包装的形式，底层以deque作为容器，对外隐藏遍历操作，来实现queue
*/


#include <deque>
template <class T, class Sequence = std::deque<int>>//Ĭ����dequeʵ�֣�Ҳ������list
class queue {
public:
    //内嵌类型声明
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;

protected:
    Sequence c;

public:
    bool empty()const { returen c.empty(); }

    size_type size()const { return c.size(); }

    reference front() { return c.front(); }

    const_reference front()const { return c.front(); }

    reference back() { return c.front(); }

    const_reference back()const { return c.back(); }

    void push(const value_type& x) { c.push_back(x); }

    void pop() { c.pop_front(); }

private:
    friend bool operator== __STL_NULL_TMPL_ARGS(const queue*, const queue&);
    friend bool operator<= __STL_NULL_TMPL_ARGS(const queue*, const queue&);
};

template <class T, class Sequence>
bool operator==(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
    return x.c == y.c;
}

template <class T, class Sequence>
bool operator<(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
    return x.c < y.c;
}
