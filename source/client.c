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
	printf("------------------------------\n");

	//Client Loop	
	for(;;){
		printf("Choose an option:\n\t1) Add a Record\n\t2) List all Records\n\t3) Edit a Record (Not Implemented)\n\t4) Remove all Records\n\t0) Exit\n");
		int input[2];
		scanf("%d", input);
		switch(*input){
			case ADD: ;
			{
				int n; 
				n = send(sockfd, input, 4, 0);
				if(n < 0){
					printf("Error Sending [%d] to Server\n", *input);
				}
				if(n >= 0){
					printf("Sent [%d] to Server\n", *input);
				}
				int dataLength = 16;	
				char name[25];
				char number[8];				
				char state[3];

				while(1){
					printf("Name of new Record:\n");
					fgets(name, 24, stdin);
					fgets(name, 24, stdin);
					//scanf("%24[0-9a-zA-Z ]",name);
					for(int i = 0; i < 25; i++){
						if(name[i] == '\n') name[i] = '\0';
					}
					printf("Number of new Record:\n");
					scanf("%7s", number);
				
					printf("State of new Record:\n");
					scanf("%2s", state);

					printf("Add [%s,%s,%s]? (y/n)\n", name, number, state);
					char answer[2];
					scanf("%1s", answer);
					if(answer[0] == 'y') break;
				}
				//Begin calculating size of name
				int name_size = 0;
				for(int i = 0; i < 25; i++){
					if(name[i] == '\0'){
						break;
					}
					name_size = i + 1;
				}
				dataLength = name_size + 1 + 7 + 1 + 3;
				//Begin adding char arrays together
				char data[dataLength];
				for(int i = 0; i < name_size; i++){
					data[i] = name[i];	
				}
				data[name_size] = ',';
				for(int i = name_size + 1; i < name_size + 1 + 8; i++){
					data[i] = number[i - name_size - 1];
				}
				data[name_size + 8] = ',';
				data[name_size + 9] = state[0];
				data[name_size + 10] = state[1];
				data[name_size + 11] = '\0';
				
				//printf("Sending Length of Test Record\n");
				n = send(sockfd, &dataLength, 4, 0);
				if(n < 0){
                                        printf("Error Sending [%d] to Server\n", dataLength);
                                }
                                if(n >= 0){
                                        printf("Sent [%d] to Server\n", dataLength);
                                }

				//printf("Sending Test Record\n");		
				//char data[16] = "Test,1234567,VA";
				n = send(sockfd, &data, dataLength, 0);
				if(n < 0){
                                        printf("Error Sending [%s] to Server\n", data);
                                }
                                if(n >= 0){
                                        printf("Sent [%s] to Server\n", data);
                                }
			}
				break;
			case LIST: ; 
			{
				int n;
				n = send(sockfd, input, 4, 0);
				if(n < 0){
                                        printf("Error Sending [%d] to Server\n", *input);
                                }
                                if(n >= 0){
                                        printf("Sent [%d] to Server\n", *input);
                                }

				int data_length;
				n = recv(sockfd, &data_length, 4, 0);
				if(n < 0){
                                        printf("Error recieving from Server\n");
                                }
                                if(n >= 0){
                                        printf("Recieved [%d] to Server\n", data_length);
                                }
				if(data_length > 0){
					char buff[data_length];
					n = recv(sockfd, &buff, data_length, 0);
					if(n < 0){
                                        	printf("Error recieving from Server\n");
                                		continue;
					}
                                        printf("Recieved [\n%s] from Server\n", buff);
                               	}
				if(data_length <= 0){
					printf("Recieved [ ] from Server\n");
				}
			}
				break;
			case REMOVE: ;
			{
				int n;
				n = send(sockfd, input, 4, 0);
				if(n < 0){
					printf("Error Sending [%d] to Server\n", *input);
					goto end_connection;
				}
				printf("Sent [%d] to Server\n", *input);
			}
				break;
			case EXIT:
				goto end_connection;
				
			default:
				continue;
		}
		printf("------------------------------\n");
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
