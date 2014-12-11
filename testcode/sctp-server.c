#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>

#define MYPORT "9000"
#define BUFFSIZE 1024*1024


void handclient(int sock)
{
    char buff[BUFFSIZE];
    int n;
    while (1) {
	printf("处理客户端!\n");
	n = recv(sock, buff, BUFFSIZE,0);
	if (n < 0)
	    	break;
	buff[n+1]='\0';
	printf("recv: %s\n",buff);
	n=send(sock, buff, n, 0);
	if (n <0 )
		break;
	
    }
    close(sock);
    printf("客户端断开连接，退出子进程!\n");
    exit(0);
}

int main(void)
{
    int sfd, cfd, i, len, pid;
    char buffer[BUFFSIZE];
    struct addrinfo hints, *srvinfo, *p;
    struct sockaddr_in caddr;

    len = 0;
    memset(&hints, 0, sizeof hints);
    memset(&caddr, 0, sizeof caddr);
    memset(buffer, 0, BUFFSIZE);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_SCTP;
    hints.ai_flags = AI_PASSIVE;


    i = getaddrinfo(NULL, MYPORT, &hints, &srvinfo);
    if (i < 0) {
	fprintf(stderr, "getaddrinfo %s\n", gai_strerror(i));
	exit(2);
    }

    for (p = srvinfo; p != NULL; p = p->ai_next) {
	sfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
	if (sfd < 0)
	    continue;
	i = bind(sfd, p->ai_addr, p->ai_addrlen);
	if (i == 0)
	    break;
    }
    if (p == NULL) {
	perror("bind 失败\n");
	exit(2);
    }

    i = listen(sfd, 10);
    if (i < 0) {
	perror("listen 失败\n");
	exit(2);
    }
    while (1) {
	printf("等待客户端连接!\n");
	cfd = accept(sfd, (struct sockaddr *) &caddr, &len);
	pid = fork();
	if (pid < 0) {
	    printf("fork error!\n");
	    exit(2);
	}
	if (pid == 0) {
	    close(sfd);
	    handclient(cfd);
	    exit(0);
	} else {
	    close(cfd);
	}
    }


    return 0;
}
