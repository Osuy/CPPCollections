#pragma once
/*
	case 13:
		1.获取资源的时机就是初始化对象的时机RAII（new与资源管理类的构造最好结合成一句，也即case17）
		如果一个函数返回T*，并存入shared_ptr<T>，不如直接让函数范围shared_ptr<T>
		因为它返回了T*资源，意味着它内部有new，而new应该被立刻置于资源管理内中

		2.资源管理类的原理是离开作用域自动调用对象的析构函数
			象的析构函数是自动的，一定发生的。delete语句移动到类的析构函数中，使资源能够自动释放
			手动delete可能会被未知的异常和新增return语句落空而使内存泄露

		3.只能指针无法管理数组
			std的所有指针类：aut_ptr、shared_ptr、unique_ptr，其析构语句都是delete而非delete[]
			因此不要拿它们去持有动态对象数组。stl没有对应的智能指针，因为这种需求总是能被vector和string替代
			如真需要，请使用boost::scope_array 和 boost::shared_array
	

	case 14：留意资源的复制
		确认复制行为以及希望的结果到底如何
			复制资源：深拷贝。new一个新的资源
			复制引用：浅拷贝。复制引用，可以直接复制原生指针，可以复制支持拷贝的智能指针
				并非所有资源都是内存，可能存在其他类型的资源，并且有着不同的释放方式
				例如锁，通常会作为固有变量，因此无需在堆上创建。被引用时枷锁，无引用时解锁
				std::shared_ptr的默认释放方式是删除，对目标内存delete。但是它支持传入自定义删除器
				将解锁函数传入shared_ptr的构造
			禁用复制：弃置或private声明赋值。unique_ptr
			转移资源：移动赋值，使用move将复制对象变为右值


	case 15：资源管理类提供对原始资源的访问
		显式获取资源的get函数
		对指针型资源，重载operator*和operator->
		对成员资源，提供隐式类型转换operator T()


	case 16：使用相同形式的new和delete
		new和delete与new[]和delete[] 不可混用
		new[]和delete  只对数组的第一个元素调用析构
		new和delete[]  错误地以内存地址的前方的未知数据所谓数组长度而调用N次析构

		
	case 17：见case13.1


*/