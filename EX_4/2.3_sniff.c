#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <pcap.h>

pcap_t *handle;

/* 4 bytes IP address */
typedef struct ip_address{
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
}ip_address;

/* IPv4 header */
typedef struct ip_header{
    u_char  ver_ihl;        // Version (4 bits) + Internet header length (4 bits)
    u_char  tos;            // Type of service 
    u_short tlen;           // Total length 
    u_short identification; // Identification
    u_short flags_fo;       // Flags (3 bits) + Fragment offset (13 bits)
    u_char  ttl;            // Time to live
    u_char  proto;          // Protocol
    u_short crc;            // Header checksum
    ip_address  saddr;      // Source address
    ip_address  daddr;      // Destination address
    u_int   op_pad;         // Option + Padding
}ip_header;


/*This function will be invoked by pcap for each captured packet.We can process each packet inside the function.*/
void got_packet(u_char *args, const struct pcap_pkthdr *header,const u_char *packet)
{
	ip_header *ih= (ip_header *) (packet + 14); //length of ethernet header;
	ip_address tmp;
	
        
        /* Replacing src and dst */
        tmp = ih->saddr;
        ih->saddr = ih->daddr;
        ih->daddr = tmp;
        
        struct icmphdr *icmp_hdr = (struct icmphdr *)(packet + 14 + 20);
        icmp_hdr->type = ICMP_ECHOREPLY;
        
	/* Send down the packet */
	if (pcap_sendpacket(handle, packet, header->caplen) != 0)
    	{
        	fprintf(stderr,"\nError sending the packet: \n");
        	return;
    	}
    	printf( "Packet send\n");
}

int main()
{
	char errbuf[PCAP_ERRBUF_SIZE];
	struct bpf_program fp;
	char filter_exp[] = "icmp";
	bpf_u_int32 net;
	handle = pcap_open_live("br-37aac2ad9869", BUFSIZ, 1, 1000, errbuf);
	// Step 2: Compile filter_exp into BPF psuedo-code
	pcap_compile(handle, &fp, filter_exp, 0, net);
	if (pcap_setfilter(handle, &fp) !=0) 
	{
		pcap_perror(handle, "Error:");
		exit(EXIT_FAILURE);
	}
	// Step 3: Capture packets
	pcap_loop(handle, -1, got_packet, NULL);
	pcap_close(handle);   //Close the handle
	return 0;
}
