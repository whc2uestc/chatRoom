/*
 * client.c
 * Author:whc
 * Date:2015.7.20
 * Description: a simple chat room implementation code of client.
 */

#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <pthread.h>
void readFromStd();
void readFromSock();
char writeBuff[1024] = "LOGIN ";
char readBuff[1024] = {0};
int sockfd;
int stdFlag = 0;
int sockFlag = 0;

int main(int argc,char *argv[]){
    char *pName = NULL;
    if(argc == 2){
        pName = argv[1];
    }
    struct sockaddr_in servAddr;

    if((sockfd=socket(AF_INET,SOCK_STREAM,0)) < 0){
        exit(0);
    }
    printf("sock ok!\n");
    
    bzero(&servAddr,sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(65400);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(sockfd,(struct sockaddr*)&servAddr,sizeof(struct sockaddr)) < 0){
        exit(0);
    }
    printf("connect ok!\n");
    if(pName != NULL)
        strcpy(&writeBuff[7],pName); 
    if(write(sockfd,writeBuff,strlen(writeBuff)) > 0){
        if(read(sockfd,readBuff,1024) > 0)
            printf("***********登录成功************\n");
    }
    memset(writeBuff,0,sizeof(writeBuff));
    pthread_t thrd_std,thrd_sock;
    pthread_create(&thrd_std,NULL,(void*)readFromStd,NULL);
    pthread_create(&thrd_sock,NULL,(void*)readFromSock,NULL);
    while(1){
       //scanf("%[^\n]",writeBuff);
        if(stdFlag == 1){
            stdFlag = 0;
            if(write(sockfd,writeBuff,strlen(writeBuff)) > 0)
                //printf("write ok~\n");
            if(strstr(writeBuff,"EXIT")){
                printf("**************退出**************\n");
                exit(0);
            }
            memset(writeBuff,0,sizeof(writeBuff));
        }
        if(sockFlag == 1){
            sockFlag = 0;
            printf("%s\n",readBuff);
            if(strstr(readBuff,"EXIT")){
                printf("**************退出**************\n");
                exit(0);
            }
            memset(readBuff,0,sizeof(readBuff));
        }
    }
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
        if(read(sockfd,readBuff,1024) > 0)
            sockFlag = 1;
    }
}
