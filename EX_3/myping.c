
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/time.h> // gettimeofday()
#include <fcntl.h>
#include <resolv.h>
#include <netdb.h>
#define IP4_HDRLEN 20
#define ICMP_HDRLEN 8 // type 8 to ping request
// Checksum algo
unsigned short calculate_checksum(unsigned short * paddress, int len);

#define DESTINATION_IP "1.1.1.1"//Cloudflare
int main(int argc, char const *argv[])
{
    clock_t start, end;
    int sock,msg_len;
    socklen_t dest_size;//sizeof
    char packet[256];//buffer
    struct sockaddr_in dest;
    struct icmp *icmphdr;//prto icmp

    sock = socket (AF_INET, SOCK_RAW, IPPROTO_ICMP);//the raw sock in prto icmp;
    if(sock==-1){
        perror("!error in socket!\n");
        exit(1);
    }





    //===================
    // sockaddr_in
    //===================
   dest_size =sizeof(dest);
   bzero(&dest, sizeof(dest));
  dest.sin_family = AF_INET;// ip v4
 inet_pton (AF_INET, DESTINATION_IP, (struct in_addr*)&dest.sin_addr.s_addr);//big andian

   char *data = "This is the ping.\n";
   int datalen = strlen(data);

     // header
   memcpy ((packet ), &icmphdr, ICMP_HDRLEN);
    memcpy (packet + ICMP_HDRLEN, data, datalen);

    //===================
    // ICMP header
    //===================
  icmphdr =(struct icmp*)packet;
   icmphdr->icmp_type= ICMP_ECHO;
   icmphdr->icmp_code=0;
   icmphdr->icmp_cksum=0;
   icmphdr->icmp_id = 18;
    icmphdr->icmp_seq = 0;
   icmphdr->icmp_cksum= calculate_checksum((unsigned short *) (packet ), sizeof(packet));

 start = clock();
   int send;
   send= sendto (sock, packet, sizeof(packet), 0, (struct sockaddr *) &dest, dest_size);
   if(send==-1){
       perror("!error in sendto!\n");
       exit(1);
   }
 close(sock);

///////listener

int sd;
struct sockaddr_in addr;
unsigned char buf[1028];
sd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
int bytes, len=sizeof(addr);
bzero(buf, sizeof(buf));
 end = clock();
bytes = recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr*)&addr, &len);
struct iphdr *ip = buf;
struct sockaddr_in *dest1;
struct icmphdr *icmp =buf+ip->ihl*4;

///print the ip 
char ip4[INET_ADDRSTRLEN];
inet_ntop(AF_INET,&(addr.sin_addr),ip4,INET_ADDRSTRLEN);
printf("The IPV4 is: %s\n",ip4);

///print the adder ip
printf("IPv%d: hdr-size=%d pkt-size=%d protocol=%d TTL=%d\n",
		ip->version, ip->ihl*4, ntohs(ip->tot_len), ip->protocol,
		ip->ttl+1);

///print the prot Icmp
printf("ICMP: type[%d/%d] checksum[%d] id[%d] seq[%d]\n",
	icmp->type, icmp->code, ntohs(icmp->checksum),
	icmp->un.echo.id, icmp->un.echo.sequence);


///print time
double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
 printf( "RTT is %f , start: %ld , end: %ld , CLOCKS_PER_SEC=%ld\n", cpu_time_used, start, end, CLOCKS_PER_SEC);




close(sd);




    return 0;
}




// Compute checksum (RFC 1071).
unsigned short calculate_checksum(unsigned short * paddress, int len)
{
	int nleft = len;
	int sum = 0;
	unsigned short * w = paddress;
	unsigned short answer = 0;

	while (nleft > 1)
	{
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1)
	{
		*((unsigned char *)&answer) = *((unsigned char *)w);
		sum += answer;
	}

	// add back carry outs from top 16 bits to low 16 bits
	sum = (sum >> 16) + (sum & 0xffff); // add hi 16 to low 16
	sum += (sum >> 16);                 // add carry
	answer = ~sum;                      // truncate to 16 bits

	return answer;
}
