#include "head.h"


char * strdispose(char * str, int f, int l) //截取指定部分的字符串
{
	char newstr[100];
	int i, j = 0;
	for(i=0; i<l; i++)
	{
		if(i>=f)
		{
			newstr[j] = str[i];
			j++;
		}
	}
	newstr[j] = '\0';
	strcpy(str, newstr);
	return str;
}

void findmod(char mod[], int num)  //判断类型
{
	int temp;
	temp = num & S_IFDIR;
	if (temp == S_IFDIR)
		mod[0]='d';
	else
		mod[0]='-';
	temp = num & S_IRWXU;
	if(temp == S_IRWXU)
	{
		mod[1]='r';
		mod[2]='w';
		mod[3]='x';
	}else
	{
		temp = num & S_IRUSR;
		if(temp == S_IRUSR)
			mod[1]='r';
		else
			mod[1] = '-';
		temp = num & S_IWUSR;
		if(temp == S_IWUSR)
			mod[2]='w';
		else
			mod[2] = '-';
		temp = num & S_IXUSR;
		if(temp == S_IXUSR)
			mod[3]='x';
		else
			mod[3] = '-';
	}
	temp = num & S_IRWXG;
	if(temp == S_IRWXG)
	{
		mod[4]='r';
		mod[5]='w';
		mod[6]='x';
	}else
	{
		temp = num & S_IRGRP;
		if(temp == S_IRGRP)
			mod[4]='r';
		else
			mod[4] = '-';
		temp = num & S_IWGRP;
		if(temp == S_IWGRP)
			mod[5]='w';
		else
			mod[5] = '-';
		temp = num & S_IXGRP;
		if(temp == S_IXGRP)
			mod[6]='x';
		else
			mod[6] = '-';
	}
	temp = num & S_IRWXO;
	if(temp == S_IRWXO)
	{
		mod[7]='r';
		mod[8]='w';
		mod[9]='x';
	}else
	{
		temp = num & S_IROTH;
		if(temp == S_IROTH)
			mod[7]='r';
		else
			mod[7] = '-';
		temp = num & S_IWOTH;
		if(temp == S_IWOTH)
			mod[8]='w';
		else
			mod[8] = '-';
		temp = num & S_IXOTH;
		if(temp == S_IXOTH)
			mod[9]='x';
		else
			mod[9] = '-';
	}
	mod[10] = '\0';
}

void ls_al( char * cur_addr, char * rep)
{
	bzero(rep, sizeof(rep));
	char mod[11];	//存取类型字符串
	char addr[128]; //存取地址字符串
	char time[30];  //存取时间字符串
	
	DIR * dir;
	dir = opendir(cur_addr);
	struct dirent *pt;
	struct stat term;
	int size;
	char str[80];

	while(pt = readdir(dir))
	{
		bzero(str, 80);

		sprintf(addr, "%s%s%s", cur_addr, "/", pt->d_name);
		stat(addr, &term);
		findmod(mod, term.st_mode);
		strcpy(time, ctime(&term.st_ctime));
		//printf("%s %-3ld %-5s %-5s %-7ld %s %s\n", mod, term.st_nlink, getpwuid(term.st_uid)->pw_name, getgrgid(term.st_gid)->gr_name, term.st_size,strdispose(time, 4, 16), pt->d_name);
		size = sprintf(str, "%s  %-3ld %-5s %-5s %-8ld %s %s ", mod, term.st_nlink, getpwuid(term.st_uid)->pw_name, getgrgid(term.st_gid)->gr_name, term.st_size,strdispose(time, 4, 16), pt->d_name);
		//puts(str);
		//send(sfd, str, strlen(str), 0);
		strcat(rep, str);
		strcat(rep, "\n");


	}
}
