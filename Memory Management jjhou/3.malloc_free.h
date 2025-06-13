#pragma once

/*
	操作系统在在运行你的程序之后、进入 main之前
	从内核开始调用 mainCRTStartup 进行一系列准备后才调用 main
	sbh -small block heap 小块堆内存


	_heap_alloc_dbg(...) 
		增加调试信息的地方，对需要分配内存的大小进行调整，前面加上32字节的调试信息头
		后面加上4字节的无人区（调试信息头的最后4字节也是无人区）
		无人区写入0xfd，在debug模式下，如果访问了无人区，就会被识别出未定义行为
		header的其他地方也会写入预定义的内容，其中给程序 用的内存会填入0xcd
		这个值如果打印会是 "屯"
		之后调用 _heap_alloc_base(size_t size)

	_heap_alloc_base(size_t size)
		小区块优化，当size小于1016时调用 _sbh_alloc_block(size)
		否则调用 HeapAlloc
		1016是因为还未记入首尾cookie，加入首尾cookie后就是1024
		调用 __sbh_alloc_block

	__sbh_alloc_block(...)
		将size+8，再上调为16的倍数（((size + 8)+ 15) & ~15）
		由于 上调为16的倍数，最后4位便空了出来
		sbh用最后一位的1或0表示是否被使用，如果程序还在使用，最后一位为1
		否则已经归还给sbh了，则最后一位为0，由sbh的其他 算法决定何时归还给操作系统
*/