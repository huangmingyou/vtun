#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

#define BUF_SIZE 500

int main(int argc ,char *argv[]){
	int i,sfd;
	struct addrinfo hints,*res,*p;
	struct sockaddr_storage peer_addr;
	void * addr;
	socklen_t peer_addr_len;
	ssize_t nread;
	char buf[BUF_SIZE];
	char ipstr[INET6_ADDRSTRLEN];

	memset(&hints,0,sizeof hints);
	memset(buf,0,BUF_SIZE);
	hints.ai_family=AF_INET;
	hints.ai_socktype=SOCK_DGRAM;
	hints.ai_flags=AI_PASSIVE;

	i=getaddrinfo(argv[1],"90",&hints,&res);
	if(i != 0){
		fprintf(stderr,"getaddrinfo get error:%s\n",gai_strerror(i));
		exit(i);
	}
	for(p=res;p!=NULL;p=p->ai_next){
		i++;
		inet_ntop(p->ai_family,&((struct sockaddr_in *)p->ai_addr)->sin_addr,ipstr,sizeof ipstr);
		printf("%s\n",ipstr);
	}
	printf("i=%d\n",i);
		
	return 0;
}
