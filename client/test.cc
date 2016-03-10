 ///
 /// @file    test.c
 /// @author  zzseven
 /// @date    2016-03-10 16:24:24
 ///
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

int main()
{
	char input[100];
	int a;
	char buf1[50];
	char buf2[50];
	while(1)
	{
		printf("zz'ftp: ");
		memset(input, 0, 100);	
		memset(buf1, 0, 50);	
		memset(buf2, 0, 50);	
		fgets(input, 100, stdin);
		sscanf("12 hello", "%d %s", &a, buf2);
		printf("%d\n", a);
		if(!strcmp(buf1, "cd"))
		{
			puts(buf1);
				
		}else if(!strcmp(buf1, "pwd") && strlen(buf2)==0) 
		{
			puts(buf1);
		}else if(!strcmp(buf1, "clear") && strlen(buf2)==0 )
		{
			system("clear");
		}else if(!strcmp(buf1, "ls"))
		{
			puts(buf1);
			
		}else if(!strcmp(buf1, "exit"))
		{
			break;
		}
	}
}
