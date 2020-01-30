#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>

void main(int argv, char *argc[]){
	struct timeval start, end;
	struct rusage usage;
	getrusage(RUSAGE_SELF, &usage);
	start = usage.ru_utime;
	//printf("Time in User Mode: %s\nTime in Kernel Mode: %s\n", usage.ru_utime.tv_sec, usage.ru_stime.tv_sec);

	int a = 0;
	while(a < 1000000){
		int b = 1;
		int c = b;
		c++;
		a++;
	}

	getrusage(RUSAGE_SELF, &usage);
        end = usage.ru_utime;
	
	printf("Time in User Mode: %1dms\n", (/*start.tv_sec - */end.tv_usec));
}
