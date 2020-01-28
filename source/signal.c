#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>


void signal_handler(int);

void main(int argv, char** argc){
	if(signal(SIGUSR1, signal_handler) == SIG_ERR){
		printf("%s\n", strerror(errno));
	}
	int a = 0;
	while(1){
		a++;
		int b = a;
		int c = b - 1;
	}
	//kill(getpid(), SIGUSR1);
}

void signal_handler(int sig){
	if(sig < 0){
		printf("Signal Handler Failed\n");
		exit(sig);
	}
	if(sig >= 0){
		printf("Signal Handler Success\n");
		exit(0);
	}
	
}
