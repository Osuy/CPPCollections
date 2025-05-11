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

	int listenfd;//�ͻ����׽���

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

	//ev����ע���¼���events���ڻش�Ҫ������¼�
	struct epoll_event ev,events[20];

	//����epoll����¼������
	epfd = epoll_create(256);

	//����������ַ�ṹ��
	struct sockaddr_in clientaddr;
	struct sockaddr_in serveraddr;

	//��ʼ���׽���
	listenfd = socket(AF_INET,SOCK_STREAM,0);

	//����ev�¼������
	ev.data.fd = listenfd;
	ev.events = EPOLLIN | EPOLLET;


	//ע��ev
	//epfd�����ĸ�epollִ�в���
	//EPOLL_CTL_ADD��Ҫִ�е���ע�ᣬ�������������޸ĺ�ɾ��
	//listenfd����Ҫ������һ���׽���
	//Ҫ����ʲô���͵��¼�
	epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

	//����serveraddr
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;

	char *local_addr="127.0.0.1";
	inet_aton(local_addr,&(serveraddr.sin_addr));//����serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1")

	serveraddr.sin_port = htons(portnumber);
	
	//���׽��ֺ͵�ַ
	bind(listenfd,(sockaddr*)&severaddr, seizof(serveraddr));

	//�����׽���
	listen(listenfd,LISTENQ);

	maxi=0;

	while(1)
	{

	ndfs=epoll_wait(epfd,events,20,500);
	for(i=0;i<nfds;++i)
	{
		//���event[i]�������׽���
		if(event[i].data.fd == listenfd)
		{
			//accept���տͻ����׽���
			connfd = accept(listenfd, (sockaddr*)&clientaddr,&clent);
			if(connfd < 0)
			{
				perror("connfd < 0");
				exit(1);
			}

			//str ��ȡ�ͻ����׽��ֵ�ip��ַ
			char* str = inet_ntoa(clientaddr.sin_addr);
			cout<<"accpet a connection from "<<str<<endl;

			//
			ev.data.fd=connfd;
			ev.events=EPOLLIN | EPOLLET;

			//ע��ev
			epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
		}
		//������Լ����ӵĿͻ��ˣ������յ�����
		else if(events[i].events & EPOLLIN)
		{
			cout<<"EPOLLIN"<<endl;
			//���events[i]��Ӧ���׽�����Ч���������һ��ѭ��
			if((sockfd = events[i].data.fd) < 0)
				continue;

			//��ȡ�ɹ����ض�ȡ������
			//��ȡ����β����0
			//��ȡ����ʧ�ܷ���-1
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

			//��line�����ĩβ����ս����Ȼ�����
			line[n] = '/0';
			cout<<"read"<<line<<endl;

			//
			ev.data.fd = sockfd;
			ev.events = EPOLLOUT|EPOLLET;

		}
		//��������ݷ���
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