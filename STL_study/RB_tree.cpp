/*
������ĸ��
ƽ�⣺���������ڵ����Ȳ����2
���������ӽڵ�<���ڵ�<�ҽڵ㣬�ң�һ���ڵ���������������нڵ㣬С�������������нڵ�

��ɫ��
1.���нڵ�ֻ�к�ɫ���ߺ�ɫ
2.���ڵ�Ϊ��ɫ����ǰ����Ľڵ�Ϊ��ɫ
3.�ӽڵ�͸��ڵ㲻��ͬʱΪ��
4.ȡ��һ�ڵ㣬��Ҷ�ڵ������·������ɫ�ڵ�����������ͬ
*/

//��ɫΪfalse����ɫΪtrue
typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red=false;
const __rb_tree_color_type __rb_tree_black=true;

//�������ڵ㣺������ɫ���ݡ�����ָ���������͵�ָ�롢ȡ�����Сֵ����
struct __tree_node_base{
    typedef __rb_tree_color_type color_type;
    typedef __rb_tree_node_base* base_ptr;

    color_type color;
    base_ptr parent;
    base_ptr_left;
    base_ptr right;

    //ȡ��Сֵ
    static base_ptr minimum(base_ptr x){
        while (x->left!=0)x=x->left;
        return x;
    }
    //ȡ���ֵ
    static base_ptr maximum(base_ptr x){
        while (x->right!=0)x=x->right;
        return x;
    }
};

//������ڵ㣬�̳л������ڵ�
//һ��ָ���������͵�ָ�롢һ������
template <class value>
struct __rb_tree_node : public __rb_tree_node_base{
    typedef __rb_tree_node<Value>* link_type;
    Value value_field;
};


//��������������һ��ָ�����������
struct __rb_tree_base_iterator{
    typedef __rb_tree_node_base::base_ptr base_ptr;
    typedef bidirectional_iterator_tag iterator_category;
    typedef ptrdiff_t difference_type;

    base_ptr node;//ָ��������ڵ��ָ��

    //����ʵ��++���أ�Ѱ��node�ĺ�̽ڵ㣨����������С�ڵ�/����node��Ľڵ㣩
    void increment(){
        if(node->right!=0){//�����������,��������Ѱ�Һ�̽ڵ�
            node=node->right;
            while(node->left!=0)//ѭ�������ҵ���С�ڵ�
                node=node->left;
        }
        else{//���û��������,ֻ�������ң�����ȡy=node->parent��
            base_ptr y=node->parent;
            /*
            ��Ϊ�����������һ���ڵ���������������нڵ㣬С�������������нڵ㡣
            ����Ҫ�ҵ���һ��node������������y������ʹy����node��
            ����ѭ���жϣ����node��y���ҽڵ㣬�������ң�ֱ���ҵ�һ�����ڵ㣬ʹnode��������
            */
            while(node==y->right){
                node=y;
                y=y->parent;
            }
            if(node->right!=y)//���岻��
                node=y;
        }
    }

    //����ʵ��--���أ�Ѱ��node��ǰ���ڵ�
    void decrement(){
        if(node->color==__rb_tree_red && node->parent->parent==node)
            node=node->right;
        else if(node->left!=0){
            base_ptr y=node->left;
            while(y->right!=0)
                y=y->right;
            node=y;
        }
        else {
            base_ptr y= node->parent;
            while(node==y->parent){
                node=y;
                y=y->parent;
            }
            node=y;
        }
    }
};

//RB_tree�ĵ����������캯���������ú������Լ��Լ�
template<class Value,class Ref,class Ptr>
struct __rb_tree_iterator:public __rb_tree_base_iterator{
    typedef Value value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef __rb_tree_iterator<Value,Value&,Value*> iterator;
    typedef __rb_tree_iterator<Value,const Value&,const Value*> const_iterator;
    typedef __rb_tree_iterator<Value,Ref,Ptr> self;
    typedef __rb_tree_node<Value>* link_type;

    //���캯��
    __rb_tree_iterator(){}
    __rb_tree_iterator(link_type x){node=x;}//�ýڵ㹹��
    __rb_tree_iterator(const iterator& it){node=it.node;}//����������������

    reference operator*()const{return link_type(node)->value_field;}//*itȡ�����ݵ�����

    #ifndef __SGI_STL_NO_ARROW_OPERATOR
        pointer operator->()const {return &(operator*());}//it->
    #endif // __SGI_STL_NO_ARROW_OPERATOR

    self& operator++(){increment();return *this;}

    self operator++(int){
        self tmp =*this;
        increment();
        return tmp;
    }

    self& operator--(){decrement();return *this;}

    self operator--(int){
        self tmp =*this;
        decrement();
        return tmp;
    }
};

//����������ݽṹ
template <class Key,class Value,class KeyOfValue,class Compare,class Alloc=alloc>
class rb_tree{

protected:
    typedef void* void_pointer;
    typedef __rb_tree_node_base* base_ptr;
    typedef __rb_tree_node<Value> rb_tree_node;
    typedef simple_alloc<rb_tree_node,Alloc> rb_tree_node_allocator;
    typedef __rb_tree_color_type color_type;

public:
    typedef Key key_value;
    typedef Value value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef rb_tree_node* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
protected:
    //����һ���ڵ���ڴ�
    link_type get_node(){return rb_tree_node_allocator::allocate();}

    //�ͷ�һ���ڵ�
    void put_node(link_type p){rb_tree_node_allocator::deallocate(p);}

    //����һ���ڵ㣬��x��ʼ�������ظýڵ�
    link_type create_node(const value_type& x){
        link_type tmp=get_node();
        __STL_TRY{
            construct(&tmp->value_field,x);
        }
        __STL_UNWIND(put_node(tmp));
        return tmp;
    }

    //��¡һ���ڵ㣬���ؿ�¡�Ľڵ�
    link_type clone_node(link_type x){
        link_type tmp=creat_node(x->value_field);
        tmp->color=x->color;
        tmp->left=0;
        tmp->right=0;
        return tmp;
    }

    //���ٽڵ�
    void destroy_node(link_type p){
        destroy(&p->value_field);
        put_node(p);
    }

protected:
    size_type node_count;
    link_type header;
    Compare key_compare;

    link_type& root()const {return (link_type&) header->paerent;}
    link_type& leftmost() const {return (link_type&) header->left; }
    link_type& rightmost(0const){return (link_type&) header->right;}

    //ȡ��x�ڵ�ĳ�Ա
    static link_type& left(link_type x){return (link_type&) (x->left);}
    static link_type& right(link_type x){return (link_type&) (x->right);}
    static link_type& parent(link_type x){return (link_type&) (x->parent);}

    static reference value(link_type& x){return x->value_field;}
    static const Key& key(link_type x){return KeyOfValue()(value(x));}
    static color_type& color(link_type x){return (color_type&)(x->color);}

    //ȡ��x�ڵ�ĳ�Ա(���base_node)
    static link_type& left(base_ptr x){return (link_type&) (x->left);}
    static link_type& right(base_ptr x){return (link_type&) (x->right);}
    static link_type& parent(base_ptr x){return (link_type&) (x->parent);}

    static reference value(base_ptr& x){return x->value_field;}
    static const Key& key(base_ptr x){return KeyOfValue()(value(x));}
    static color_type& color(base_ptr x){return (color_type&)(x->color);}


    //ȡ�����ֵ����Сֵ������node_base�ڵ�ĳ�Ա����
    static link_type minimum(link_type x){return (link_type) __rb_tree_node_base::minmum(x);}
    static link_type maximum(link_type x){return (link_type) __rb_tree_node_base::maximum(x);}

public:
    typedef __rb_tree_iterator<value_type,reference,pointer> iterator;

private :
    iterator __insert(base_ptr x,base_ptr y,const value_type& v);
    link_type __copy(link_type x,link_type p);
    void __erase(link_type x);

    void init(){
        header =get_node();//����һ���ڵ�
        color(header)=__rb_tree_red;//�ڵ���ɫΪ��

        root()=0;//��Ϊ0�����ҽڵ�Ϊheader�Լ�
        leftmost()=header;
        rightmost()=header;
    }

public:
    rb_tree(const Compare& comp=Compare()):node_count(0),key_compare(comp){inir();}

    ~rb_tree(){
    clear();
    put_node(header);
    }

    rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& operater=(const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& x);

public:
    Compare key_comp()const{return key_compare;}

    iterator begin(){return leftmost();}

    iterator end(){return header;}

    bool empty()const {return node_count==0;}

    size_type size()const {return node_count;}

    size_type max_size()const {return size_type(-1);}

public:
    pair<iterator,bool> insert_unique(const value_type& x);

    iterator insert_equal(const value_type& x);
};


//Ԫ�صĲ���
template <class Key,class Value,class keyOfValue,class Compare ,class Alloc>
typename rb_tree<Key,Value,Compare,Alloc>::iterator//��һ���Ƿ���ֵ
rb_tree<Key,Value,Compare,Alloc>::insert_equal(const Value& v){
    link_type y=header;
    link_type x=root();
    while(x!=0){
        y=x;
        x=key_compare(KeyOfValue()(v),key(x))?left(x):right(x);
    }
    return __insert(x,y,v);
}

//���뺯��������ֵv���뵽���ʵ�λ��
template <class Key,class Value,class keyOfValue,class Compare ,class Alloc>
pair<typename rb_tree< Key,Value, keyOfValue, Compare,Alloc>::iterator,bool>//��һ���Ǻ�������ֵ��pairģ����Ҫһ����������boolģ�����
rb_tree<Key,Value, keyOfValue, Compare,Alloc>:://��һ��ָ��������
insert_unique(const Value& v){//������
    link_type y=header;
    link_type x=root();
    while(x!=0){//ѭ����ֱ��x��λ��һ�������������еĽڵ㣬Ҳ�����ҵ��˲���㡣
        y=x;
        comp=key_compare(KeyOfValue()(v),key(x));//v��x��ֵ�Ƚϣ�x�󷵻�true��v�󷵻�false
        x=comp?left(x):right(x);//true����false����
    }

    iterator j=iterator(y);//��ʱ��yָ������ĸ��ڵ�
    if(comp)//�뿪ѭ��ʱ�����compΪtrue�������ʱ���ڵ����ڵ�
        if(j==begin())//������ڵ�������ڵ㣬�Ͳ���v��ƽ�⣻����j���ҵ����ڵ��ǰ���ڵ�
            return pair<iterator,bool>(__insert(x,y,v),true);
        else
            --j;
    if(key_compare(key(j.node),KeyOfValue()(v)))//���v����y��ǰ���ڵ�����y����ڵ�|v=y���Ͳ���v��ƽ��
        return pair<iterator,bool>(__insert(x,y,v),true);
    return pair<iterator,bool>(j,false);//����ʲô������
}

template <class Key,class Value,class KeyOfValue,class Compare,class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator//��һ���Ƿ�������
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(base_ptr x_,base_ptr y_,const Value& v){
    link_type x=(link_type)x_;
    link_type y=(link_type)y_;
    link_type z;

    if(y==header||x!=0||key_compare(KeyOfValue()(v),key(y))){
        z=create_node(v);
        left(y)=z;
        if(y==header){
            root()=z;
            rightmost()=z;
        }
        else if(y==leftmost())
            leftmost()=z;
    }
    else{
        z=create_node(v);
        right(y)=z;
        if(y==rightmost())
            rightmost()=z;
    }
    parent(z)=y;
    left(z)=0;
    right(z)=0;

    __rb_tree_rebalance(z,header->parent);
    ++node_count;
    return iterator(z);
}


//find
template<class Key,class Value,class KeyOfValue,class Compare,class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator//��һ���Ƿ�������
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k){
    link_type y=header;
    link_type x=root();
    while(x!=0)
        if(!key_compare(Key(x),k)) y=x,x=left(x);
        else x=right(x);

    iterator j=iterator(y);
    return (j==end()||key_compare(k,key(j.node)))?end():j;
}



