# include "headers.h"
void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    struct sniff_icmp *icmph;
    icmph = (struct sniff_icmp *)(packet+SIZE_ETHERNET+20);
     if(icmph->icmp_type != 8){
          return;
    }
    printf("Type: %d (Echo (ping) request)\n",icmph->icmp_type);
    printf("Code: %d\n",icmph->icmp_code);
    printf("checksum: %d\n",icmph->icmp_cksum);
    printf("seq: %d\n",icmph->icmp_seq);
     struct sniff_ip *iph;
    iph = ( struct sniff_ip *) (packet+ SIZE_ETHERNET-20 );
    printf("source ID: %s\n", inet_ntoa(iph->ip_src));  
    printf("destination ID: %s\n", inet_ntoa(iph->ip_dst));
 
     }
int main(int argc, char **argv) {
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program fp;
     char filter_exp[] = "icmp";
    bpf_u_int32 net;
    // Step 1: Open live pcap session on NIC with name "eth0"
    handle = pcap_open_live("eth0", BUFSIZ, 1, 1000, errbuf);

    // Step 2: Compile filter_exp into BPF psuedo-code
    pcap_compile(handle, &fp, filter_exp, 0, net);
    pcap_setfilter(handle, &fp);

    // Step 3: Capture packets
    pcap_loop(handle, -1, got_packet, NULL);
    pcap_close(handle);

    //Close the handle
    return 0;
}