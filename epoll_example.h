#pragma once

/*
#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
using namespace std;

#define MAXLINE 5
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 5000
#define INFTIM 1000

void setNonBlocking(int sock)
{
	int opts = fcntl(sock,F_GETFL);
	if(opts < 0)
	{
		perror("fcntl(sock,GETFL)");
		exit(1);
	}
	opts = opts | O_NONBLOCK;
	if(fcntl(sock,F_SETFL,opts) < 0)
	{
		perror("fcntl(sock,SETFL,opts)");
		exit(1);
	}
}

int main(int argc,char* argv[])
{
	int i, maxi, connfd, 
		sockfd, epfd, nfds, portnumer;

	int listenfd;//客户端套接字

	ssize_t n;
	char line[MAXLINE];
	socklen_t clilen;

	if(2==argc)
	{
		if((portnumber = atoi(argv[1]) < 0)
		{
			fprintf(stderr,"Usage:%s portnumber /a/n ",argv[0]);
			return 1;
		}
	}
	else
	{
		fprintf(stderr,"Usage:%s portnumber /a/n ",argv[0]);
			return 1;
	}

	//ev用于注册事件，events用于回传要处理的事件
	struct epoll_event ev,events[20];

	//创建epoll并记录描述符
	epfd = epoll_create(256);

	//创建两个地址结构体
	struct sockaddr_in clientaddr;
	struct sockaddr_in serveraddr;

	//初始化套接字
	listenfd = socket(AF_INET,SOCK_STREAM,0);

	//设置ev事件的相关
	ev.data.fd = listenfd;
	ev.events = EPOLLIN | EPOLLET;


	//注册ev
	//epfd：向哪个epoll执行操作
	//EPOLL_CTL_ADD：要执行的是注册，其他操作还有修改和删除
	//listenfd：需要监听哪一个套接字
	//要监听什么类型的事件
	epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

	//设置serveraddr
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;

	char *local_addr="127.0.0.1";
	inet_aton(local_addr,&(serveraddr.sin_addr));//或者serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1")

	serveraddr.sin_port = htons(portnumber);
	
	//绑定套接字和地址
	bind(listenfd,(sockaddr*)&severaddr, seizof(serveraddr));

	//监听套接字
	listen(listenfd,LISTENQ);

	maxi=0;

	while(1)
	{

	ndfs=epoll_wait(epfd,events,20,500);
	for(i=0;i<nfds;++i)
	{
		//如果event[i]关联的套接字
		if(event[i].data.fd == listenfd)
		{
			//accept接收客户端套接字
			connfd = accept(listenfd, (sockaddr*)&clientaddr,&clent);
			if(connfd < 0)
			{
				perror("connfd < 0");
				exit(1);
			}

			//str 获取客户端套接字的ip地址
			char* str = inet_ntoa(clientaddr.sin_addr);
			cout<<"accpet a connection from "<<str<<endl;

			//
			ev.data.fd=connfd;
			ev.events=EPOLLIN | EPOLLET;

			//注册ev
			epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
		}
		//如果是以及连接的客户端，并且收到数据
		else if(events[i].events & EPOLLIN)
		{
			cout<<"EPOLLIN"<<endl;
			//如果events[i]对应的套接字无效，则继续下一轮循环
			if((sockfd = events[i].data.fd) < 0)
				continue;

			//读取成功返回读取的数量
			//读取到结尾返回0
			//读取数据失败返回-1
			if((n=read(sockfd,line,MAXLINE) < 0)
			{
				//?
				if(errno == ECONNRESET)
				{
					close(sockfd);
					events[i].data.fd = -1;
				}
			}
			else if(n==0)
			{
				close(sockfd);
				events.data.fd = -1;
			}

			//在line数组的末尾添加终结符，然后输出
			line[n] = '/0';
			cout<<"read"<<line<<endl;

			//
			ev.data.fd = sockfd;
			ev.events = EPOLLOUT|EPOLLET;

		}
		//如果有数据发送
		else if(events[i].events&EPOLLOUT)
		{
			sockfd=events[i].data.fd;
			write(sockfd,line,n);

			ev.data.fd=sockfd;
			ev.events=EPOLLIN|EPOLLET;
			epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
		}
	}
	}
	return 0;
}

*/