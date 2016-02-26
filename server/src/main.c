#include "factory.h"

int main(int argc, char* argv[])
{
	if(argc != 5)
	{
		printf("error args\n");
		return -1;
	}

	int thread_num = atoi(argv[3]); //thread account
	int capacity = atoi(argv[4]);   //duilie changdu
	factory tf;			            //struct thread information
	factory_init(&tf, thread_num, capacity, thread_handle);  //factory init
	factory_start(&tf);

	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sfd)
	{
		perror("socket");
		return -1;
	}

	struct sockaddr_in ser;
	bzero(&ser, sizeof(ser));
	ser.sin_family = AF_INET;
	ser.sin_port = htons(atoi(argv[2]));
	ser.sin_addr.s_addr = inet_addr(argv[1]);
	int ret;
	ret = bind(sfd, (struct sockaddr*)&ser, sizeof(struct sockaddr));
	if(ret == -1)
	{
		perror("bind");
		return -1;
	}
	listen(sfd, capacity); //capacity表示能通处理的最大连接数
	int epfd = epoll_create(1);
	struct epoll_event ev, evs[2];
	ev.events = EPOLLIN;
	ev.data.fd = sfd;
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &ev);
	if(-1 == ret)
	{
		perror("epoll_ctl");
		return -1;
	}
	while(1)
	{
		ret = epoll_wait(epfd, evs, 2, -1);
		if(ret > 0)
		{
			int new_fd;
			if(evs[0].data.fd == sfd && evs[0].events == EPOLLIN)
			{
				pnode pnew;
				pnew = (pnode)calloc(1, sizeof(node));
				new_fd = accept(sfd, NULL, NULL);
				pnew->new_fd = new_fd;
				printf("%d\n", new_fd);
				factory_que_set(&tf.fd_que, pnew);
				printf("sdfffffsfffffffffffff\n");
				pthread_cond_signal(&tf.cond);
			}
		}
	}
}

void* thread_handle(void* arg)
{
	
	pfactory pf = (pfactory)arg;
	pque_t pq=&pf->fd_que;
	printf("The start_flag is %d\n", pf->start_flag);
	pnode pcur;
	if(factory_que_empty(pq))
	{
		pthread_cond_wait(&pf->cond, &pq->mutex);
		pthread_mutex_unlock(&pq->mutex);
	}
	factory_que_get(pq, &pcur);
	printf("got new_fd\n");
	send_file(pcur->new_fd);
	//close(pcur->new_fd);
	free(pcur);
	//action
}
