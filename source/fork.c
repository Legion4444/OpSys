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
	printf("Parent PID: %d\n", getpid());
	
	int forkID = 1;
	while(forkID > 0){
		forkID = fork();
		if(forkID == 0)
			break;	
	}
	if(forkID == 0){
		sleep(1);
                printf("Child PID: %d\n", getpid());
                exit(0);
      	}
	if(forkID != 0){
		sleep(2);
		wait(NULL);
	}
}
