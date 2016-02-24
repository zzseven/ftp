#ifndef __FACTORY__
#define __FACTORY__
#include "head.h"
#include "work_que.h"

typedef void* (*pfunc)(void*); //将该函数指针定义别名pfunc
typedef struct thread_pool{    //线程信息结构体
	pthread_cond_t cond;       //控制线程同步的条件变量
	pthread_t * pth;		   //线程数组
	pfunc entry;               //子线程入口func
	que_t fd_que;              //线程队列
	int start_flag;            //线程是否启动
	int thread_num;            //线程数量
}factory, *pfactory;

void factory_init(pfactory, int, int, void* (*pfunc)(void*)); //初始化
void factory_start(pfactory); //进程开始工作
void* thread_handle(void*);  //线程处理函数
#endif
