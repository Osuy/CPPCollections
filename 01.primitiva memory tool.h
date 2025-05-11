#pragma once
#include <corecrt_malloc.h>
/*
	本文档的内容关于：
		1.malloc 、free
		2.new 、new[]、new() 、delete 、delete[] 、delete()
		3.operator new 、::operator new
*/


/*
	malloc和free是C风格的内存分配、释放工具
	使用如下：
		void* p = malloc(512); //分配512字节内存，并返回void指针给p
		free(p); //释放p指向的内存

	malloc分配内存会带有cookie，而套用malloc的new也会带有
	cookie的大小一般是8字节，调试模式下还有32字节的调试信息
	因此，如果分配一百万个int，那就有八百万个字节用来放cooike，这是极大的浪费
	也是内存管理的目的：
		避免频繁使用malloc，造成大量cookie

	再谈free
		malloc分配内存后，尽管带有cookie和调试信息，但是返回的指针指向的还是可用的字节，即
			4字节 cookie头
			32字节 调试信息
			n字节 可用内存   <-- 返回的指针指向此地址
			xxx字节 padding
			4字节 cookie尾
		那么想要释放内存时，怎么办呢？free已经为我们做好了
		当free一个指针时，free函数会去寻找cookie，确定了这个指针所指的内存的整个内存后，再释放
		因此，free是会正确地释放cookie、调试信息和padding的
		
		对于数组，free数组内的一个指针，会将整个数组的内存释放掉
		因为整个数组共用一个cookie。当free某一个元素的指针时，free会向前向后查找cookie，
		并将头尾cookie包围的内存释放掉。如下
			int* arr = new int[3];
			free(arr+1);
			arr[0] = 1; // error。因为free(arr+1)，整个数组的内存都已经被释放了

*/

/*
	new\new[]\new()
	首先需要认识一下C++的new和delete的种类：
		1.new/delete关键字：是由程序员手动使用的，编译器会扩展代码，实际上是调用operator new/delete
		2.::operator new/delete：全局的内存分配、释放工具，底层仍然使用malloc和free
		3.operator new/delete：可重载的操作符，可以针对一个类重载new和delete，用以独特的分配和释放内存
		

	
	new和delete关键字是最常用的内存分配工具，由程序员手动调用，编译器将代码扩展
		new扩展：
		1.调用类重载的operator new或全局::operator new 分配类大小的字节
		2.将返回的void指针转型为对应类型的指针
		3.用指针调用构造函数

		delete扩展：
		1.用指针调用对象的析构函数（因为是指针调用，所以可以多态，父类指针可以调用子类的析构，前提是虚析构）
		2.调用operator delete释放指针所指的内存

		如果类重载了operator new/delete，但是还是希望使用全局new/delete，可以用::前缀修饰
		A a = ::new A(); //A重载了operator new，但是指定用全局new来分配
		::operator new和delete可重载，但是
		其基本类型和没有重载new的类的默认内存工具，因此不建议重载
		
		在new扩展的代码中，出现了用指针调用构造函数的情况
		这种操作只能是由编译器来做，程序员是不可以用指针调用构造函数的
		但如果想要这么做，C++也是允许的，那就是placement new
		另外，程序员是可以通过指针调用析构函数的，并且支持多态，因此
		如果存在继承，最好用虚析构，以避免内存泄露

	
	new、delete类内重载。它们的声明应当如下：
		void* operator new(size_t n); //申请n个字节的空间，返回void*指针
		void operator delete(void* ptr); //释放指针
		
		重载new可以有额外参数，那就是placement new


	new[]/delete[]
		对于分配和释放数组，new[]应该和delete[]配合使用
		
		对于单个变量
			使用malloc分配内存，首先会分配两个4字节的地址作为内存的头尾
			然后会在变量的前面分配32个字节，在变量的后面分配4个字节的调式信息（在release模式下没有）
			之后会在尾部填充使整个内存为16的倍数

			例如，分配8个字节的内存，实际分配是： 4+32+8+4+4 = 52 填充 -> 64字节
				而在release模式下，由于没有调试信息，实际分配为：4+8+4 = 16 恰好为16的倍数无需填充

		对于数组，
			所有元素顺序排列，同样有头尾字节、调试信息和填充字节
			除此之外，malloc会额外分配4个字节内存，用于记录元素的个数

			例如， 3个元素的数组，每个元素字节为8，则实际分配： 4+32+4+8*3+4+4 = 72 填充 -> 80
				而在release哦是下，没有调试信息，实际分配为： 4+4+8*3+4 = 36 填充 -> 48

		new[]会分配 4+n个对象大小的内存，因为除了n个对象，还需一个unsigned int 来记录n的个数
		这个计数会在delete[]时，n次调用对象的析构函数

		因为用delete去释放一个数组时，会认为对象只是一个变量，因而只会调用一次析构函数
		如果对象持有指针，那么就可能造成内存泄露（没有指针成员不会泄露）
		以上时一种典型的内存泄露
		而delete[]会逐个对数组的每个元素都调用析构


	new()/delete()，也称placement new
		定位new/delete是一个偏门的工具，它也是类内的new/delete重载，只不过在会有额外的参数
		但是第一参数必须是size_t
		例如
			void* operator new(size_t, int);
			void* operator new(size_t, long);
			void* operator new(size_t, int, long);

		额外的参数要怎么传递给new呢？毕竟在印象里，A* p = new A(...); 参数都是传给类的构造函数的
		对于以上三种new，有：
			int start = 1;
			A* p2 = new(2) A(...);
			A* p3 = new(2l) A(...);
			A* p4 = new(2, 2l) A(...);	
		即：new的额外参数写在new后缀的括号里

		这种用法有什么好处呢？
		常规的A* p = new A(...); 编译器会计算A类的大小，然后分配其大小的内存，最后返回void*
		但是可能会出现，希望除了分配A的大小，再额外分配一些内存的情况
		例如string，内部持有一个const char*指针。string的内存大小总是固定的，但是STL的开发者希望
		string在new时，申请额外的内存来放置const char*指向的字符串

		除了以上三个例子，其实额外参数还可以是一个（已分配好内存的）指针
		这就引出了一个new()的全新的用法：new()不仅可以额外分配一些内存，也可以完全不分配内存
			首先new会被扩展为：1.分配内存；2.类型转换；3.调用构造函数
			但是可能会出现一种情况：
				一个指针已经分配好内存，并且类型转换了，现在要做的就是调用构造函数
				但是程序员是没办法手动调用构造函数
				因此需要借用new()

			重载new()接受一个额外的指针，直接return这个指针，不做任何内存分配
			void* operator(size_t,void* p){ return p; }
			这样，利用new的扩展机制，就可以对额外指针调用构造函数
			使用如下：
				A* p = new A;	//默认构造函数
				new(p) A(...);	//对p调用其他构造函数

			stl的工具函数construct(...)就是利用placement new来手动调用类的构造函数

*/

//重载::operator new/delete
//不可以写在任何命名空间中，使用时前缀::
inline void* operator new(size_t size);
inline void operator delete(void* p);

//重载类内operatornew/delete
class Foo
{
public:
	//必须是静态函数，因为发生在对象构造之前
	static inline void* operator new(size_t size) { return malloc(size); }
	static inline void operator delete(void* p) { free(p); }

	static inline void* operator new[](size_t size) { return malloc(size); }
	static inline void operator delete[](void* p) { free(p); }

	//new()，除了size，还有额外参数，此处以void*为例
	static inline void* operator new(size_t size, void* p){ return p; }
	//与new()对应的delete()几乎不会调用，只有在构造抛出错误时，才会用到
	static inline void operator delete(void* p, void*) { free(p); }
};

