#!/usr/bin/env python3
#
from scapy.all import *

def print_packet(pkt):
	if pkt[ICMP].type == 8: #request
		p = IP(src=pkt[IP].dst, dst=pkt[IP].src)/ICMP(type=0,code=0,seq=pkt[ICMP].seq, id=pkt[ICMP].id)/Raw('11111111111111111111111111111111111112222222234444444444')
		send(p)
	
pkt = sniff(iface='br-37aac2ad9869', filter='icmp', prn=print_packet)

