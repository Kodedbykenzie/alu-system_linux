#include "multithreading.h"



/**
* tprintf- func
* @format: char * const
* Return: int
*/
int tprintf(char const *format, ...)
{
	pthread_t tid;

	tid = pthread_self();
	printf("[%ld] %s", tid, format);
	return (0);
}
