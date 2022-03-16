#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> 
#include <netinet/tcp.h> 

#define MAX 5000
#define PORT 8080
#define SA struct sockaddr
char buff[MAX];

#define MIN(a,b) (a>b)? b: a
     
// Function designed for chat between client and server.
void func(int sockfd)
{
	int n, i =0, j=0;
    long bufsize;
    clock_t start, end;
    double cpu_time_used;
     
	// infinite loop for chat
	for (i=0;i<5;i++) {
		// read the message from client and copy it in buffer
		recv(sockfd, (char *)&bufsize, sizeof(long), 0);
        printf( "Read file  #%d\n", i);
        start = clock();
        printf("clock=%ld\n", start);
            // Start measuring time
    struct timespec begin6, end6; 
    clock_gettime(CLOCK_REALTIME, &begin6);
        while( bufsize > 0)
        {
            n = recv(sockfd, buff, MIN(bufsize,MAX), 0);
            bufsize -= n;
            printf("n = %ld\n", n);
            if( n <=0 )
            {
                printf( "Client closed the socket\n" );
                break;
            }
    		//printf("From client: geting %d bytes, buf number %d\n", n, ++j);
        }
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf( "time is %f %d %d CLOCKS_PER_SEC=%ld\n", cpu_time_used, start, end, CLOCKS_PER_SEC);
		// print buffer which contains the client contents
    // Stop measuring time and calculate the elapsed time
    clock_gettime(CLOCK_REALTIME, &end6);
    long seconds = end6.tv_sec - begin6.tv_sec;
    long nanoseconds = end6.tv_nsec - begin6.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;
    printf("Time measured: %.3f seconds.\n", elapsed);        
	}
}

// Driver function
int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;
    char cc_buf[256];

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server accept failed...\n");
		exit(0);
	}
	else
		printf("server accept the client...\n");

    len = sizeof(cc_buf); 
    if (getsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, cc_buf, &len) != 0) { 
        perror("getsockopt");
        return -1;
    } 

    printf("Current CC: %s\n", cc_buf); 
	// Function for reading 5 times file
	func(connfd);

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

	func(connfd);
	// After chatting close the socket
	close(sockfd);
    close(connfd);
}
