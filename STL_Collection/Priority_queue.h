#pragma once

template <class T,class Sequence = vector<T>,class Compare = less<typename Sequence::value_type>>
class priority_queue
{
public:
	//同queue一样，没有迭代器，也没有大多数容器的写操作。
	typedef typename Sequence::value_type value_type;
	typedef typename Sequenve::size_type size_type;
	typedef typename Sequenve::reference reference;
	typedef typename Sequenve::const_reference const_reference;
protected:
	Sequence c;
	Compare comp;
public:
	priority_queue() :c() {}
	explicit priority_queue(const Compare& x) :c(), comp(x) {}//显式构造
	
	//泛化构造，传入区间并构造出vector，然后在函数体内make_heap
	template<class InputIterator>
	priority_queue(InputIterator first, InputIterator last, const Compare& x) : c(first, last) { make_heap(c.begin(), c.end(), comp); }

	//同上，省略了comp
	template<class InputIterator>
	priority_queue(InputIterator first, InputIterator last) : c(first, last) { make_heap(c.begin(), c.end()); }

	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }
	const_reference top()const { return c.front(); }
	void push(const value_type& x) 
	{
		__STL_TRY
		{
			//放在vector尾部，然后用push_heap来对区间做上溯
			c.push_back(x);
			push_heap(c.begin(), c.end(), comp);
		}
		__STL_UNWIND(c.clear());
	}

	void pop()
	{
		__STL_TRY
		{
			//pop_heap将最大值放在最后一个元素的位置，然后用vector的pop_back移除
			pop_heap(c.begin(),c.end(),comp);
			c.pop_back();
		}
		__STL_UNWIND(c.clear());
	}


};