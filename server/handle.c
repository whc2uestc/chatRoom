/*
 * handle.c
 * Author:whc
 * Date:2015.10.26
 * Description: Deal with the client's affairs.
 */

#include "config.h"
#include "handle.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

/*
 * alloc a new node of UserNode.
 */
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

/*
 * deal with the affair of login.
 */
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
		uList->tail = pNode;
	}
	uList->online_number ++;
	return 1;
}

/*
 * deal with the affair of log out.
 */
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

/*
 * chat in the public channel.
 */
void do_chat_public(UserNode *pSender,char *name,char *message){
	UserNode *pNode = uList->head;
	char *msg = (char*)malloc(strlen(name)+strlen(message)+2);
	memset(msg,0,strlen(name)+strlen(message)+2);
	strcat(msg,name);
	strcat(msg,":");
	strcat(msg,message);
	while(pNode){
		if(pNode != pSender){
			write(pNode->user->acceptfd,msg,strlen(name)+strlen(message)+2);
		}
		pNode = pNode->next;
	}
	free(msg);
}

/*
 * chat in the private channel.
 */
void do_chat_private(char *fromName,char *toName,char *message){
	UserNode *pNode = uList->head;
	char *msg = (char*)malloc(strlen(fromName)+strlen(message)+2);
	memset(msg,0,strlen(fromName)+strlen(message)+2);
	strcat(msg,fromName);
	int index = 0;
	while(message[index]!=':')
		index ++;
	
	strcat(msg,&message[index]);
	while(pNode){
		if(0==strcmp(pNode->user->name,toName)){
			write(pNode->user->acceptfd,msg,strlen(msg)+1);
		}
		pNode = pNode->next;
	}
}

/*
 * check if the chat is in the private channel or not.
 */
int check_if_private(char *name,char *message){
	if(NULL==name || NULL==message)
		return 0;
	if(message[0] == '#'){
		int index = 1;
		while(message[index] != '\0'){
			if(message[index] == ':' && index > 1)
				return 1;
			name[index-1] = message[index++];
		}
	}
	return 0;
}

/*
 * get all the list name.
 */
void get_list_name(char *buf){
	UserNode *pNode = uList->head;
	while(pNode){
		strcat(buf,pNode->user->name);
		strcat(buf,"\n");
		pNode = pNode->next;
	}
}

/*
 * the sub process to deal with the client.
 */
void sub_process(){
		UserNode *pNode = uList->tail;
		char *pName = pNode->user->name;
		char toName[20] = {0};
		int acceptfd = pNode->user->acceptfd;
    	char readBuff[1024] = {0};
		printf("into sub process:%s %d\n",pName,acceptfd);
        while(1){
        	if(read(acceptfd,readBuff,1024) > 0){
	            printf("%s:%s\n",pName,readBuff);
	            if(strstr(readBuff,"#EXIT")){
	                exit(0);
	            }else if(readBuff==strstr(readBuff,"#LIST")){
					memset(readBuff,0,sizeof(readBuff));
					get_list_name(readBuff);
					write(acceptfd,readBuff,strlen(readBuff)+1);
				}else if(check_if_private(toName,readBuff)){
					do_chat_private(pName,toName,readBuff);
				}else
					do_chat_public(pNode,pName,readBuff);
                memset(readBuff,0,sizeof(readBuff));
			}
        }
        close(acceptfd);	

}
