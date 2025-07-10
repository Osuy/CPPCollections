#pragma once
#include <mutex>

/*
	一个线程等待另一个线程计算出结果后才继续
	这并非join汇合。
	另一个线程计算结果，并不代表它要结束。它依然继续运行，持续不断地计算
	而本线程也持续运行，只是在某些步骤停顿，等待另一线程的结果
	这样的过程持续发生，相当于两个线程在合作运行

	让两个线程合作并不难设计。由于它们需要交互数据，所以可以另数据为全局变量。
	再用另一个变量标记数据是否可用
	当然由于多线程访问，所以需要互斥保护
*/

std::mutex mtx;
int some_data;
bool usable = false;
void set_some_data(int sd)
{
	std::unique_lock lk{ mtx };
	usable = true;
	some_data = sd;
}

void use_some_data()
{
	while (!usable);
	std::unique_lock lk{ mtx };
	usable = false;
	some_data;// 使用some_data
}

/*
	线程1持续不断地访问 usable
	当线程2设置好了some_data后，线程1会立即锁住互斥使用some_data

	如此做法的缺点在于：线程1对usable的不停地进行无意义的访问，反而使整体性能下降，达不到利用多线程提高性能的目的
	可以利用sleep让线程1的访问usable不那么频繁。但是无论sleep多久，都远远大于cpu时钟周期
	是程序“表现起来性能低下”
*/