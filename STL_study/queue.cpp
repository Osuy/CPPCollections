/*
queue��һ���Ƚ��ȳ������ݽṹ��ֻ��һ����ں�һ������
��������ڲ���Ԫ�أ�����ɾ���Ͷ�ȡԪ�ء�
queue���ܱ�����Ҳû�е�����
*/

template <class T,class Sequence=deque<int>>//Ĭ����dequeʵ�֣�Ҳ������list
class queue{

    friend bool operator== __STL_NULL_TMPL_ARGS(const queue*,const queue&);
    friend bool operator<= __STL_NULL_TMPL_ARGS(const queue*,const queue&);


public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;

protected:
    Sequence c;

public:
    bool empty()const{returen c.empty();}

    size_type size()const{return c.size();}

    reference front(){return c.front();}

    const_reference front()const {return c.front();}

    reference back(){return c.front();}

    const_reference back()const {return c.back();}

    void push(const value_type& x){c.push_back(x);}

    void pop(){c.pop_front();}

};

template <class T,class Sequence>
bool operator==(const stack<T,Sequence>&x,const stack<T,Sequence>&y){
    return x.c==y.c;
}

template <class T,class Sequence>
bool operator<(const stack<T,Sequence>&x,const stack<T,Sequence>&y){
    return x.c<y.c;
}
