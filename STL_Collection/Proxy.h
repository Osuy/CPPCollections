#pragma once

/*
	代理模式
	间接层解耦思想
	对于一个十分复杂的对象，为其他对象提供一个代理以访问这个对象。

	创建一个基类，定义虚函数func
	imp类派生自基类，实现了func的功能，非常复杂
	proxy类派生自基类，同时与imp对象有关联（不一定是包含指针，可能是网络或线程）
		proxy实现了func，但是内部是转调了imp对象的func，
		只不过同时还做了很多工作，为imp功能的运行做铺垫
	因为func的参数都是一样的，所以proxy的代理保留了大部分透明度
	只损失了实现的具体细节

	具体proxy的铺垫工作，在实际的程序设计中常常也是复用的，多态的，就需要应用其他设计模式
*/