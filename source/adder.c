#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>


void main(int argv, char** argc){
	printf("Please enter a number from the list:\n(Positive) Add 1\n(Negative) Subtract 1\n(0) Exit\n");
	while(1){
		int in = 2;
		scanf("%d", &in);	
		if(in == 0){
			printf("Returning to Parent...\n");
			exit(0);
		}
		if(in > 0){
			printf("Adding 1...\n");
			kill(getppid(), SIGUSR1);
		}
		if(in < 0){
			printf("Subtracting 1...\n");
                        kill(getppid(), SIGUSR2);
		}
	}
}
