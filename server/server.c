#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>

#include "handle.h"
#include "init.h"

static char readBuff[1024] = {0};
static char writeBuff[1024] = {0};
int main(int argc,char *argv[]){
    char pName[10] = {0};
    int sockfd;
	int acceptfd;
    struct sockaddr_in servAddr;
	
	char ipaddr[16] = "127.0.0.1";
	int port = 65400;
	if(argc != 3 && argc != 1){
		printf("input error!!!");
		exit(1);
	}
	else if(argc == 3){
		memset(ipaddr,0,sizeof(ipaddr));
		strcpy(ipaddr,argv[1]);
		sscanf(argv[2],"%d",&port);
	}
	
	printf("%s %d",ipaddr,port);
	
	init();

    if((sockfd=socket(AF_INET,SOCK_STREAM,0)) < 0){
        exit(0);    
    }
    printf("sock ok!\n");
    
    bzero(&servAddr,sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    servAddr.sin_addr.s_addr = inet_addr(ipaddr);
    if(bind(sockfd,(struct sockaddr*)&servAddr,sizeof(struct sockaddr)) < 0){
        exit(0);
    }
    printf("bind ok!\n");
    
    while(1){
        if(listen(sockfd,10) < 0){

        }
        printf("listen ok!\n");
        

        if((acceptfd=accept(sockfd,NULL,NULL)) < 0){
            exit(0);
        }
        printf("accept ok!\n");
        


        if(read(acceptfd,readBuff,1024) > 0){
                strcpy(pName,&readBuff[6]);
				printf("%s\n",pName);
				if(do_login(pName,"127.0.0.1",0,acceptfd) > 0){
                	if(write(acceptfd,"LOGIN OK",10) > 0)
                    	printf("*********%s已登录**********\n",pName);
				}
        }
		//pthread_t thrd;
		//pthread_create(&thrd,NULL,(void*)sub_process,NULL);

		pid_t pid;
		if((pid=fork()) == 0){
			sub_process(pName,acceptfd);
			exit(0);
		}
        
    } 
    close(sockfd);
    return 0;
}

