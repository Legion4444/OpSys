/*
 * server.c
 * derived from Advanced Programming in the UNIX Environment, 3rd Edition
 *
 * Eric McGregor 02/27/15
 */

#include <sys/types.h>	// getaddrinfo()
#include <netdb.h>	// getaddrinfo()
#include <sys/socket.h> // getaddrinfo()
#include <stdio.h>	// printf()
#include <stdlib.h> 	// exit()
#include <unistd.h>	// gethostname()
#include <arpa/inet.h>  // inet_ntop()
#include <string.h>	// strerror()
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <errno.h>
#include <pthread.h>

#include <stdbool.h>	//Why the hell does C need to import the freaking bool type

#define QLEN 128

#define END_TRANSMISSION -1
#define EXIT 0
#define ADD 1
#define LIST 2
#define EDIT 3
#define REMOVE 4
#define START_TRANSMISSION 5
#define INVALID_DATA 6
#define SUCCESSFUL 7

#ifndef HOST_NAME_MAX 
	#define HOST_NAME_MAX 256 
#endif

extern int errno;

void print_ip_addresses(struct addrinfo *ai);
void print_error(char *);
void * handle_client(void *);

int main(int argc, char *argv[]) 
{
	char *host_name;

    	if (argc == 2) {
		host_name = argv[1];		
	}
	else {
		host_name = malloc(HOST_NAME_MAX);
		memset(host_name, 0, HOST_NAME_MAX);

		if (gethostname(host_name, HOST_NAME_MAX) < 0) {
			print_error("gethostname error");
			return(-1);
		}
	}

	printf("host name: %s\n", host_name);

	struct addrinfo 	*host_ai;
	struct addrinfo 	hint;

	hint.ai_flags = 0;
        hint.ai_family = 0;
        hint.ai_socktype = SOCK_STREAM;
        hint.ai_protocol = 0;
        hint.ai_addrlen = 0;
        hint.ai_canonname = NULL;
        hint.ai_addr = NULL;
        hint.ai_next = NULL;

	if ((getaddrinfo(host_name, "tokenserver", &hint, &host_ai)) != 0) { 
		print_error("getaddrinfo error");
        	exit(1);
	}

	print_ip_addresses(host_ai);

    	int host_fd;
	if ((host_fd = socket(host_ai->ai_addr->sa_family, SOCK_STREAM, 0)) < 0) {
		print_error("unable to create socket"); 
		return(-1);
	}
	printf("socket created [%d]\n", host_fd);

        if (bind(host_fd, host_ai->ai_addr, host_ai->ai_addrlen) < 0) {
                print_error("unable to bind to socket");
                exit(1);
        }
	printf("bind returned success\n");

	freeaddrinfo(host_ai);

        if (listen(host_fd, QLEN) < 0) {
                print_error("listen failed");
                exit(1);
        }
	printf("listen returned success\n");

	int last_client_ip_int = 0;

	struct sockaddr client_sockaddr;
	socklen_t client_sockaddr_len = sizeof(client_sockaddr);
	printf("Awaiting Connection(s)...\n------------------------------\n");
	
	//The server loop
	for (;;) {
        	//Waits for connection
		int clfd = accept(host_fd, &client_sockaddr, &client_sockaddr_len);
        	if (clfd < 0) {
			print_error("accept error");
			exit(1); 
		}
		printf("accepted connection, socket [%d]\n", clfd);
		
		//Error if IPv6
		if (client_sockaddr.sa_family != AF_INET) {
			printf("Can not onnect with IPv6 addresses\n");
			printf("Sending -1\n");

			int mssg = -1;
			int len = send(clfd, &mssg, 4, 0);
			if (len < 0) {
				print_error("error sending data");
			}
			printf("sent %d bytes\n", len);

			close(clfd);
			continue;
		}
		
		//Fork and let child handle the client so multiple clients can be handled at once
		int fork_code = fork();
		if(fork_code == 0){
			int fd = clfd;
			pthread_t tid;
			pthread_create(&tid, NULL, handle_client, &fd);
			pthread_detach(tid);
		}
		if(fork_code > 0){
			printf("Created thread to handle client\n");
			printf("------------------------------\n");
			continue;
		}
		if(fork_code < 0){
			printf("Error in making new thread: %s", strerror(errno));
		}
		
	} 
	close(host_fd);
}

void * handle_client(void * fd){
	int token = 0;
	int clfd = *(int *) fd;
	//The real server loop starts here
        while(1){
		int n = recv(clfd, &token, 4, 0);
		if(n <= 0){
                	printf("Error recieving Op Code from Client\n");
			goto end_connection;
		}
        	if(n > 0){
                	printf("Recieved Op Code [%i] from Client\n", token);
        	}	
		
        	switch(token){
                	case ADD: ;
                        	int dataLength;
                        	n = recv(clfd, &dataLength, 4, 0);
                        	if(n < 0){
                                	printf("Error recieving dataLength from Client\n");
					goto end_connection;
				}
                        	if(n >= 0){
                                	printf("Recieved [%i] from Client\n", dataLength);
                        	}
                        	{       //Need to put the data array in a seperate statement thing so it doesn't error out
                                	char data[dataLength];
                                	n = recv(clfd, &data, dataLength, 0);
                                	if(n < 0){
                                        	printf("Error recieving data from Client\n");
						goto end_connection;
					}
                                        printf("Recieved [%s] from Client\n", data);
					FILE *fp;
					fp = fopen("./server_files/records.csv", "a+");
					fputs(data, fp);
					fputc('\n', fp);
					fclose(fp);
					
					printf("Wrote [%s] to server_files/records.csv\n", data);
					printf("------------------------------\n");
                        	}
                        	break;
			case LIST: ; 
				{
					int data_length, n;
		
					printf("Opening File...\n");	
					FILE *fp;
					fp = fopen("./server_files/records.csv", "r");
					fseek(fp, 0, SEEK_END);
					data_length = ftell(fp);
					fseek(fp, 0, SEEK_SET);
					n = send(clfd, &data_length, 4, 0);		
					if(n < 0){
                                	        printf("Error sending to Client\n");
                                	}
                                	if(n >= 0){
                                        	printf("Sent [%i] from Server\n", data_length);
                                	}

					char buff[data_length];
					fscanf(fp, "%[ \n,0-9a-zA-Z]", buff);
					n = send(clfd, &buff, data_length, 0);
					if(n < 0){
                                        	printf("Error sending data to Client\n");
                                	}
                                	printf("Sent [\n%s] to Client\n", buff);
					printf("------------------------------\n");
				}
				break;
			case REMOVE: ; 
				{
					remove("./server_files/records.csv");
					FILE *fp;
					fp = fopen("./server_files/records.csv", "a");
					fclose(fp);
					printf("Deleted ./server_files/records.csv\n");
					printf("------------------------------\n");
				}
				break;
			case EXIT: ;
                        	printf("Closing Connection\n");
				goto end_connection;
			default:
				printf("Op Code [%i] does not match available operations\n", token);
				break;
		}
	}
	end_connection:
        	close(clfd);
		return 0;
}

void print_ip_addresses(struct addrinfo *host_ai)
{
	struct addrinfo	*next_ai; 
	unsigned long *ipv4_addr;
	unsigned char *ipv6_addr;

	char ip_str[INET6_ADDRSTRLEN];

	for (next_ai = host_ai; next_ai != NULL; next_ai = next_ai->ai_next) {

		memset(ip_str, '\0', INET6_ADDRSTRLEN);

		if (next_ai->ai_addr->sa_family == AF_INET) {
			printf("IPv4 ");
			ipv4_addr  = (unsigned long*) &(((struct sockaddr_in*)next_ai->ai_addr)->sin_addr);
			inet_ntop(AF_INET, ipv4_addr, ip_str, sizeof(ip_str));
		}
		else {
			printf("IPv6 ");
			ipv6_addr = (unsigned char*) &(((struct sockaddr_in6*)next_ai->ai_addr)->sin6_addr);
			inet_ntop(AF_INET6, ipv6_addr, ip_str, sizeof(ip_str));
		}

		printf("IP address: %s\n", ip_str);
	}
}

void print_error(char *str) {
	printf("%s: %s\n", str, strerror(errno));
	//printf("%s\n", str);
	exit(1);
}

