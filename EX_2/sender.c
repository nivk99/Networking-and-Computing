#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h> 

#define MAX 80
#define PORT 8080
#define SA struct sockaddr
void func(int sockfd, char *filename)
{
	char *buff;
	int i;
    long bufsize;
    FILE *fp = fopen(filename, "rb");
    if( fp == NULL )
    {
        fprintf( stderr, "File lgb.txt can not be opened\n");
        exit(1);
    }
    
    if (fseek(fp, 0L, SEEK_END) == 0) {
        /* Get the size of the file. */
        bufsize = ftell(fp);
        if (bufsize == -1) {
            fprintf( stderr, "ftell failed\n");
            exit(1);
        }

        printf( "File size is %ld bytes\n", bufsize);
        /* Allocate our buffer to that size. */
        buff = malloc(sizeof(char) * bufsize);

        /* Go back to the start of the file. */
        if (fseek(fp, 0L, SEEK_SET) != 0) { 
            /* Error */ 
            fprintf( stderr, "fseek failed\n");
            exit(1);
        }

        /* Read the entire file into memory. */
        size_t newLen = fread(buff, sizeof(char), bufsize, fp);
        if ( ferror( fp ) != 0 ) {
            fprintf( stderr, "fread failed\n");
            exit(1);
        } 
    }
    fclose(fp);    
	for (i = 0; i < 5; i++) 
    {
        printf( "sending file size=%d %d\n", bufsize, sizeof(long) ); 
        write(sockfd, (char *)&bufsize, 8);
        printf( "sending file #%d\n", i+1 ); 
		write(sockfd, buff, bufsize);
	}
}

int main()
{
	int sockfd, len;
	struct sockaddr_in servaddr, cli;
    char cc_buf[256];

	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

    len = sizeof(cc_buf); 
    if (getsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, cc_buf, &len) != 0) { 
        perror("getsockopt");
        return -1;
    } 

    printf("Current CC: %s\n", cc_buf); 

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for sending file 5 times
	func(sockfd, "lgb.txt");

    strcpy(cc_buf, "reno"); 
    len = strlen(cc_buf);
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, cc_buf, len) != 0) {
    perror("setsockopt"); 
    return -1;
    }
    len = sizeof(cc_buf); 
    if (getsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, cc_buf, &len) != 0) {
    perror("getsockopt"); 
    return -1; 
    } 
    printf("New: %s\n", cc_buf); 

	// function for sending file 5 times
	func(sockfd, "lgb.txt");

	// close the socket
	close(sockfd);
}
