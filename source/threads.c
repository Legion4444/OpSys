#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_THREADS 1000000L

void * thread_do(void *arg);
void printids(const char *s);

int main(int argc, char*argv[])
{
	int err;
	pthread_t tid;

	if (argc != 2) {
		printf("usage: %s num_threads \n", argv[0]); 
		exit(0);
	}

	printf("here\n");

	long num_threads = atol(argv[1]);
	if (num_threads > MAX_THREADS) {
		printf("usage: %s num_threads (< %ld)\n", argv[0], MAX_THREADS); 
		exit(0);
	}


	int arr[MAX_THREADS];
	long i;
	for (i = 0; i < num_threads; i++)
		arr[i] = i;

	for (i = 0; i < atol(argv[1]); i++)
		err = pthread_create(&tid, NULL, thread_do, &arr[i]);
		//err = pthread_create(&tid, NULL, thread_do, &i);

	if (err != 0)
		perror("Error creating thread");

	printids("main thread");
	
	sleep(2);
	exit(0);
}

void * thread_do(void *arg)
{
	int i = *(int *) arg;
	printf("thread %d\n", i);
	fflush(NULL);
	
	char str[16];
	sprintf(str, "thread: %d", i);
	printids(str);

	while(1) {
		printids(str);
		sleep(1);
	}	

	return ((void *)0);
}

void printids(const char *s)
{
	pid_t 		pid;
	pthread_t	tid;

	pid = getpid();
	tid = pthread_self();

	printf("%s: pid %u tid (0x%x) \n", s, (unsigned int)pid, (unsigned int)tid);
	fflush(NULL);
}
