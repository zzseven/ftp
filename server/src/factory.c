#include "factory.h"

void factory_init(pfactory pf, int num, int capacity, pfunc entry)
{	
	int ret;
	ret = pthread_cond_init(&pf->cond, NULL);
	if(0!=ret)
	{
		printf("pthread_cond_init fail\n");
		return ;
	}
	pf->pth = (pthread_t*)calloc(num, sizeof(pthread_t));
	pf->entry = entry;
	factory_que_init(&pf->fd_que, capacity);
	pf->start_flag = 0;
	pf->thread_num = num;
}

void factory_start(pfactory pf)
{
	if(0 == pf->start_flag)
	{
		int i;
		for(i=0;i<pf->thread_num;i++)
		{
			ret = pthread_create(&pf->pth[i], NULL, pf->entry, (void*)pf);
			if(0!=ret)
			{
				printf("pthread_create fail\n");
				return ;
			}
		}
		pf->start_flag = 1;
	}
}
