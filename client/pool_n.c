#include "func.h"

void recv_n(int sfd,char* p,int len)
{

	int total=0;
	int size=0;
	while(total < len)
	{
		size=recv(sfd,p+total,len-total,0);
		total=total+size;
	}
}
