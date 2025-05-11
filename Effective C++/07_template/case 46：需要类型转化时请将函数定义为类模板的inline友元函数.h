#pragma once
/*
	类模板的inline友元函数

	为了避开函数模板参数推导，需要将函数的声明写在类模板内部，借用类模板具象化确定参数，避开函数模板参数推导
	为了以no-member-function的方式实现，需要用friend修饰（不friend就是成员函数了）
	为了能链接成功，需要inline

	这是case 26的模板延续
	回顾case 26，no-member-function实现那些需要其他类型隐式转化this的函数
	因为
	例如左乘运算符：
		对于Class1的的乘法，通常是Class1自己乘以自己类型的对象，但是如果Class1支持其他类型的隐式构造（假设int）
		那么随之而来的是Class1和int的混合乘法。这点对于右乘，也就是成员*重载很简单，
		因为Class1支持int隐式构造，所以在右乘是个int时，会自动构造匿名的Class1对象并调用*操作符
		但假如是一个左乘，显然int没有对应的operator*，应该说没有任何operator*
		于是就需要定义一个no-member版本的operator*，两个参数都是const CLass1&
		（无需前int，后const Class1&，因为左乘int也可以被隐式转换为Class1）

	即对于operator*，或者其他类型的函数、操作符，如果参数都允许隐式转化，那么应该将其定义为no-member-function
	换个说法，operator*为了支持混合运算，两个参数都允许隐式转换，但是member版不能支持，所以只能用no-member版

	
	回归正题
	对于一个类模板，Class2<T>，同case 26一样，希望实现混合乘法运算
	有如下no-member

	template<class T>
	const Class2<T> operator*(const Class2<T>&, const Class2<T>&);

	然而这不能通过编译，因为这是一个函数模板，需要根据函数的参数进行模板参数推导
	当参数为Class2<int>时，可以正确地推导出T为int
	但是既然要混合乘法，那么当参数为int时，编译匹配不了因此推导失败
	int不能隐式转化为Class2<int>再推导吗？
	函数模板的参数推导是不会考虑隐式转换的


	有一个方法可以不让编译器推导函数模板，就是借用类模板具象化，在类模板内声明函数，然后为了是no-member，需要声明为友元friend
	参数推导只有函数模板才能做到，类模板不会使用参数推导，而是通过具象化来确定函数的参数。
	类模板的所有成员函数，在类模板具象化后，且该成员函数被调用时，就会具象化出确定的版本，无需参数推导

	因此只需在Class2里加上一个友元：
	template<class T>
	class Class2
	{
		...
		friend const Class2 operator*(const Class2&, const Class2&); // Class2具象化时，此友元函数的参数也一并确定，无需参数推导
		...
	}

	但是还有一个问题，就是这个friend函数始终会链接失败。原因我不知道
	不过解决办法非常简单，inline。
	inline函数，将声明和定义写在一起，就不会链接失败。这是解决链接失败的杀器
	但是如果这个函数很庞大，就需要借助另一个函数实现逻辑，而friend负责调用，总而言之需要inline

*/