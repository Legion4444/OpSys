#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int varx, char* varc[]){
	char* filepath = "/tmp";
	int tokid = 4;	
	key_t key;
	int semid;
	if((key = ftok(filepath, tokid)) == -1){
		printf("Cannot create token: %s\n", strerror(errno));
		exit(-1);
	}
	if((semid = semget(key, 1, 0666|IPC_CREAT)) == -1){
		printf("Cannot get semaphore id: %s\n", strerror(errno));
		exit(-1);
	}
	printf("Starting symaphore value: %d\n", semctl(semid, 0, GETVAL));	
	int input = 1;
	while(input != 0){
		printf("Type a positive number to add:\n");
		scanf("%d", &input);
		if(input <= 0){ continue; }
		struct sembuf op[1];
		op[0].sem_num = 0;
		op[0].sem_op = input;
		op[0].sem_flg = 0;
		if(semop(semid, op, 1) == -1){
			printf("Cannot add/subtract %d to semaphore: %s\n", input, strerror(errno));
			exit(-1);
		}
		printf("Symaphore value: \t%d\n", semctl(semid, 0, GETVAL));
	}
}
