#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <unistd.h>
#include <assert.h>

void main(int argv, char** argc){
	printf("Parent PID: %d\n", getpid());

	struct rlimit resource;
	resource.rlim_cur = resource.rlim_max = 20; 

	const struct rlimit* res = &resource;	
	setrlimit(RLIMIT_NPROC, res);
	
	printf("Child Process Limit Set To: %d\n", resource.rlim_cur);
	
	int forkID = 1;
	int firstPid = 0;
	int lastPid = 0;
	int count = 0;
	while(forkID > 0){
		forkID = fork();
		if(forkID == 0)
			break;	
		if(forkID > 0){
			count++;
			lastPid = forkID;
			firstPid = firstPid == 0 ? lastPid: firstPid;
		}
	}
	if(forkID == 0){
                printf("Child PID: %d (%i)\n", getpid(), ++count);
                exit(0);
      	}
	if(forkID != 0){
		for(int *status; firstPid <= lastPid; firstPid++){
			int *status;
			waitpid(firstPid, status, 0);
		}
		printf("Number of Children Processes Created: %d\nAll Child Processes Completed!\n", count);
		exit(0);
	}
}
