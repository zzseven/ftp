#include "head.h"


void make_child(pchild cptr, int n)
{
	int i;
	int fds[2];
	int ret;
	int pid;
	for(i=0; i<n; i++)
	{
		ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, fds);
		if(-1 == ret)
		{
			perror("socketpair");
			return;
		}
		pid = fork();
		if(pid==0)
		{
			close(fds[1]);
			child_handle(fds[0]);
		}

		close(fds[0]);
		cptr[i].pid = pid;
		cptr[i].fds = fds[1];
		cptr[i].busy = 0;
	}
}

void child_handle(int fdr)
{
	int fd;
	int flag = 1;
	while(1)
	{
		recv_fd(fdr, &fd);
		send_file(fd);
		write(fdr, &flag, 4);
	}
}

void send_file(int sfd)
{
	int ret;
	data_t buf;
	buf.len = strlen(DOWN_FILE);
	strcpy(buf.buf, DOWN_FILE);
	ret = send(sfd, &buf, buf.len+4, 0);
	if(-1 == ret)
	{
		perror("send1");	
		return;
	}

	int fd=open(DOWN_FILE, O_RDONLY);
	if(-1 == fd)
	{
		perror("open");
		return;
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
	close(sfd);
	return ;
}
