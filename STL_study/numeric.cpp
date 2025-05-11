/*���ܺ�
    accumulatey��������[first,last)������Ԫ�ص��ܺ͡�
    ע����Ҫһ��init��ʼֵ����Ϊϣ����ʹ����Ϊ�գ�Ҳ�ܷ���һ����Ч��ֵ���������init���Խ�����Ϊ0
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



/*������Ԫ�صĲ�
    adjacent_difference��[first,last)��������Ԫ�صĲ
    ��ÿ���������result��������ָ������
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



/*������������ڻ�
    ��[first1,last1)�����[first2,first2+(first1-last1)������ĳɶ�Ԫ�صĻ����ܺ�
    ͬ��Ϊ�˷���һ����Чֵ������init�ĳ�ֵ
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



/*�󲿷ֺ�
    partial_sum�൱��adjacent_difference�ķ�������������Ԫ��֮��
    �������v1��adjacent_difference����󸳸�v2��v2��partial_sum����󸳸�v3��v1��v3��Ԫ�ص������ȵ�
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
    ����
*/

//power-1
template <class T,class Integer>
inline T power(T x,Integer n){
    return power(x, n,multiplise<T>());
}

//power-2������
/*
��ͳ��������
һ��ѭ����while(n!=0){temp*=x;n--;} //��ҪƵ����ֵ������Ч�ʵ�
����ѭ����x*=x;��һ��ѭ��x^2���ڶ���ѭ��x^4��������ѭ��x^8�����Ĵ�ѭ��x^16���Դ�����

��2���Ƶ��ӽ�ȥ���������㣺
һ��ʮ������ת��Ϊ����������144=10010000�����ö����������ú�����(��)������ơ�
nÿ����(i+1)�Σ�x��ͨ��x*=x�õ���x=_x^i��
���������Ƶ����λ��1���Ͱ�x�˸�result��result*=x��
���������Ƶ����Ϊ��0���ͼ����ۼ�xΪ_x�Ķ��ٴΡ�


���n�Ķ������ǣ�16=10000������ĩβΪ0�������ͽ������棨�������ַ���ֱ��x=x^(2^i)
16=10000�����ĸ�0����ʱi=4��x=x^(2^i)����Ľ����x^16��
��������(i+1)=5�Σ����ж��Ƿ�Ϊ0����Ϊ16=10000������ε���0�����Լ������
������144=10010000��������κ�ʣ��100����Ҫѭ�������£�
x=x*x��������Ϊ1��result=result*x��������λΪ0���ͽ�n��������
n=144=10010000������5�κ�n=100��x=_x^16��
����ѭ��������x=x*x=_x^32�����λΪ0�����ƣ�n=10��
��x=x*x=_x^64�����λΪ0�����ƣ�n=1��
��x=x*x=_x^128�����λΪ1��result=result*x=result*_x^128=_x*144;
*/
template <class T class Integer ,class MonoidOperation>
T power(T x,Integer n,MonoidOperation op){//������������ѭ����ʵ���Ͽ���д��һ�𣬵�Ϊ�˱���n=1000000000000����������0�����Ƶ���ж�if�������ֳ�����ѭ��
    if(n==0)
        return identity_element(op);
    else{
        while((n&1)==0){//n�����λΪ1ʱ�Ƴ�ѭ��
            n>>=1;
            x=op(x,x);
        }
        T result =x;//resultȡ�õ�ǰ��x����
        n>>=1;
        while(n!=0){//n�������ƣ�����1�Ͱ�x�˸�result
            x=op(x,x);
            if((n&1)!=0)
                result=op(result,x);
            n>>1;
        }
        return result;
    }
}





