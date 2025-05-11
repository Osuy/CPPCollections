#pragma once
#include <iosfwd>
#include <mutex>

/*
	
*/

namespace case29
{
/*
	观察以下代码
	一个支持异步更换背景的窗口类
*/
class Image
{
public:
	Image(std::istream& imgSrc) {}
};
class PrettyMenu
{
public:
	// ...
	void changeBackground(std::istream& imgSrc);
	// ... 

private:
	std::mutex mutex;
	class Image* bgImage = nullptr;
	int imageChanges = 0;
};

void PrettyMenu::changeBackground(std::istream& imgSrc)
{
	mutex.lock();

	if (bgImage)delete bgImage;
	
	++imageChanges;

	bgImage = new Image(imgSrc);

	mutex.unlock();
}
/*
	它因没有考虑new异常而存在以下问题：
		1.mutex没有解锁
		2.bgImage持有的资源已经释放调且未置空
		3.bgImage和imageChanges无法恢复

	关于mutex的解锁问题，参考case13，应使用资源管理类来自动加锁解锁，那样即使抛异常也能保持正常
	关于bgImage，关键在于delete要发生在new的后面，可以移动语句，也可以考虑改为智能指针
	bgImage.reset(new Image{imgSrc});
	由于调用reset之前必须对参数求值，所以它发生在new的后面，一旦new失败，reset不会被调用，也就不会delete旧指针

	异常安全的三种保证：
		1.基本保证。保证发生异常后，程序可以保持合法状态，不会泄露、死锁或崩溃
		2.强烈保证。保证要么执行成功，要么滚回到调用函数前的状态
		3.无异常保证。保证绝不抛出异常

	强烈保证的方法：copy-and-swap
		对某一对象操作而又希望提供强烈保证，可以拷贝一个副本，在其上操作
		如果所有操作通过，将其swap回原对象上
		达到了“要么全对，要么全错”的效果

	异常安全的木桶效应
	考虑以下代码
*/

void f1();
void f2();
void doSomeThings()
{
	// ...
	f1();
	// ... 
	f2();
	// ...
}
/*
	doSomeThings的异常安全级别受f1和f2的影响
	如果f1提供了基本保证，那么doSomeThings也只有基本保证。f同理
	但是f1和f2都提供了强烈保证，那doSomeThings是否就达到强烈保证级别了呢？
	不一定。
	因为f2的强烈保证，只能使姿态回到f1调用之后，而不能回到doSomeThing调用之后
	f1成功所造成的一切修改，即便f2失败也不会滚回，所以整体依然只有基本保证

	对于一个系统而言，所有函数都强烈保证仍然不能使整个体系达到强烈保证
*/
}