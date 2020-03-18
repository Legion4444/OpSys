#define _GNU_SOURCE
#include <time.h>
#include <dlfcn.h>
#include <stdio.h>

struct tm *(*orig_localtime)(const time_t *timep);

struct tm *localtime(const time_t *timep){
	time_t t = *timep - 60 * 60 * 24;
	return orig_localtime(&t);
}

void 
_init(void){
	printf("Loading Date Hack.\n");
	//struct tm* (*orig_localtime)(const time_t *) = dlsym(RTLD_NEXT, "localtime");
	orig_localtime = dlsym(RTLD_NEXT, "localtime");
}
