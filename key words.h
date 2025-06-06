#pragma once
/*
	const
		修饰全局变量：分配在常量区，不可更改
		修饰局部变量：分配在栈区，不可直接更改，但是可以通过指针间接欸更改
		修饰成员函数：函数不能修改类的成员变量
		修饰函数参数：函数参数在函数体内不能被修改
		修饰函数返回值：函数返回一个常量
*/

/*
	static
		修饰全局变量、函数：该全局变量、函数只能在文件内访问
			普通全局变量和函数是可以通过添加头文件被其他文件访问的
			这也是静态变量和全局变量的区别
			全局变量和静态变量都是分配在静态区，一个外部可访问，一个仅内部可访问
		
		修饰局部变量
*/

