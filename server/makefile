server:handle.o server.o init.o
	gcc handle.o server.o init.o -lpthread -o server
server.o:server.c handle.h init.h
	gcc -c server.c -lpthread -o server.o
handle.o:handle.c handle.h config.h
	gcc -c handle.c -lpthread -o handle.o
init.o:init.c init.h config.h
	gcc -c init.c -o init.o
