#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <assert.h>

void main(int argv, char** argc){
	int forkID = fork();
	assert(forkID >= 0);	

	if(forkID == 0){
		printf("Child PID%d\n", getpid());
		exit(0);
	}
	if(forkID > 0){
		printf("Parent PID%d\n", getpid());
		wait(&wstatus);
	}
}
