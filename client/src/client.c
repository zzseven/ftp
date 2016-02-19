#include "func.h"

#define NUM 10

int main(int argc, char* argv[])
{
	if(argc !=3)
	{
		printf("error args\n");
		return -1;
	}
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
	ret = connect(sfd, (struct sockaddr*)&ser, sizeof(struct sockaddr));
	if(-1 == ret)
	{
		perror("connect");
		return -1;
	}
	printf("connect success\n");
	int len;
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
	close(sfd);
}

