#ifndef HANDLE_H_
#define HANDLE_H_

int do_login(char *name,char *ipaddr,int port,int fd);
void do_logout(char *name,char *ipaddr,int port);
void do_chat_public(char *name,char *message);

#endif
