/*
	stack是一种先进后出的结构，只有顶端可以访问。
	换言之，stack不支持遍历行为，故没有迭代器。

	STL的stack建立在序列容器的基础上，通过接口封装实现，默认为deque
*/

#include <deque>
template<class T,class Sequence = std::deque<T>>
class stack {
public:
	//内嵌类型声明
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;

protected:
	Sequence c;

public:
	bool empty()const { returen c.empty(); }

	size_type size()const { return c.size(); }

	reference top() { return c.back(); }

	const_reference top()const { return c.back(); }

	void push(const value_type& x) { c.push_back(x); }

	void pop() { c.pop_back(); }

private:
	//以下友元函数模板需要加上空模板参数列表<>，即宏 __STL_NULL_TMPL_ARGS，详情参考类模板的友元函数模板
	friend bool operator== __STL_NULL_TMPL_ARGS(const stack*, const stack&);
	friend bool operator<= __STL_NULL_TMPL_ARGS(const stack*, const stack&);
};

template <class T, class Sequence>
bool operator==(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
	return x.c == y.c;
}

template <class T, class Sequence>
bool operator<(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
	return x.c < y.c;
}