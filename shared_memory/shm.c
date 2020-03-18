/***
 * shm.c - A shared memory demo.
 *
 * Inspired by examples in Advanced Programming in the UNIX Environment, 
 * third edition, pp. 529.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h> 

void print_error(const char*, int);

int main(int argc, char* argv[])
{

	/*
	 * Get a key I can share with other processes.
	 */

	int tokid = 40;
	char *filepath = "/tmp";

	key_t key;
	if ((key = ftok(filepath, tokid)) == -1)
		print_error("Can not create token", errno);

	printf("Token id: %d\n", key);

	/*
	 * Get an id for the shared memory space.
	 */

	long bufsz = sysconf(_SC_PAGESIZE);
	printf("Page size: %ld\n", bufsz);

	int shmid;
	if ((shmid = shmget(key, bufsz, IPC_CREAT | 0666)) == -1)
		print_error("Can not create shared memory", errno);

	printf("Shared memory id: %d\n", shmid);

	/*
	 * Attach: Get a pointer to the memory space. 
	 */

	char *shm = NULL;
	if ((shm = shmat(shmid, 0, 0)) == (void *)-1)
		print_error("Unable to attach to shared memory", errno);

	/*
	 * Write to the shared memory.
	 */ 

	/*
	 * Set up semaphore
 	*/
	int semid;
	key_t key2;
	if((key2 = ftok(filepath, tokid+2)) == -1){
		print_error("Cannot create token 2", errno);
		exit(-1);
	}
	if((semid = semget(key2, 1, 0666|IPC_CREAT)) == -1){
		print_error("Cannot get semaphore id", errno);
		exit(-1);
	}
	//Set up Ops
	struct sembuf op[2];
	op[0].sem_num = 0;
	op[1].sem_num = 0;
	op[0].sem_op = 0; //Wait for Zero
	op[1].sem_op = 1; //Add one
	op[0].sem_flg = 0;
	op[1].sem_flg = SEM_UNDO;	
	//Since semop performs all ops atomically, 
	//	we can have it wait for zero and then add 1 when it is zero all at once, 
	//	so that no two ops are ever modifying the space at the same time
	
	if(semop(semid, op, 2) == -1){ //Waits for semid[1] to be zero and immediately adds 1 to it before any other semop can
		printf("Cannot wait for 0/add 1 on semaphore\n");
		exit(0); 
	}	
	int shmlen = strlen(shm);
	printf("Shared memory bytes used: %d\n", shmlen);

	if(argc == 2){
		char cbuf[256];
		char *name = "Patrick: ";
		char *n = "\n";
		strncpy(cbuf, n, 1);
		strncat(cbuf, name, 10);
		strncat(cbuf, argv[1], 245);

		int cbuflen = strlen(cbuf);
		printf("Length of string to write: %d\n", cbuflen);

		if (shmlen + cbuflen + 1 < bufsz) {
			printf("Before write size: (%lu)\n", strlen(shm));

			memcpy(shm + shmlen, cbuf,  cbuflen + 1);

			printf("After write size: (%lu)\n", strlen(shm));
			printf("Log:%s\n", shm);
		} 
		else {
			printf("Warning: Not Enough Shared Memory, Messages Purged\n");
			memset(shm, 0, bufsz);
			memcpy(shm, cbuf, cbuflen + 1);
			printf("After write size: (%lu)\n", strlen(shm));
			printf("Log:%s\n", shm); 
		}
	}
	if(argc == 1){
		printf("Log:%s\n", shm);	
	}
	if(argc > 2){
		printf("Usage:\n\tAdd Message - ./shm \"Message\"\n\tView Messages - ./shm\n");
	}
	//sleep(30);

	/*
 	 * Clean up the shared memory pointer and id.
	 */
	
	/* Turned this all off so that we don't have to constantly keep a process open
	if (shmdt(shm) == -1)
		print_error("Unable to detach shared memory", errno);

	if (shmctl(shmid, IPC_RMID, 0)  == -1)
		print_error("Can not validate shared memory", errno);
	*/
	exit(0);
}

void print_error(const char* str, int code)
{
	printf("%s: %s\n",str, strerror(code));
	exit(-1);

}


//  END OF FILE
