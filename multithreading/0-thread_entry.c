#include "multithreading.h"



/**
* thread_entry- func
* @arg: void
* Return: void *
*/
void *thread_entry(void *arg)
{
	printf("%s\n", (char *)arg);
	pthread_exit(NULL);
}
