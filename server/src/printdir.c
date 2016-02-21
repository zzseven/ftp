#include "head.h"

void printdir(char *dir, char * rep)
{
	bzero(rep, sizeof(rep));
	int depth = 0;
	DIR *dp;
	struct dirent * entry;
	struct stat statbuf;
	
	if((dp = opendir(dir))== NULL)
	{
		//fprintf(stderr, "cannot open directroy: %s\n", dir);
		strcat(rep, "Can't open directroy: ");
		strcat(rep, dir);
		return ;
	}
	chdir(dir);
	while((entry = readdir(dp)) != NULL)
	{
		lstat(entry->d_name, &statbuf);
		if(S_ISDIR(statbuf.st_mode))
		{
			if(!strcmp(".", entry->d_name) ||!strcmp("..", entry->d_name))
				continue;
			//printf("%*s%s/  ", depth, "", entry->d_name);
			//printdir(entry->d_name, depth+4);
			strcat(rep, entry->d_name);
			strcat(rep, "/  ");
		}
		else{
			//printf("%*s%s  ", depth, "", entry->d_name);
			strcat(rep, entry->d_name);
			strcat(rep, "  ");
		}
	}

	//strcat(rep, "\n");
	//printf("\n");
	//chdir("..");
	closedir(dp);
}

