#include <iostream>
#include <vector>
using namespace std;

template<class T>
class base{
public:

    T x;
};

template<class T>
class Son:public base<T>{
public:

    T y;
    Son(T a,T b):y(b){
        typename base::x=a;
    }
};


void test(){
    Son<int> a(1,2);

    cout<<a.x<<a.y<<endl;
}

int main()
{
    test();
    return 0;
}
