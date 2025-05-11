//priority_queue以queue为底层实现，是adapter

template<class T ,class Sequence=vector<T>,
            class Compare=less<typename Sequence::value_type>>
class priority_queue{
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;
protected:
    Sequence c;
    Compare comp;//元素比较标准
public:
    priority_queue():c(){}
    explicit priority_queue(const Compare& x):c(),comp(x){}

    template<class InputIterator>
    priority_queue(InputIterator first,InputIterator last,const Compare& x):
        c(first,last),comp(x){make_heap(c.begin,c.end(),comp);}

    template<class InputIterator>
    priority_queue(InputIterator first,InputIterator last):
        c(first,last){make_heap(c.begin,c.end(),comp);}

    bool empty() const {return c.empty();}
    size_type size()const {return c.size();}
};
