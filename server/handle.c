#include "config.h"
#include "handle.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

UserNode* alloc_node(){
	UserNode *pNode = (UserNode*)malloc(sizeof(UserNode));
	if(NULL != pNode){
		pNode->user = (UserInfo*)malloc(sizeof(UserInfo));
		if(NULL == pNode->user){
			free(pNode);
			return NULL;
		}
		memset(pNode->user,0,sizeof(UserInfo));
		pNode->next = NULL;
	}
	return pNode;
}



int do_login(char *name,char *ipaddr,int port,int fd){
	UserNode *pNode = alloc_node();
	if(NULL == pNode)
		return -1;
	strcpy(pNode->user->name,name);
	strcpy(pNode->user->ipaddr,ipaddr);
	pNode->user->port = port;
	pNode->user->acceptfd = fd;
	if(NULL==uList->head && NULL==uList->tail)
		uList->head = uList->tail = pNode;
	else{
		uList->tail->next = pNode;
	}
	uList->online_number ++;
	return 1;
}

void do_logout(char *name,char *ipaddr,int port){
	UserNode *pNode = uList->head;
	UserNode *pPre  = pNode;

	if(strcmp(pNode->user->name,name) == 0){
		uList->head = pNode->next;
		free(pNode->user);
		free(pNode);
		if(uList->online_number == 1)
			uList->tail = NULL;
		uList->online_number --;
		return ;
	}else
		pNode = pNode->next;
	while(pNode != uList->tail){
		if(strcmp(pNode->user->name,name) == 0){
			pPre->next = pNode->next;
			free(pNode->user);
			free(pNode);
			uList->online_number --;
			return ;
		}
		pPre = pPre->next;
		pNode = pNode->next;
	}
	uList->tail = pPre;
	pPre->next = NULL;
	free(pNode->user);
	free(pNode);
	uList->online_number --;
}

/*void do_chat_public(char *name,char *message){
	UserNode *pNode = uList->head;
	char *msg = (char*)malloc(strlen(name)+strlen(message)+2);
	memset(msg,0,strlen(name)+strlen(message)+2);
	strcat(msg,name);
	strcat(msg,":");
	strcat(msg,message);
	while(pNode->next){
		if(strcmp(pNode->user->name,name) != 0){
			write(pNode->user->acceptfd,msg,strlen(name)+strlen(message)+2);
		}
		pNode = pNode->next;
	}
	free(msg);
}*/
void do_chat_public(UserNode *pSender,char *name,char *message){
	UserNode *pNode = uList->head;
	char *msg = (char*)malloc(strlen(name)+strlen(message)+2);
	memset(msg,0,strlen(name)+strlen(message)+2);
	strcat(msg,name);
	strcat(msg,":");
	strcat(msg,message);
	printf("chat_public:%s\n",msg);
	while(pNode->next){
		if(pNode != pSender){
			write(pNode->user->acceptfd,msg,strlen(name)+strlen(message)+2);
		}
		pNode = pNode->next;
	}
	free(msg);
}



char* do_chat_private(char *name,char *message){


}

static char readBuff[1024] = {0};
static char writeBuff[1024] = {0};
static int stdFlag = 0;
static int sockFlag = 0;
void readFromStd(){
    while(1){
        gets(writeBuff);
        stdFlag = 1;
    }
}
static int acptfd;
void readFromSock(){
    while(1){
		printf("acceptfd:%d\n",acptfd);
        if(read(acptfd,readBuff,1024) > 0)
            sockFlag = 1;
		printf("read data from socket!!\n");
    }
}

void sub_process(){
		UserNode *pNode = uList->tail;
		char *pName = pNode->user->name;
		int acceptfd = pNode->user->acceptfd;
		acptfd = acceptfd;
    	memset(readBuff,0,sizeof(readBuff));
        pthread_t thrdStd,thrdSock;
        pthread_create(&thrdStd,NULL,(void*)readFromStd,NULL);
        pthread_create(&thrdSock,NULL,(void*)readFromSock,NULL);
        while(1){
            if(sockFlag == 1){
		        sockFlag = 0;
				
	            printf("%s:%s\n",pName,readBuff);
	            if(strstr(readBuff,"EXIT")){
	                exit(0);
	            }else
					do_chat_public(pNode,pName,readBuff);
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
        close(acceptfd);	

}
/*void sub_process(char *pName,int acceptfd){
		acptfd = acceptfd;
    	memset(readBuff,0,sizeof(readBuff));
        pthread_t thrdStd,thrdSock;
        pthread_create(&thrdStd,NULL,(void*)readFromStd,NULL);
        pthread_create(&thrdSock,NULL,(void*)readFromSock,NULL);
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
        close(acceptfd);	

}*/
