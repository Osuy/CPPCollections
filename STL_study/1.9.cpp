#include <iostream>
using namespace std;

/*
组态3：如果编译器无法处理类模板静态成员static members of template classes
就定义__STL_STATIC_TEMPLATE_MEMBER_BUG

#if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ <8)
#	define  __STL_STATIC_TEMPLATE_MEMBER_BUG
*/

//如果编译器不支持静态类模板成员，就不能定义_data
template <class T>
class testClass1 {
public:
	static T _data;
};

/*
组态5：偏特化
#ifdef _PARTAL_SPECIALIZATION_OF_CLASSTEMPLATES
#	define __STL_CLASS_PARTAL_SPECIAALIZATION
偏特化：可以用参数列表对多个未定类型中的几个特别指定
指定类型的参数可以是其他未定类型，也可以是自身的指针（指针偏特化）
*/

//一般化设计，有两个未定类型
template<class I,class O>
struct testClass {
	testClass() { cout << "<I,O>" << endl; }
};

//偏特化设计：只有一个未定类型，另一个未定类型被指定为int
template <class T>
struct testClass<T,int>{
	testClass() {
		cout << "<T，int>" << endl;
	}
};
//当然，指定为T也是可以的
template <class T>
struct testClass<T,T> {
	testClass() {
		cout << "<T，T>" << endl;
	}
};

//指针偏特化:指定为自身的指针，如此就需要写全未定类型
template<class I, class O>
struct testClass<I*,O*> {
	testClass() { cout << "<I*,O*>" << endl; }
};

//const偏特化：指定为常量化类型
template<class I, class O>
struct testClass<I const, O const> {
	testClass() { cout << "<I const,O const>" << endl; }
};



//全特化设计：把全部的未定类型都指定。模板函数不能偏特化，只能全特化
template <>
struct testClass<int，char> {
	testClass() {
		cout << "<int,char>" << endl;
	}
};

/*

*/