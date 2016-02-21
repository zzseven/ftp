#include "head.h"

int flchdir(pmy_addr laddr,  char *buf2, char * home)
{
	int i = 0;
	int k = 0;
	int leavel = laddr->leavel;
	int len = strlen(laddr->cur_addr);
	int j = len;
	char filename[128];
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
			}else if(buf2[i+1] != ' ' && buf2[i+1] != '\0')
			{
				goto ADD;
			}else
				i++;
		}else if(buf2[i] == ' ')
		{
			i++;
		}else
		{
ADD:		laddr->cur_addr[j++] = '/';
			bzero(filename, sizeof(filename));
			k = 0;
			while(buf2[i]!='\0' && buf2[i]!=' ')
			{
				filename[k++] = buf2[i];
				laddr->cur_addr[j++] = buf2[i++];
			}
			leavel ++;
			filename[k] = '\0';
		}

	}

	laddr->cur_addr[j] = '\0';
	if(access(laddr->cur_addr, 0)==0) 
	{
		strcpy(buf2, filename);
		return 1;
	}
	
//	struct stat info;
//	stat(laddr->cur_addr, &info);
//	if(S_ISDIR(info.st_mode))
//	{
//		puts(laddr->cur_addr);
//		laddr->leavel = leavel;
//		return 1;
//	}else
//	{
//		return 0;
//  }
}
