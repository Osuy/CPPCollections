#pragma once

/*
	绝不覆盖继承而来的非虚函数
	替代方式为：
		1.改为虚函数
		2.改为非public继承
		3.不覆盖

	非虚函数是编译期静态绑定的
	即便使用一个父类指针持有子类对象，哪怕子类覆盖了父类的非虚函数，调用非虚函数仍然是父类的版本

*/