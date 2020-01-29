#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>

int total = 0;

void add_handler(int);
void sub_handler(int);

void main(int argv, char** argc){
	if(signal(SIGUSR1, add_handler) == SIG_ERR){
		printf("%s\n", strerror(errno));
	}
	if(signal(SIGUSR2, sub_handler) == SIG_ERR){
		printf("%s\n", strerror(errno));
	}
	int forkID = fork();
	if(forkID == 0){
		printf("Entering Adder...\n");
		const char* path[] = {"./adder", NULL};
		execl(path[0], NULL);
	}
	if(forkID > 0){
		int *status;
		waitpid(forkID, status, 0);
		printf("Final Total: %d\n", total);
	}
}

void add_handler(int sig){
	if(sig < 0){
		printf("Signal Handler Failed\n");
		exit(sig);
	}
	if(sig >= 0){
		total++;
	}
}
void sub_handler(int sig){
	if(sig < 0){
		printf("Signal Handler Failed\n");
		exit(sig);
	}
	if(sig >= 0){
		total--;
	}
}
