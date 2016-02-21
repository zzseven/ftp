#include "func.h"


int send_file(int sfd)
{
	int ret;
	data_t buf;
	char fileinfor[100];
	bzero(fileinfor, 100);
	recv(sfd, fileinfor, sizeof(fileinfor), 0);
	//puts(fileinfor);
	//buf.len = strlen("file");
	//strcpy(buf.buf, "file");
	//ret = send(sfd, &buf, buf.len+4, 0);
	//if(-1 == ret)
	//{
	//	perror("send1");	
	//	return -1;
	//}

	int fd=open(fileinfor, O_RDONLY);
	if(-1 == fd)
	{
		perror("open");
		return -1;
	}

	while(bzero(&buf, sizeof(buf)),(buf.len = read(fd, buf.buf, sizeof(buf.buf)))>0)
	{
		send_n(sfd, (char*)&buf, buf.len+4);	
	}

	bzero(&buf, sizeof(buf));
	int flag = 0;
	buf.len = sizeof(int);
	memcpy(buf.buf, &flag, 4);
	send(sfd, &buf, buf.len+4, 0);
	
	printf("send over!\n");
}


int recv_file(int sfd)
{
	int len;
	int ret;
	data_t buf;
	bzero(&buf, sizeof(buf));
	ret = recv(sfd, &buf.len, 4, 0);
	if(-1 == ret)
	{
		perror("recv1");
		return -1;
	}

	recv(sfd, buf.buf, buf.len, 0);
	int fd;
	fd = open(buf.buf, O_RDWR|O_CREAT, 0666);
	if(-1 == fd)
	{
		perror("open");
		return -1;
	}

	int flag = 0;
	while(1)
	{
		bzero(&buf,sizeof(buf));
		ret = recv(sfd, &buf.len, 4, 0);
		if(-1 == ret)
		{
			perror("recv2");
			return -1;
		}
		
		ret = recv(sfd, buf.buf, buf.len, 0);
		if(-1 == ret)
		{
			perror("recv3");
			return -1;
		}

		if((buf.len == 4) && !memcmp(buf.buf, &flag, 4))
		{
			break;
		}
		
		ret = write(fd, buf.buf, buf.len);
		if(ret < 0)
		{
			perror("write");
			return -1;
		}
	}
	printf(" received the file!\n");
}


void send_n(int sfd, char *p, int len)
{
	int total = 0;
	int size = 0;
	while(total < len)
	{
		size = send(sfd, p+total, len-total, 0);
		total = total + size;
	}
}



