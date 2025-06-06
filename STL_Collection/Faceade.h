#pragma once

/*
	门面模式
	主要体现了间接层思想
	软件内部存在若干个子系统，外部存在若干个用户类。子系统负责处理软件逻辑，用户类负责处理用户的请求
	假如子系统和用户类的关系是耦合的，用户类直接访问子系统
	那么当子系统发生变化时，用户类极有可能连带地被改变。
		例如一个mysql子系统替换成redis子系统时，用户类对数据库的访问手段就要更改

	门面模式的目的是消除子系统与用户类的紧耦合关系，也就是将两者的直接关联改为间接关联。
	为此创建Facade类，专门用于处理所有用户类访问所有子系统的逻辑
	这样，子系统变更就不会影响到用户类。

*/