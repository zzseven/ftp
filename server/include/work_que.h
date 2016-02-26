#ifndef __WORK_QUE__
#define __WORK_QUE__
#include "head.h"
#define DOWN_FILE "files"
typedef struct node_fd{      //链表节点
	int new_fd;
	struct node_fd* pnext;
}node, *pnode;

typedef struct tdata{
	int len;
	char buf[524288];
}data_t,*pdata_t;

typedef struct que_fd{       //队列结构体，保存着队列信息
	pnode que_head, que_tail;//琐
	pthread_mutex_t mutex;
	int size;                //当前放置的描述符个数
	int que_capacity;        //队列最大长度
}que_t, *pque_t;

void factory_que_init(pque_t, int); //队列初始化
void factory_que_set(pque_t, pnode);  //将new_fd放到队列中
void factory_que_get(pque_t, pnode*);  //从队列中取new_fd;
void factory_que_destory(pque_t);
int factory_que_empty(pque_t);
void send_file(int);
void send_n(int , char *, int);
#endif
