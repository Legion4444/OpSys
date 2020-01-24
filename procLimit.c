#include <sys/resource.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void){
	struct rlimit resource;

	if(getrlimit(RLIMIT_NPROC, &resource) < 0){
		printf("%s\n", strerror(errno));
	}
	printf("Current limit is (%1ld)\n", (long long int) resource.rlim_cur);
	printf("Max limit is (%1ld)\n", (long long int) resource.rlim_max);

}
