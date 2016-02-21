#include "head.h"

int mychdir(pmy_addr laddr,  char *buf2, char * home)
{
	int i = 0;
	int leavel = laddr->leavel;
	int len = strlen(laddr->cur_addr);
	int j = len;
	char cp[256];
	if(buf2[0] == '/')
		return 0;
	else if(buf2[0] == '~')
	{
		bzero(laddr->cur_addr, sizeof(laddr->cur_addr));
		laddr->leavel = 0;
		leavel = 0;
		strcpy(laddr->cur_addr, home);
		len = strlen(laddr->cur_addr);
		j = len;
		buf2[0] = ' ';
	}

	i = 0;
	while(buf2[i] != '\0')
	{
		if(buf2[i] == '/')
		{
			buf2[i] = ' ';
		}
		i++;
	}
	i = 0;
	while(buf2[i]!='\0')
	{
		if(buf2[i]=='.')
		{
			if(buf2[i+1]=='.')
			{
				if(leavel == 0)
				{
					return 0;
				}else
				{
					 while(laddr->cur_addr[j]!='/')
					 {
					 	j--;
					 }
					 leavel --;
				}
				i += 2;
			}else
			{
				i++;
			}
		}else if(buf2[i] == ' ')
		{
			i++;
		}else
		{
			laddr->cur_addr[j++] = '/';
			while(buf2[i]!='\0' && buf2[i]!=' ' && buf2[i]!='.')
			{
				laddr->cur_addr[j++] = buf2[i++];
			}
			leavel ++;
		}

	}

	laddr->cur_addr[j] = '\0';
	struct stat info;
	stat(laddr->cur_addr, &info);
	if(S_ISDIR(info.st_mode))
	{
		puts(laddr->cur_addr);
		laddr->leavel = leavel;
		return 1;
	}else
	{
		return 0;
	}
}
