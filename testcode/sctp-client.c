#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>

#define BUFFSIZE 1024*1024
#define SRVPORT "9000"
int main(int argc ,char *argv[]){
	int sfd,i,n;
	char buffer[BUFFSIZE];
	struct addrinfo hints,*srvinfo,*p;
	struct sockaddr_in *ipv4;
	void *addr;
	char ipstr[INET6_ADDRSTRLEN];


/*
	if(argc <2){
		printf("useage: %s serverip\n",argv[0]);
		exit(0);
	}
*/
	
	memset(&hints,0,sizeof hints);
	memset(buffer,0,BUFFSIZE);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_SCTP;


	i=getaddrinfo(argv[1],SRVPORT,&hints,&srvinfo);
	if(i !=0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(i));
		exit(1);
	}

	for(p=srvinfo;p!=NULL;p=p->ai_next){
		inet_ntop(p->ai_family,&((struct sockaddr_in *)p->ai_addr)->sin_addr,ipstr,sizeof ipstr);
		printf("%s\n",ipstr);
		sfd=socket(p->ai_family,p->ai_socktype,p->ai_protocol);
		if(sfd == -1){
			printf("can't create socket\n");
			continue;
		}
		
		if(connect(sfd,p->ai_addr,p->ai_addrlen) == -1 ){
			printf("can't connect to server\n");
			continue;
		}
		break;
	}
	if (p==NULL){
		printf("cant't connect to server \n");
		exit(1);
	}

	printf("成功连接\n");
	while(1){
		gets(buffer);
		i=send(sfd,(void *)buffer,strlen(buffer),0);
		if(i<0){
			printf("send error!\n");
			close(sfd);
			exit(0);
		}

		printf("send: %s \n",buffer);
		i=recv(sfd,buffer,i,0);
		if(i<0){
			printf("recv error!\n");
			close(sfd);
			exit(0);
		}

		buffer[i+1]='\0';
		printf("recv: %s \n",buffer);
	}
		
	
	return 0;
}
