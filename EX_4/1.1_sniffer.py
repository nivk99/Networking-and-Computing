#!/usr/bin/env python3
#
from scapy.all import *

def print_packet(pkt):
	pkt.show()
	
pkt = sniff(iface='br-37aac2ad9869', filter='icmp', prn=print_packet)
#pkt = sniff(iface='br-37aac2ad9869', filter='tcp and dst 10.9.0.6 and (port 23) ', prn=print_packet)
#pkt = sniff(iface='br-37aac2ad9869', filter="tcp and src='128.230.0.0/16'", prn=print_packet)

