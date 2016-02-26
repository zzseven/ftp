#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct tdata{
	int len;
	char buf[524288];
}data_t,*pdata_t;

typedef struct threadt{
	pthread_t  pth;
	char filename[50];
	long int filesize;
	long int recvsize;
}thrnode, *pthrnode;

void recv_n(int , char *, int);
void * threadfunc(void *(arg));
