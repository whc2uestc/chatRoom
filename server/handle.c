#include "config.h"
#include "stdlib.h"
#include <string.h>

UserNode alloc_node(){
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

void do_chat_public(char *name,char *message){
	UserNode *pNode = uList->head;
	char *msg = (char*)malloc(strlen(name)+strlen(message)+2);
	memset(msg,strlen(name)+strlen(message)+2);
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
}


char* do_chat_private(char *name,char *message){


}



