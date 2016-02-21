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
		//dispose accident
		dispose_accident(fd);
		//recv_file(fd);
		//send_file(fd);
		//write(fdr, &flag, 4);
	}
}


void dispose_accident(int sfd)
{
	int size;
	int flag;
	int ret;
	char buf[512];
	char buf1[10],buf2[64]; 
	my_addr laddr;
	my_addr cpladdr;
	laddr.leavel = 0;
	cpladdr.leavel = laddr.leavel;
	char home[256];
	char strls[100];
	char rep[1000];
	int dirtest;
	if(!strcmp(getlogin(),"root"))
	{
		strcpy(laddr.cur_addr, "/");
		strcat(laddr.cur_addr, getlogin());
	}else
	{
		strcpy(laddr.cur_addr, "/home/");
		strcat(laddr.cur_addr, getlogin());
	}
	strcpy(home, laddr.cur_addr);

	while(1)
	{	
		bzero(buf, sizeof(buf));
		bzero(buf1, sizeof(buf1));
		bzero(buf2, sizeof(buf2));
		bzero(strls, sizeof(strls));
		size = recv(sfd, buf, sizeof(buf), 0);
		if(size > 0)
		{
			ret = string_handle(buf, buf1, buf2);
			if(ret == -1)
			{
				send(sfd, "Illegal request!\n", 17, 0);
				//send(sfd, buf, strlen(buf), 0);
			}
			else
			{
				if(!strcmp("pwd",buf1))
				{
					send(sfd, laddr.cur_addr, strlen(laddr.cur_addr), 0);
				}else if(!strcmp("ls", buf1) && (!strcmp("-a", buf2) || strlen(buf2)==0))
				{
					printdir(laddr.cur_addr, rep);
					send(sfd, rep, strlen(rep), 0);
				}else if(!strcmp("ls", buf1) && (!strcmp("-l", buf2) || !strcmp("-la", buf2) || !strcmp("-al", buf2)))
				{
					ls_al(laddr.cur_addr, rep);
					send(sfd, rep, strlen(rep), 0);
				}else if(!strcmp("ls", buf1))
				{	
					if(buf2[0]!='/')
						sprintf(strls,"%s/%s", laddr.cur_addr, buf2);
					else
						sprintf(strls, "%s", buf2);

					printdir(strls, rep);
					send(sfd, rep, strlen(rep), 0);
				}else if(!strcmp("cd", buf1) && (!strcmp("~", buf2) || !strcmp("",buf2)))
				{
					laddr.leavel = 0;
					strcpy(laddr.cur_addr, home);
					send(sfd, laddr.cur_addr, strlen(laddr.cur_addr), 0);
				}
				else if(!strcmp("cd", buf1))
				{
					
					bzero(cpladdr.cur_addr, sizeof(cpladdr.cur_addr));
					strcpy(cpladdr.cur_addr, laddr.cur_addr);
					printf("start %d \n", cpladdr.leavel);
					if(islegaladdr(buf2))
					{
						ret = mychdir(&cpladdr, buf2, home);
					}else
					{
						printf("hhhhhh\n");
						ret = 0;		
					}
					if(ret)
					{
						laddr.leavel = cpladdr.leavel;
						bzero(laddr.cur_addr, sizeof(laddr.cur_addr));
						strcpy(laddr.cur_addr, cpladdr.cur_addr);
						send(sfd, laddr.cur_addr, strlen(laddr.cur_addr), 0);
					}else
					{
						send(sfd, "That is not a directory or you can't access!", 44, 0);
					}

				}
			}
		}
	}
}

int string_handle(char* buf, char* buf1, char* buf2)
{
	int len = strlen(buf);
	int i, j;
	i = j = 0;

	// split first cmd
	while(buf[i] ==' ')
	{
		i++;
	}

	while(buf[i] != ' ' && i != len)
	{
		buf1[j++]= buf[i++];
	}
	buf1[j] = '\0';

	// split second cmd (address)
	j = 0;

	while(buf[i] == ' ')
	{
		i++;
	}

	while(buf[i] != ' ' && i != len)
	{
		buf2[j++]= buf[i++];
	}
	buf2[j] = '\0';

	// judge illegal string

	while(buf[i] == ' ')
	{
		i++;
	}

	if(buf[i] != '\0')
	{
		return -1;
	}else
	{
		return 0;
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
}


int islegaladdr(char *buf2)
{
	int i=0;
	while(buf2[i]!='\0')
	{
		if(buf2[i]=='.' )
		{
			if( buf2[i+1]=='.')
			{
				if(buf2[i+2] != '/' && buf2[i+2] != '\0')
				{
					return 0;
				}
			}else if(buf2[i+1]!='/' && buf2[i+1] != '\0')
			{
				return 0;
			}
		}else if(buf2[i] == '~')
		{
			if(buf2[i+1]!='/' && buf2[i+1] != '\0')
			{
				return 0;
			}
		}else if(buf2[i] == '/')
		{
			if(buf2[i+1] == '/')
			{
				return 0;
			}
		}
		i++;
	}
	return 1;
}
