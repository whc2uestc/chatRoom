#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <pthread.h>
void readFromStd();
void readFromSock();
        int acceptfd;
char readBuff[1024] = {0};
char writeBuff[1024] = {0};
int stdFlag = 0;
int sockFlag = 0;
int main(){
    char pName[10];
    int sockfd;
    struct sockaddr_in servAddr;

    if((sockfd=socket(AF_INET,SOCK_STREAM,0)) < 0){
        exit(0);    
    }
    printf("sock ok!\n");
    
    bzero(&servAddr,sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(65400);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
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
                strcpy(pName,&readBuff[7]);
                if(write(acceptfd,"LOGIN OK",10) > 0)
                    printf("*********%s已登录**********\n",pName);
        }
        memset(readBuff,0,sizeof(readBuff));
        pthread_t thrdStd,thrdSock;
        pthread_create(&thrdStd,NULL,(void*)readFromStd,NULL);
        pthread_create(&thrdSock,NULL,(void*)readFromSock,NULL);
        //if(fork() == 0){
        while(1){
            if(sockFlag == 1){
		        sockFlag = 0;

	            printf("%s:%s\n",pName,readBuff);
	            if(strstr(readBuff,"EXIT")){
	                exit(0);
	            }
                memset(readBuff,0,sizeof(readBuff));
		    }
                
		    if(stdFlag == 1){
		        stdFlag = 0;
                if(write(acceptfd,writeBuff,strlen(writeBuff)) > 0)
                    //printf("write ok!\n");
                if(strstr(writeBuff,"EXIT")){
                    exit(0);
                }
                memset(writeBuff,0,sizeof(writeBuff));
		    }
        }
        //}
        close(acceptfd);
    } 
    close(sockfd);
    return 0;
}
void readFromStd(){
    while(1){
        gets(writeBuff);
        stdFlag = 1;
    }
}
void readFromSock(){
    while(1){
        if(read(acceptfd,readBuff,1024) > 0)
            sockFlag = 1;
    }
}
