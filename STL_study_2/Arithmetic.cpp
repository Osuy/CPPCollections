/*
	算术类仿函数，主要包括
	加法、减法、乘法、除法、取负、取余
	配合STL算法使用，不是给程序员自己用
*/

template<class T>
struct plus :public binary_function<T, T, T> {
	T operator()(const T& x, const T&y)const {
		return x + y;//对复杂类型，需要重载+运算符,下同
	}
};

template<class T>
struct minus :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const {
		return x - y;
	}
};

template<class T>
struct multiplies :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const {
		return x * y;
	}
};

template<class T>
struct divides :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const {
		return x / y;
	}
};

template<class T>
struct modulus :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const {
		return x % y;
	}
};

template<class T>
struct multiplies :public unary_function<T, T> {
	T operator()(const T& x)const {
		return -x;
	}
};

void text() {
	//两种调用方式

	//定义函数对象，用对象名调用
	plus<int> p;
	int result=p(1, 3);

	//用匿名对象调用
	//两个括号，第一个括号是声明了匿名对象，第二个括号调用的()重载
	//运行到分号，匿名对象释放
	plus<int>()(1, 3);
}