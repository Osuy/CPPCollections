/*
	概述：
	vector和数组很像，因为它使用的是一段连续的内存
	重载了[]操作符，从而行为很像数组
	差异：
	数组在声明时被分配内存，之后便无法改变；
	如果希望扩充，就要再声明一个更大的数组，把原数组的元素搬到新数组中，再释放原数组
	vector正是将上述繁琐的行为，以及一些其他功能封装好了，省去程序员造轮子的时间
	vector可以根据放入的元素自动扩展，扩展后再删除元素，容量并不会缩小

	优点：
	快速随机访问和尾部插入
	缺点：
	头部插入和删除需要移动其他元素，效率低
*/

//Vector的迭代器
/*
	因为原生指针所具有的功能正好就是迭代器所需要的功能，所以Vector的迭代器就是原生指针T*
	在Vector类中的内嵌声明如下：
	typedef value_type* iterator;
 */


 //Vector的数据结构
template<class T, class Alloc = alloc>
class vector {
public:
	//内嵌类型声明
	typedef T           value_type;
	typedef value_type* pointer;
	typedef value_type* iterator;           //元素的迭代器，本质是原生指针
	typedef value_type& reference;          //元素的引用，本质是原生引用
	typedef size_t      size_type;          //无符号整数，容器的尺寸的单位，不能完全用uint代替
	typedef ptrdiff_t   difference_type;    //带符号整数，两个迭代器之差的单位，不能用int替代

protected:
	//配置器类型声明
	typedef simple_alloc<value_type, Alloc> data_allocator;

protected:
	iterator start;//指向第一个元素
	iterator finish;//指向最后一个元素的下一个地址,容器满时等于end_of_storge
	iterator end_of_storge;//指向容器末端的下一个地址

public:

	//构造与析构
	vector() :start(0), finish(0), end_of_storge(0) {}

	vector(size_type n, const T& value) { fill_initialize(n, value); }

	vector(int n, const T& value) { fill_initialize(n, value); }

	vector(long n, const T& value) { fill_initialize(n, value); }

	explicit  vector(size_type n) { fill_initialize(n, T()); }//取消单参构造的隐式类型转换，如：vector<int> v='a'将会编译不通过

	~vector() { destroy(start, finish); deallocate(); }

	//工具函数
	iterator  begin() { return start; }
	iterator  end() { return finish; }
	size_type size()const { return size_type(end() - begin()); }
	size_type capacity()const { return size_type(end_of_storge - begin()); }
	bool      empty()const { return begin() == end(); }

	//以下两个函数和begin()、end()区别：前者返回元素的引用，可以直接使用；后者返回的是迭代器（指针），需要提领
	reference front() { return *begin(); }
	reference back() { return *(end() - 1); }

	//重载[]操作符，使vector容器能够像数组一样用下标访问成员
	reference operator[](size_type n) { return *(begin() + n); }

	//分容器已满和未满两种情况，未满直接在finish处添加然后自加；已满则用insert_aux来插入
	void push_back(const T& x) {
		if (finish != end_of_storge) {
			construct(finish, x);
			++finish;
		}
		else
			insert_aux(end(), x);
	}

	void pop_back() {
		--finish;
		destroy(finish);
	}

	//删除当前下标的元素，后面元素往前移，返回新的当前下表的元素
	iterator erase(iterator position) {
		if (position + 1 != end())
			copy(position + 1, finish, position);//copy为全局函数，接受迭代器
		--finish;
		destroy(finish);
		return position;
	}

	void resize(size_type new_size, const T& x) {
		if (new_size < size())
			erase(begin() + new_size, end());
		else
			insert(end(), new_size - size(), x);
	}

	void resize(size_type new_size) { resize(new_size, T()); }

	void clear() { erase(begin(), end()); }

protected:
	//内部工具函数
	void insert_aux(iterator position, const T& x);
	void insert(iterator position, size_type n, const T& x);

	//壳函数，调用元素配置器的deallocate，以start为起点，释放长度为(end_of_storge - start)的地址
	void deallocate() {
		if (start)
			data_allocator::deallocate(start, end_of_storge - start);
	}

	//用于构造，配置n个元素的空间，并用value填充
	void fill_initialize(size_type n, const T& value) {
		start = allocate_and_fill(n, value);
		finish = start + n;
		end_of_storge = finish;
	}

	//配置空间并填满内容
	iterator allocate_and_fill(size_type n, const T& x) {
		iterator result = data_allocator::allocate(n);
		uninitialized_fill_n(result, n, x);
		return result;
	}
};

template<class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const T& x) {//这里position一般都是end(),x是新插入但插不进的元素
	if (finish != end_of_storge) {
		construct(finish, *(finish - 1));
		++finish;
		copy_backward(position, finish - 2, finish - 1);//后移
		//用x的拷贝来赋值
		T x_copy = x;
		*position = x_copy;
	}
	else {
		//如果原大小为0，配置1，如果不为0，配置原来的两倍
		const size_type old_size = size();
		const size_type len = old_size != 0 ? 2 * old_size : 1;

		//new_start和new_finish都先指向新内存的起始地址，
		iterator new_start = data_allocator::allocate(len);
		iterator new_finish = new_start;

		try {
			//将原来vector的内容拷贝到new_start，返回末尾位置给new_finish
			new_finish = uninitialized_copy(start, position, new_start);
			//用新元素给new_finish赋值，然后new_finish后移
			construct(new_finish, x);
			++new_finish;

			//如果position不是end(),那么要把[position,finish)区间的元素也拷贝过来
			new_finish = uninitialized_copy(position, finish, new_finish);
		}
		catch (...) {
			//扩展失败就取消分配的内存
			destroy(new_start, new_finish);
			data_allocator::deallocate(new_start, len);
			throw;
		}

		//析构并释放原来的vector
		destroy(begin(), end());
		deallocate();

		//调整迭代器
		statr = new_start;
		finish = new_finish;
		end_of_storge = new_start + len;
	}
}


template<class T, class Alloc>
void vector<T, Alloc>::insert(iterator position, size_type n, const T& x) {//在position插入n个x
	if (n != 0) {//n==0什么都不做
		//剩余空间比n大
		if (size_type(end_of_storge - finish) >= n) {
			T x_copy = x;
			const size_type elems_after = finish - position;
			iterator old_finish = finish;
			if (elems_after > n) {
				uninitialized_copy(finish - n, finish, finish);
				finish += n;
				copy_backward(position, old_position - n, old_finish);
				fill(position, position + n, x_copy);
			}
			else {
				uninitialized_fill_n(finish, n - elems_after, x_copy);
				finish += n - elems_after;
				uninitialized_copy(position, old_finish, finish);
				finish += elems_after;
				fill(position, old_finish, x_copy);
			}
		}
		else {
			const size_type old_size = size();
			const size_type len = old_size + max(old_size + size());

			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;

			__STL_TRY{
				new_finish = uninitialized_copy(start,position,new_start);
				new_finish = uninitialized_fill_n(new_finish, n, x);
				new_finish = uninitialized_copy(position, finish, new_finish);
			}
#ifdef __STL_USE_EXCEPTIONS
				catch (...) {
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, len);
				throw;
			}
#endif
			destroy(start, finish);
			deallocate();
			start = new_start;
			finish = new_finish;
			end_of_storge = new_start + len;
		}
	}
}


