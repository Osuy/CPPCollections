/*
stack是一种先进后出的数据结构。只有一个出口。
能被操作的只有栈口（顶端）元素，只有先取出栈口元素，才能对下一个元素操作
换言之，stack不可以遍历，所以不需要迭代器
由于stack十分像一个只有一个出口的deque，
可以通过一些配接器修改deque的接口实现stack
因此stack往往不被归类为container，而是container adapter
stack相当于将其他容器的接口修改的配接器adapter
*/

template <class T,class Sequence=deque<T>>//默认用deque实现stack，也可以使用list
class stack{

    //以下见1.9.1
    friend bool operator== __STL_NULL_TMPL_ARGS(const stack*,const stack&);
    friend bool operator<= __STL_NULL_TMPL_ARGS(const stack*,const stack&);


public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;

protected:
    Sequence c;//底层容器
public:
    bool empty()const{returen c.empty();}

    size_type size()const{return c.size();}

    reference top(){return c.back();}

    const_reference top()const {return c.back();}

    void push(const value_type& x){c.push_back(x);}

    void pop(){c.pop_back();}

};

template <class T,class Sequence>
bool operator==(const stack<T,Sequence>&x,const stack<T,Sequence>&y){
    return x.c==y.c;
}

template <class T,class Sequence>
bool operator<(const stack<T,Sequence>&x,const stack<T,Sequence>&y){
    return x.c<y.c;
}
