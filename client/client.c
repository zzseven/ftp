#include "func.h"
#include <pthread.h>
#include <unistd.h>

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
	printf("zhielijiagedenglu:");
	//这里可以加一个提示页面
	//
	//连接成功，下面开始做正事！
	//1.while循环,读取用户输入，对用户输入进行检查
	//2.逐个命令进行判断，先分析第一个命令
	//3.对能在本地完成的命令，本地进行操作如clear
	
	char input[100];
	char buf1[50];
	char buf2[50];
	char message[100];
	char recvmess[1000];
	while(1)
	{
		printf("zz'ftp: ");
		memset(input, 0, 100);	
		memset(buf1, 0, 50);	
		memset(buf2, 0, 50);	
		memset(message, 0, 100);
		memset(recvmess, 0, 1000);
		fgets(input, 100, stdin);
		sscanf(input, "%s %s", buf1, buf2);
		if(!strcmp(buf1, "pwd") && strlen(buf2)==0)
		{
			strcpy(message, "1");
			ret = send(sfd, message, strlen(message), 0);
			if(ret == -1)
			{
				printf("send message 1 error! ");
			}
			//recv1
			ret = recv(sfd, recvmess, sizeof(recvmess), 0);
			puts(recvmess);

		}
		else if(!strcmp(buf1, "cd")) 
		{
			strcpy(message, "2 ");
			strcat(message, buf2);
			puts(message);
			ret = send(sfd, message, strlen(message), 0);
			if(ret == -1)
			{
				printf("send message 2 error! ");
			}
			//recv2
			ret = recv(sfd, recvmess, sizeof(recvmess), 0);
			puts(recvmess);

		}
		else if(!strcmp(buf1, "ls"))
		{
			strcpy(message, "3 ");
			strcat(message, buf2);

			ret = send(sfd, message, strlen(message), 0);
			if(ret == -1)
			{
				printf("send message 3 error! ");
			}
			//recv3
			ret = recv(sfd, recvmess, sizeof(recvmess), 0);
			puts(recvmess);
			
		}
		else if(!strcmp(buf1, "get") && strlen(buf2)!=0 )
		{
			strcpy(message, "4 ");
			strcat(message, buf2);
			ret = send(sfd, message, strlen(message), 0);
			if(ret == -1)
			{
				printf("send message 1 error! ");
			}
			recv_file(sfd);
			//recv4
			ret = recv(sfd, recvmess, sizeof(recvmess), 0);
			puts(recvmess);
		}
		else if(!strcmp(buf1, "send") && strlen(buf2)!=0)
		{
			strcpy(message, "5 ");
			//recv_file(sfd);
			strcat(message, buf2);
			ret = send(sfd, message, strlen(message), 0);
			if(ret == -1)
			{
				printf("send message 1 error! ");
			}
			//recv
			ret = recv(sfd, recvmess, sizeof(recvmess), 0);
			puts(recvmess);
		}
		else if(!strcmp(buf1, "remove") && strlen(buf2)!=0)
		{
			strcpy(message, "6 ");
			strcat(message, buf2);
			//recv_file(sfd);
			ret = send(sfd, message, strlen(message), 0);
			if(ret == -1)
			{
				printf("send message 1 error! ");
			}
			//recv
			ret = recv(sfd, recvmess, sizeof(recvmess), 0);
			puts(recvmess);
		}
		else if(!strcmp(buf1, "clear") && strlen(buf2)==0 )
		{
			system("clear");
		}
		else if(!strcmp(buf1, "exit"))
		{
			strcpy(message, "0");
			ret = send(sfd, message, strlen(message), 0);
			if(ret == -1)
			{
				printf("send message 1 error! ");
			}
			//recv
			ret = recv(sfd, recvmess, sizeof(recvmess), 0);
			puts(recvmess);
			break;
		}
	}
}
	
//接收文件用的
int recv_file(int sfd)
{
	int len, ret;
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
		if(line)
			break;
		ret = write(fd, buf.buf, buf.len);
		if(ret < 0)
		{
			perror("write");
			return -1;
		}
	}
	pthread_join(thr.pth, (void**)&ret);
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
	printf("\033[?25h");
}
