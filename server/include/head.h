#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <pwd.h>
#include <grp.h>

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

typedef struct addr{
	int leavel;
	char cur_addr[256];
}my_addr, *pmy_addr;

void make_child(pchild, int);
void child_handle(int);
void send_file(int sfd, char * path, char * filename);
int  recv_file(int sfd, char *path);
void send_fd(int, int);
void recv_fd(int, int*);
void send_n(int, char*, int);
void dispose_accident(int sfd);
int string_handle(char* buf, char* buf1, char* buf2);
void printdir(char *dir, char *rep);
char * strdispose(char * str, int f, int l);
void findmod(char mod[], int num);
void ls_al(char *cur_addr, char *rep);
int mychdir(pmy_addr laddr, char *buf2, char *home);
int flchdir(pmy_addr laddr, char *buf2, char *home);
int islegaladdr(char *buf2);
int islegalfl(char *buf2);
void get_filename(char *buf2, char *filename);





