#pragma once
/*
	利用traits classes表现类型信息，同时利用类模板特化去拟合基本类型的信息
	利用这些信息去创建变量，特化模板，匹配重载

	虽然名为traits classes，但是一般用struct去实现。
	它是一个类模板，完全不做任何事，仅仅只是鹦鹉学舌地把模板类型T的typedef重新声明为同一的名字
	如：
	template<class T>
	struct test_traits
	{
		typedef T::class_info_t class_info_t; // 将T的内部类型class_info_t声明为自己的内部类型class_info_t
	};
	于是约定：所有希望使用这个traits的自定义类都必须在内部声明class_info_t类型
	
	这有什么用呢？如果类按照约定都声明了class_info_t类型，那么直接在需要的地方用T::class_info_t
	不是一样可以达到目的吗？何必多一层嵌套

	然而凡是嵌套必须提高嗅觉，嵌套是用来拟合所有情况的
	例如如果T是int，显然它不存在内部类型int::class_info_t，对于所有基本类型也是如此
	traits的技术要求之一就是必须对自定义类型和基本类型有一致的表现
	traits作为嵌套，它是一个类模板，这意味着它可以偏特化
	于是对于int可以有以下特化：
	template<>
	struct test_traits<int>
	{
		typedef XXX class_info_t; // 对于int的特化，将XXX作为class_info_t
	};

	于是，可以大胆地使用test_traits<T>::class_info_t，例如用它创建变量，特化类模板，匹配函数重载

	一个设计良好的traits，可以应对各种基本类型，同时对于那些满足约定的自定义类，也能运作正常


	traits的使用：
		traits本质是获取到类型的typedef，这是类型。可以定义变量，也可以用来特化其他类模板，此外用得最多的是命中重载

	例如：
		struct info1{}; // 两个info类型，用于typedef
		struct info2{};
		class C1{ public: typedef info1 class_info_t; }; // 两个自定义类，分别声明info1和info2为class_info_t
		class C2{ public: typedef info2 class_info_t; };

		void func(..., info1); // 两个函数，仅最后一个参数不同，形成重载
		void func(..., info2); // info1和info2不需要参数名，是占位参数

		template<class T>
		void caller(T t)
		{
			func(..., test_traits<T>::class_info_t{}); 
		}

		于是对于caller，传入C1或C2的对象，根据traits提取的信息，会得到不同的类型，进而匹配不同的func重载
		而如果test_traits对int进行了特化，那么穿int也能正常运作
		于是对于自定义类和基本类型，caller都表现良好

	顺带一提：traits是重载的好帮手，对于匹配不同的分支，如果不同重载
	会是：
		if(typeid(test_traits<T>::class_info_t).name() == "info1")
		{
			// 版本1
		}
		else
		{
			// 版本2
		}

	然而，显然对于类的内部类型，在编译器就依据是确定的，而用if的判断会将这部分本可以提前确定的逻辑
	交给运行时去处理。编译时能做的事运行时才去做，徒增消耗

	可能会疑问：test_traits<T>::class_info_t{}创建了一个匿名对象，再去匹配函数重载
	不见得会比if更好吧。
	是的if很快，开销几乎没有，相比之下创建一个匿名对象可能不明智。
	但是已经说过，typdef已经在编译期就确定了，这个创建匿名对象的过程可能压根不会发生
	编译器会将这个过程优化掉，直接完成重载匹配

	题外话：
		对于ifelse来分支处理迭代器的advance
		if(迭代器是随机访问迭代器)
		{
			it += n;
		}
		else if(双向迭代器且n为负)
		{
			while(n++) --it;
		}
		else
		{
			while(n--) ++it;
		}
	咋看之下没有问题吧？但是实际上是完全错误的代码。
	对于运行时程序，它必须保证所有分支的正确性。即以上的写法对任何迭代器都必须满足+=、--、++，否则编译失败。
	对于非随机访问迭代器，虽然它永远不会走第一个分支
	但是编译器仍然会检查每个分支，所以会因为不支持+=而报错。
	所以需要换个思维方式，对于不同的类型，在一个函数里做分别处理，看似可行实则无用。
	既然C++已经提供了重载函数来处理不同类型的分支
	那么也就很容易想到traits和重载的合作。
	自此无需感慨前人的智慧，凭自己也能摸索出类似的解法

*/