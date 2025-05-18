#pragma once
/*
	隐式接口：基于表达式与必要函数
	编译期多态：早绑定，编译时，模板具象化导致不同的同名函数、重载函数完成代码解析，运行时同一模板类型永远执行相同的代码

	与之对应的面向对象的
	显式接口：基于虚函数
	运行期多态：晚绑定，运行时不同子类对虚函数的重写执行不同代码

	例：
		假设obj是T类型的对象的引用
		if( obj.size() > 10 && obj != someNastyObj)
		{
			T temp{obj};
			temp.normalize();
			temp.swap(obj);
		}

		对于面向对象，obj必须满足：size()、normalize()、swap()、!=重载
		为了实现多态，往往它们都是virtual，

		而对于模板，需要size()，但不必须是接口，只需要自身或者父类有size()成员
		size()的返回值也不一定是数值，可以是对象，该对象不一定重载了>int的操作符，也可以是具有非explicit的单参int构造
		obj也不一定重载了!=，可以是和someNastyObj具有和非explicit的单参构造

		甚至可能重载了&&是的表达式的含义完全不同

*/