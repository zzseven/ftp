#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#define DOWN_FILE "file"

typedef struct pro_child{
	pid_t pid;
	int fds;
	int busy;
}child, *pchild;


typedef struct tdata{
	int len;
	char buf[1000];
}data_t, *pdata_t;

void make_child(pchild, int);
void child_handle(int);
void send_file(int);
int  recv_file(int);
void send_fd(int, int);
void recv_fd(int, int*);
void send_n(int, char*, int);



