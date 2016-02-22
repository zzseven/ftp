#ifndef __WORK_QUE__
#define __WORK_QUE__
#include "head.h"

typedef struct node_fd{  //队列节点结构体
	int new_fd;
	struct node_fd* pnode;
}node, *pnode;

typedef struct que_fd{   //队列结构体，保存着队列信息
	pnode que_head, que_tail;
	pthread_mutex_t mutex;
	int size; //当前放置的描述符个数
	int que_capacity;//队列最大长度
}que_t, *pque_t;

void factory_que_init(pque_t, int); //队列初始化
#endif
