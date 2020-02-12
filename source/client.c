#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAXADDRLEN 256
#define BUFLEN 128

#define END_TRANSMISSION -1
#define EXIT 0
#define ADD 1
#define LIST 2
#define EDIT 3
#define REMOVE 4
#define START_TRANSMISSION 5
#define INVALID_DATA 6
#define SUCCESSFUL 7

extern int errno;

void print_error(char *);

int main(int argc, char *argv[]) {
	struct addrinfo *host_ai;
	struct addrinfo hint;
	int sockfd, err;
    
	if (argc != 2)
        	print_error("usage: client hostname");
    
	hint.ai_flags = 0;
    	hint.ai_family = AF_INET;
    	hint.ai_socktype = SOCK_STREAM;
    	hint.ai_protocol = 0;
    	hint.ai_addrlen = 0;
    	hint.ai_canonname = NULL;
    	hint.ai_addr = NULL;
    	hint.ai_next = NULL;

	if ((err = getaddrinfo(argv[1], "tokenserver", &hint, &host_ai)) != 0) {
		printf("getaddrinfo error: %s\n", gai_strerror(err));
		return 0;
	}

	printf("creating socket\n");
	if ((sockfd = socket(host_ai->ai_addr->sa_family, SOCK_STREAM, 0)) < 0) {
		print_error("Error creating socket");
	}
	printf("socket created\n");

	printf("attempting Connection\n");
	if (connect(sockfd, host_ai->ai_addr, host_ai->ai_addrlen) != 0) {
		printf("can't connect to %s\n", argv[1]);
		print_error("Error connecting to server");
	}
	printf("connection made...\n"); 
	freeaddrinfo(host_ai);
	
	//Client Loop	
	for(;;){
		printf("Choose An Option:\n\t1) Add a Record\n\t2) List Records\n\t3) Edit a Record\n\t4) Remove a Record\n\t0) Exit\n");
		int* input;
		scanf("%d", input);
		switch(*input){
			case 1: ;
				int n; 
				n = send(sockfd, input, 4, 0);
				if(n < 0){
					printf("Error Sending [%d] to Server\n", *input);
				}
				if(n >= 0){
					printf("Sent [%d] to Server\n", *input);
				}
				int dataLength = 16;
				
				//printf("Sending Length of Test Record\n");
				n = send(sockfd, &dataLength, 4, 0);
				if(n < 0){
                                        printf("Error Sending [%d] to Server\n", dataLength);
                                }
                                if(n >= 0){
                                        printf("Sent [%d] to Server\n", dataLength);
                                }

				//printf("Sending Test Record\n");		
				char data[16] = "Test,1234567,VA";
				n = send(sockfd, &data, dataLength, 0);
				if(n < 0){
                                        printf("Error Sending [%s] to Server\n", data);
                                }
                                if(n >= 0){
                                        printf("Sent [%s] to Server\n", data);
                                }

				break;
			case 0:
				goto end_connection;
				
			default:
				continue;
		}
	}
	end_connection:
		close(sockfd);
	
    	exit(1);
}


void print_error(char *str) {
	printf("%s: %s\n", str, strerror(errno));
	exit(1);
}


// END OF FILE	
