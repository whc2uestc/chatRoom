#include <stdlib.h>
#include "config.h"
#include "init.h"

int init(){
	uList = (UserList*)malloc(sizeof(UserList));
	if(uList == NULL)
		return -1;
	uList->head = uList->tail = NULL;
	uList->online_number = 0;
	return 1;
}



