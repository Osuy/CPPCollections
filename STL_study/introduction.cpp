/*
struct在C中只能包含变量；在C++中，struct可以含有方法和typedef
struct的成员默认public，class成员默认private
*/

//声明枚举类型。类似struct，需要先声明一个类型名
enum Enum {one,two,three,four};

//三种定义变量的方式
//先声明类型，后定义变量
enum Enum {one,two,three,four};
enum Enum m_enum;

//声明的同时定义变量
enum Enum {one,two,three,four}m_enum;

//省去声明类型名直接定义变量
enum {one,two,three,four}m_enum;

//变量名是一个指针，指向枚举变量所在的内存地址
//m_enum在底层上指向第一个成员，所以可以通过循环遍历
for()



