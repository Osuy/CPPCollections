#pragma once

/*
	condition_variable提供了阻塞和唤醒所在线程的功能

	阻塞：wait
		参数：
			1.mutex lock
			2.bool pred（可选）
		功能：
			一般而言，本线程在执行前，会先上锁，确保临界资源的安全
			然后，因为某些资源缺失而不能继续执行
			于是调用condition_variable的wait函数，将自己阻塞
			需要将锁变量传给wait，wait会自动将锁解开，以便其他线程需要访问临界资源
			直到其他线程调用了condition_variable的notify函数，唤醒了本线程
			本线程被唤醒之后，wait会自动上锁，然后继续执行接下来的代码

			关于pred参数，用于灵活地确定线程地阻塞与激活。
			调用wait且pred为false时，线程才会阻塞
			且当唤醒且pred为true时，线程才会激活。


*/