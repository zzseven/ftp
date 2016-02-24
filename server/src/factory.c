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
		int i, ret;
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


void send_file(int sfd) //其中的一个交互程序
{
	int ret;
	data_t buf;
	buf.len=strlen(DOWN_FILE);
	strcpy(buf.buf,DOWN_FILE);
	ret=send(sfd,&buf, buf.len+4,0);//发送文件名	
	if(-1==ret)
	{
		perror("send1");
		return;
	}
	
	struct stat file;
	int fd=open(DOWN_FILE,O_RDONLY);
	if(-1==fd)
	{
		perror("open");
		return;
	}
	ret  = fstat(fd, &file);
	if(ret == -1)
	{
		perror("fstat error");
		exit(-1);
	}

	ret = send(sfd,&file.st_size,8,0);//发送文件大小

	while(1)
	{
		bzero(&buf,sizeof(buf));
		buf.len=read(fd,buf.buf,sizeof(buf.buf));
		if(buf.len > 0)
			send_n(sfd,(char*)&buf,buf.len+4);
		else
			break;
	}
	bzero(&buf,sizeof(buf));
	printf("over\n");
	int flag=0;
	buf.len=sizeof(int);
	memcpy(buf.buf,&flag,4);
	send(sfd,&buf, buf.len+4,0);//发送结束符	
	//close(sfd);
	return;
}


void send_n(int sfd,char* p,int len)
{
	int total=0;
	int size=0;
	while(total < len)
	{
		size=send(sfd,p+total,len-total,0);
		total=total+size;
	}
}





