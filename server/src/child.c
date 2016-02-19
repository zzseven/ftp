#include "head.h"

void make_child(pchild cptr, int n){
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
		cptr[i].fds = fds[i];
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
		//send_file(fd);
		write(fdr, &flag, 4);
	}
}

send_file(int fd)
{
	;
}
