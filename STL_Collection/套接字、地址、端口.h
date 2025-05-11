#pragma once

/*
	套接字
		套接字关联地址和端口
		在不同操作系统中的实现不同：
		linux：文件描述符，有时写作fd，本质上是int，实际的数据由系统托管
		windows：句柄，SOCKET结构体

	套接字的创建：
		int test_sock = socket(int af,int type,int protocol)
			af：IP类型。AF_INET表示IPv4地址，AF_INET表示IPv6标志
			type：数据传输方式。SOCK_STREAM：流传输，SOCK_DGRAM：电报传输
			protocol：协议类型。IPPROTO_TCP：tcp协议；IPPROTO_UDP：udp协议
		创建套接字只是向系统申请了一个套接字，还不能使用
		使用前，还需与地址和端口绑定

	地址、端口的创建：
		首先需要创建地址端口对象。类型是sockaddr。
		更常用的方法是创建sockaddr_in对象。
		因为sockaddr的地址和端口全部存在一个char数组里，难以取用
		而sockaddr_in将二者分开，因此可以方便放开读取或修改。
		这两个结构体类型可以无损转换。
		例子：
			struct sockaddr_in test_addr;
			bzero(&test_sock,sizeof(test_addr));
			test_addr.sin_family = AF_INET;//ipv4族地址
			test_addr.sin_port = htos(800);//htos将800转化为网络字节
			test_addr.sin_addr.s_addr = inet_addr("192.168.1.0");//inet_addr将ip字符串转化为网络字节
		以上，一个地址端口结构体对象就创建、初始化完成

	套接字和地址端口的绑定：
		int bind(int sock, struct sockaddr* addr, socklen_t addrlen);
			sock：绑定的套接字
			sockaddr：端口地址结构体
			addrlen：地址长度
			return：
		绑定完成后，意味着本机这个端口的数据与该套接字关联。
		取出端口数据时，将存放到套接字的缓冲区
	
	监听端口（服务端）：
		int listen(int sock,int backlog);
			sock：要监听的端口
			backlog：请求队列数量。表示这个套接字接收到的新的连接的最大数量。
		套接字正在处理客户端请求时，新的请求将放在请求队列中

	建立连接（客户端）：
		int connect(int sock, struct sockaddr* serv_addr, socklen_t addrlen);
			sock：客户端上需要建立连接的套接字
			serv_addr：连接的服务器地址
			addrlen：地址长度
		建立连接后，若服务器的该地址端口也处于监听状态，则其accept函数由阻塞变为就绪
		accept函数将客户端的地址端口结构体写入参数，并返回套接字

	接受请求（服务端）：
		int/SOCKET accept(int sock,struct sockaddr* addr, socklen_t addrlen)
			sock：收到请求的套接字
			addr：存放客户端地址端口数据的结构体
			addrlen：地址长度
		accept在没有请求时阻塞，一旦受到请求变为就绪。
		从收到的请求里提取处客户端的地址和端口，写入addr
		然后返回一个客户端套接字
		服务器用这个套接字和read、write函数，可以实现接收和发送数据

	发送数据write（可双向）：
		ssize_t write(int fd, const void* buf, size_t nbytes);
			fd：需要发送数据的套接字
			buf：发送的数据的指针
			nbytes：发送的数据的长度
		该函数将buf中长度为nbytes的数据写入套接字的输出缓冲区，
		再由TCP协议将缓冲区内的数据发送出去
		如果发送失败，该函数返回-1

	接收数据read（可双向）：
		ssize_t read(int fd, void* buf, size_t nbytes);
			fd：接收数据的套接字
			buf：存放数据的地址指针
			nbytes：要读取多长的数据
		该函数从目标套接字的输入缓冲区读取长度为nbytes的数据
		并写入到buf。
		若成功，返回读取的长度
		若读到输入缓冲区的结尾，则返回0
		若读取失败，返回-1；

*/