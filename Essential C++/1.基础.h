#pragma once

/*
	1.2注释#include <iostream> 得到的错误： "cout" 未声明标识符
		注释 using namespace std;也得到相同错误

	1.3 main 改为 my_main 得到的错误： 无法链接的外部符号， MSVCTD.lib  int invoke_main() 引用了改符号
		说明我们的程序是起始于操作系统的一个lib，被其调用而开始运行的
*/