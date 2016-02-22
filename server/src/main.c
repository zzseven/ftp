#include "factory.h"

int main(int argc, char* argv[])
{
	if(argc != 5)
	{
		printf("error args\n");
		return -1;
	}

	int thread_num = atoi(argv[3]); //port
	int capacity = atoi(argv[4]); // thread account;
	factory tf;			//struct thread information
	factory_init(&tf, thread_num, capacity, thread_handle);  //factory init
	factory_start(&tf);
}

void* thread_handle(void* arg)
{

}
