#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>

void main(int argv, char* argc[]){
	struct utsname system;
	uname(&system);
	printf("System Information:\n\tName: \t%s\n\tNode Name: \t%s\n\tRelease: \t%s\n\tVersion: \t%s\n\tMachine: \t%s\n\tDomain Name: \t%s\n", system.sysname, system.nodename, system.release, system.version, system.machine, system.domainname);	
	printf("Logged in User: %s\n", getlogin());
	printf("Max Children per Process: %d\n", sysconf(_SC_CHILD_MAX));

}
