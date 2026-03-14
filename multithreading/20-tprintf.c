#include "multithreading.h"



static pthread_mutex_t mutex;

/*
* An other way of initilizing mutex without function declaration.
* pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
*/


/**
* mutex_create - func
*/
void mutex_create(void)
{
	pthread_mutex_init(&mutex, NULL);
}


/**
* mutex_destroy - func
*/
void mutex_destroy(void)
{
	pthread_mutex_destroy(&mutex);
}



/**
* tprintf - func
* @format: char const *
* Return: int
*/
int tprintf(char const *format, ...)
{
	pthread_t tid;
	va_list args;

	pthread_mutex_lock(&mutex);
	tid = pthread_self();
	printf("[%ld] ", tid);
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	pthread_mutex_unlock(&mutex);
	return (0);
}
