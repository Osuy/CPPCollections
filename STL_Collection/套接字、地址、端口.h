#pragma once

/*
	�׽���
		�׽��ֹ�����ַ�Ͷ˿�
		�ڲ�ͬ����ϵͳ�е�ʵ�ֲ�ͬ��
		linux���ļ�����������ʱд��fd����������int��ʵ�ʵ�������ϵͳ�й�
		windows�������SOCKET�ṹ��

	�׽��ֵĴ�����
		int test_sock = socket(int af,int type,int protocol)
			af��IP���͡�AF_INET��ʾIPv4��ַ��AF_INET��ʾIPv6��־
			type�����ݴ��䷽ʽ��SOCK_STREAM�������䣬SOCK_DGRAM���籨����
			protocol��Э�����͡�IPPROTO_TCP��tcpЭ�飻IPPROTO_UDP��udpЭ��
		�����׽���ֻ����ϵͳ������һ���׽��֣�������ʹ��
		ʹ��ǰ���������ַ�Ͷ˿ڰ�

	��ַ���˿ڵĴ�����
		������Ҫ������ַ�˿ڶ���������sockaddr��
		�����õķ����Ǵ���sockaddr_in����
		��Ϊsockaddr�ĵ�ַ�Ͷ˿�ȫ������һ��char���������ȡ��
		��sockaddr_in�����߷ֿ�����˿��Է���ſ���ȡ���޸ġ�
		�������ṹ�����Ϳ�������ת����
		���ӣ�
			struct sockaddr_in test_addr;
			bzero(&test_sock,sizeof(test_addr));
			test_addr.sin_family = AF_INET;//ipv4���ַ
			test_addr.sin_port = htos(800);//htos��800ת��Ϊ�����ֽ�
			test_addr.sin_addr.s_addr = inet_addr("192.168.1.0");//inet_addr��ip�ַ���ת��Ϊ�����ֽ�
		���ϣ�һ����ַ�˿ڽṹ�����ʹ�������ʼ�����

	�׽��ֺ͵�ַ�˿ڵİ󶨣�
		int bind(int sock, struct sockaddr* addr, socklen_t addrlen);
			sock���󶨵��׽���
			sockaddr���˿ڵ�ַ�ṹ��
			addrlen����ַ����
			return��
		����ɺ���ζ�ű�������˿ڵ���������׽��ֹ�����
		ȡ���˿�����ʱ������ŵ��׽��ֵĻ�����
	
	�����˿ڣ�����ˣ���
		int listen(int sock,int backlog);
			sock��Ҫ�����Ķ˿�
			backlog�����������������ʾ����׽��ֽ��յ����µ����ӵ����������
		�׽������ڴ���ͻ�������ʱ���µ����󽫷������������

	�������ӣ��ͻ��ˣ���
		int connect(int sock, struct sockaddr* serv_addr, socklen_t addrlen);
			sock���ͻ�������Ҫ�������ӵ��׽���
			serv_addr�����ӵķ�������ַ
			addrlen����ַ����
		�������Ӻ����������ĸõ�ַ�˿�Ҳ���ڼ���״̬������accept������������Ϊ����
		accept�������ͻ��˵ĵ�ַ�˿ڽṹ��д��������������׽���

	�������󣨷���ˣ���
		int/SOCKET accept(int sock,struct sockaddr* addr, socklen_t addrlen)
			sock���յ�������׽���
			addr����ſͻ��˵�ַ�˿����ݵĽṹ��
			addrlen����ַ����
		accept��û������ʱ������һ���ܵ������Ϊ������
		���յ�����������ȡ���ͻ��˵ĵ�ַ�Ͷ˿ڣ�д��addr
		Ȼ�󷵻�һ���ͻ����׽���
		������������׽��ֺ�read��write����������ʵ�ֽ��պͷ�������

	��������write����˫�򣩣�
		ssize_t write(int fd, const void* buf, size_t nbytes);
			fd����Ҫ�������ݵ��׽���
			buf�����͵����ݵ�ָ��
			nbytes�����͵����ݵĳ���
		�ú�����buf�г���Ϊnbytes������д���׽��ֵ������������
		����TCPЭ�齫�������ڵ����ݷ��ͳ�ȥ
		�������ʧ�ܣ��ú�������-1

	��������read����˫�򣩣�
		ssize_t read(int fd, void* buf, size_t nbytes);
			fd���������ݵ��׽���
			buf��������ݵĵ�ַָ��
			nbytes��Ҫ��ȡ�೤������
		�ú�����Ŀ���׽��ֵ����뻺������ȡ����Ϊnbytes������
		��д�뵽buf��
		���ɹ������ض�ȡ�ĳ���
		���������뻺�����Ľ�β���򷵻�0
		����ȡʧ�ܣ�����-1��

*/