#include "work_que.h"

void factory_que_init(pque_t pq, int capacity)
{
	int ret;
	pq->que_head = NULL;
	pq->que_tail = NULL;
	ret = pthread_mutex_init(&pq->mutex, NULL);
	if(0!=ret)
	{
		printf("pthread_mutex_init fail\n");
		return;
	}
	pq->size = 0;
	pq->que_capacity = capacity;
}
