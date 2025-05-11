/*求总和
    accumulatey用来计算[first,last)区间中元素的总和。
    注意需要一个init初始值。因为希望即使区间为空，也能返回一个有效的值。如果不用init可以将其设为0
*/

//accumulate-1
template<class InputIterator, class T>
T accumulate(InputIterator first, InputIterator last, T init) {
    for(; first != last; ++first)
        init += *first;
    return init;
}

//accumulate-2
template<class InputIterator, class T, class BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op) {
    for(; first != last; ++first)
        init = binary_op(init, *first);
    return init;
}



/*求相邻元素的差
    adjacent_difference求[first,last)区间相邻元素的差。
    将每个结果传给result迭代器所指的区间
*/

//adjacent_difference-1
template<class InputIterator, class OutputIterator >
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result) {
    if(first == last)return result;
    return __adjacent_difference(first, last, result, value_type(first));
}

template<class InputIterator, class OutputIterator, class T>
OutputIterator __adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, T*) {
    T value = *first;
    while(++first != last) {
        T tmp = *first;
        *++result = tmp - value;
        value = tmp;
    }
}

//adjacent_difference-2
template<class InputIterator, class OutputIterator, class BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op) {
    if(first == last)return result;
    *result = *first;
    return __adjacent_difference((first, last, result, value_type(first), binary_op);
}

       template <class InputIterator, class OutputIterator, class T, class BinaryOperation>
OutputIterator __adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, T*, BinaryOperation binary_op) {
    T value = *first;
    while(++first != last) {
        T tmp = *first;
        *++result = binary_op(tmp, value);
        value = tmp;
    }
    return ++result;
}



/*求两个区间的内积
    求[first1,last1)区间和[first2,first2+(first1-last1)）区间的成对元素的积的总和
    同样为了返回一个有效值可以设init的初值
*/

//inner_product-1
template<class InputIterator1, class InputIterator2, class T>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init) {
    for(; first1 != last1; first1++, first2++)
        init = init + (*first1**first2);
    return init;
}

//inner_product-2
template<class InputIterator, class InputIterator2, class T, class BinaryOperation1, class BinaryOperation2>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init, BinaryOperation1 binary_op1, BinaryOperation binary_op2) {
    for(; first1 != last1; first1++, first2++)
        init = binary_op1(init, binary_op2(*first1, *first2));
    return init;
}



/*求部分和
    partial_sum相当于adjacent_difference的反函数，求相邻元素之和
    如果容器v1做adjacent_difference运算后赋给v2，v2做partial_sum运算后赋给v3。v1和v3的元素的逐个相等的
*/

//partial_sum-1
template<class InputIterator, class OutputIterator >
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result) {
    if(first == last )return result;
    *result = *first;
    retusn __partial_sum(first, last, result, value, value_type(first));
}


template <class InputIterator, class OutputIterator, class T>
OutputIterator __partial_sum(InputIterator first, InputIterator last, OutputIterator result, T*) {
    T value = *first;
    while(++first != last) {
        value = value + *first;
        *++result = value;
    }
    return ++result;
}


//partial_sum-2
template <class InputIterator, class OutputIterator, class BinaryOperation>
OutputInterator partial_sum(InputInterator first, InputInterator last, OutputInterator result, BinaryOperation binary_op) {
    if(first == last)return result;
    *result = *first;
    return __partial_sum(first, last, result, value_type(first), binary_op);
}

template <class InputIterator, class OutputIterator, class T>
OutputIterator __partial_sum(InputIterator first, InputIterator last, OutputIterator result, T*, BinaryOperation binary_op) {
    T value = *first;
    while(++first != last) {
        value = binary_op(value, *first);
        *++result = value;
    }
    return ++result;
}


/*
    求幂
*/

//power-1
template <class T,class Integer>
inline T power(T x,Integer n){
    return power(x, n,multiplise<T>());
}

//power-2。较难
/*
传统的做法：
一、循环：while(n!=0){temp*=x;n--;} //需要频繁的值拷贝，效率低
二、循环：x*=x;第一次循环x^2，第二次循环x^4，第三次循环x^8，第四次循环x^16。以此类推

从2进制的视角去处理幂运算：
一个十进制数转化为二进制数：144=10010000。利用二进制数正好和上面(二)情况相似。
n每右移(i+1)次，x就通过x*=x得到，x=_x^i。
如果这次右移的最低位是1，就把x乘给result：result*=x；
如果这次右移的最低为是0，就继续累计x为_x的多少次。


如果n的二进制是：16=10000，这种末尾为0的数，就借用上面（二）的手法，直接x=x^(2^i)
16=10000，有四个0，此时i=4，x=x^(2^i)计算的结果是x^16；
于是右移(i+1)=5次，再判断是否不为0。因为16=10000右移五次等于0，所以计算完成
而对于144=10010000，右移五次后，剩下100，需要循环。如下：
x=x*x；如果最低为1，result=result*x；如果最低位为0，就将n继续右移
n=144=10010000。右移5次后n=100，x=_x^16；
进入循环，首先x=x*x=_x^32；最低位为0，右移，n=10；
再x=x*x=_x^64，最低位为0，右移，n=1；
再x=x*x=_x^128，最低位为1，result=result*x=result*_x^128=_x*144;
*/
template <class T class Integer ,class MonoidOperation>
T power(T x,Integer n,MonoidOperation op){//本函数有两个循环，实际上可以写在一起，但为了避免n=1000000000000这种连续的0过多而频繁判断if条件，分成两个循环
    if(n==0)
        return identity_element(op);
    else{
        while((n&1)==0){//n的最低位为1时推出循环
            n>>=1;
            x=op(x,x);
        }
        T result =x;//result取得当前的x的幂
        n>>=1;
        while(n!=0){//n继续右移，遇到1就把x乘给result
            x=op(x,x);
            if((n&1)!=0)
                result=op(result,x);
            n>>1;
        }
        return result;
    }
}





