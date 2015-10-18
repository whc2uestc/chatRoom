#ifndef CONFIG_H_
#define CONFIG_H_

/* 
 * the information of the user online.
 * include ip,port and name.
 */
typedef struct UserInformation{
	char name[10];
	char ipaddr[16];
	int  port;
	int acceptfd;
}UserInfo;

/*
 * the node of the user online.
 * use single list to contain the online users.
 */

typedef struct UserNode{
	UserInfo *user;
	struct UserNode *next;
}UserNode;

/*
 * the list of the online user.
 * include the head and the tail of the list;
 * in order to count the number of the online users,we need to include the online number.
 */
typedef struct UserList{
	UserNode *head;
	UserNode *tail;
	int 	 online_number;
}UserList;


/*
 * the global variable of the online users list.
 */
UserList uList; 


#endif
