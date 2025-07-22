#pragma once
#include <mutex>
#include <list>

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

/*
	4.4运用同步简化代码

	1.函数式编程
	考虑管道风格的巨大计算任务的处理，由于任务庞大，所以被拆为了各个部分
	然后交给管道中的多个部件去完成。每个部件的输出是上一个部件的输入
	缺点在于：1.可能存在有些不依赖其他数据就可以计算的独立数据，因此可以利用多线程
	2.切换部件相当于切换了上下文，因此状态必须保存于上下文以外，例如全局变量或类成员

	利用多线程同步，将计算任务交给多线程，并存留future，主函数只需在任务进行到某步时获取future的值即可
	所有逻辑在同一片上下文中，所需数据可以由参数传递而不依赖全局变量，更易理解和调试
	这种不依赖全局变量的单个函数，其 作用完全限制于它的返回值，而不会改动任何外部状态又称 纯pure函数
	纯函数是最易使用的函数。只要传入的参数相同，得到的结果也必然相同，不受外界影响

	已排序为例
*/

template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input)
{
	if (input.empty())
	{
		return input;
	}

	std::list<T> result;

	result.splice(result.begin(), input, input.begin()); // 将input的首元素移动到 result
	const T& pivot = *result.begin();
	auto divide_point = std::partition(input.begin(), input.end(), [&](const T& t) {return t < pivot;}); // 将小于pivot的元素整理到链表前面，返回后半部分的第一个元素的迭代器

	std::list<T> lower_part;
	lower_part.splice(lower_part.end(), input, input.begin(), divide_point);// 将前半部分移动给lp

	// 递归调用自身 
	// 将lp移动给sequential_quick_sort的参数，参数是值传递
	// 同时接受返回值构造一个新的链表，而sequential_quick_sort的返回值也是值传递的，自动移动
	// 整个过程没有发生任何拷贝
	auto new_lower(sequential_quick_sort(std::move(lower_part));

	// 对于input里剩下的后半部分做同样的操作
	auto new_higher(sequential_quick_sort(std::move(input));

	// 将前半部分移动到result前面，后半部分移动到result后面。（pivot此前已包含在result里）
	result.splice(result.begin(), new_lower);
	result.splice(result.end(), new_higher);

	return result;
}

/*
	注意到，在排序前半部分时并不依赖后半部分，所以是可以在主线程处理前半部分，同时另开一个线程处理后半部分
	主线程排序完成时，future.get取得后半部分的结果即可

*/