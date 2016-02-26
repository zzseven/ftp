#include "func.h"
#include <pthread.h>

#define NUM 10
int main(int argc,char* argv[])
{
	if(argc != 3)
	{
		printf("error args\n");
		return -1;
	}
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));
	ser.sin_addr.s_addr=inet_addr(argv[1]);
	int ret;	
	ret=connect(sfd,(struct sockaddr*)&ser,sizeof(struct sockaddr));
	if(-1==ret)
	{
		perror("connect");
		return -1;
	}
	printf("connect success\n");
	int len;
	data_t buf;
	bzero(&buf,sizeof(buf));
	ret=recv(sfd,&buf.len,4,0);	
	if(-1==ret)
	{
		perror("recv1");
		return -1;
	}
	recv(sfd,buf.buf,buf.len,0);
	int fd;
	char filename[50];
	strcpy(filename, buf.buf);
	puts(filename);
	fd=open(buf.buf,O_RDWR|O_CREAT,0666);
	if(-1==fd)
	{
		perror("open");
		return -1;
	}
	//建一个线程
	thrnode thr;
	pthread_t thdId;
	thr.pth = thdId;
	strcpy(thr.filename, filename);
	recv(sfd, &thr.filesize, 8, 0);
	sleep(3);
	int flag=0;
	int line = 0;
	printf("\033[?25l");
	pthread_create(&thr.pth, NULL, threadfunc, (void*)&thr);
	//printf("recv %s\t  %.0f%%\n", filename, (recvsize*100)/(filesize*1.0));
	while(1)
	{	
		bzero(&buf, sizeof(buf));
		recv_n(sfd, (char*)&buf.len, 4);
		recv_n(sfd, buf.buf, buf.len);
		if(buf.len==4 && !memcmp(buf.buf,&flag,4))
		{	
			line = 1;
		}else
			thr.recvsize += buf.len;
		if(!line)
		{
//			printf("\033[1A"); //先回到上一行
//			printf("\033[K");  //清除该行
//			printf("recv %s\t  %.0f%%\n", filename, (recvsize*100)/(filesize*1.0));
		}else
			break;
		ret = write(fd, buf.buf, buf.len);
		if(ret < 0)
		{
			perror("write");
			return -1;
		}
	//	printf("hhh\n");

	}
	pthread_join(thr.pth, (void**)&ret);
	close(sfd);

	return 0;
}

void * threadfunc(void *(arg))
{
		
	pthrnode thr = (pthrnode)arg;
	int speed;
	long int s1, s2, gap;
	s1 = gap = 0;
	s2 = thr->recvsize;
	while(1)
	{
		s2 = thr->recvsize;
		gap = s2 -s1;

		if(thr->recvsize == thr->filesize)
		{
			break;
		}

		if(gap<=1024)
		{
			printf("recv %s \t\t %.2f%%   %ldB/s\n", thr->filename, ((thr->recvsize)*100)/((thr->filesize)*1.0), gap);
		}else if(1024<gap && gap<=1048576)
		{
			printf("recv %s \t\t %.2f%%   %.2lfKB/s\n", thr->filename, ((thr->recvsize)*100)/((thr->filesize)*1.0), (double)(gap*1.0)/1024);
		}else
		{
			printf("recv %s \t\t %.2f%%   %.2lfMB/s\n", thr->filename, ((thr->recvsize)*100)/((thr->filesize)*1.0), (double)(gap*1.0)/1048576);
		}


		
		printf("\033[1A"); //先回到上一行
		printf("\033[K");  //清除该行
		s1 = thr->recvsize; 
		sleep(1);
	}
	printf("recv %s \t\t %.2f%%\n", thr->filename, ((thr->recvsize)*100)/((thr->filesize)*1.0));
	sleep(1);
}
