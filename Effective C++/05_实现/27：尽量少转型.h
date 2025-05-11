#pragma once
/*
	旧式转型
		T(expr)
		(T)expr

	新式转型
		const_cast<T>(expr) 移除const、mutable修饰
		dynamic_cast<T>(expr) 安全向下转型，唯一无法用旧式转型实现，也是性能开销最大的转型，它需要一一对比继承体系下的类名，调用strcmp
		reintepret_cast<T>(expr) 低级转型，如将指针转为整型，可以转型任意类型，不安全
		static_cast<T>(expr) 强制隐式转型，编译期检查，安全

	永远建议只使用新式转型，不仅因为新工具具有更好的性能，而且在排查错误方面，旧式转型往往和一堆表达式融为一体而难以察觉

	抛弃转型其实什么都没做的想法
	因为在多继承类族中，
		class Base1{...};
		class Base2{...};
		class Derived : public Base1 , public Base2 {...};
		Derived d;
		Base1* b1 = &d;
		Base2* b2 = &d;
	最后两句代码，子类地址赋给父类指针，是合法的，也是无误的
	但是显然，d的内存空间中Base1和Base2 的地址是不同的，因此编译器必然会在产生一些偏移的操作来使
	Base2*正确地指向了d的Base2的那部分地址
	而这d，也可以是外部传参进来的。换言之，这些偏移操作将被允许在运行时进行
	更要命的是，这种偏移依编译器决定，在不同平台将可能无法适用


	dynamic_cast 会根据类名比较，因此在继承类族中，继承深度每深一层，就会多一次strcmp
	

*/