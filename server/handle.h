#ifndef HANDLE_H_
#define HANDLE_H_
#include "config.h"
int do_login(char *name,char *ipaddr,int port,int fd);
void do_logout(char *name,char *ipaddr,int port);
void do_chat_public(UserNode *pSender,char *name,char *message);
void do_chat_private(char *fromName,char *toName,char *message);
void sub_process();

#endif
