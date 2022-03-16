#!/usr/bin/env python3
#

from scapy.all import *

def p_pkt(pkt):
	#pkt.show()
	if pkt[IP].src != '10.9.0.5': 
		print("router=", pkt[IP].src)

pkt = sniff(iface='br-37aac2ad9869', filter='icmp', prn=p_pkt)	
#pkt = sniff(iface='br-37aac2ad9869', filter='tcp and dst 10.9.0.6 and (port 23) ', prn=p_pkt)
#pkt = sniff(iface='br-37aac2ad9869', filter="tcp and src='128.230.0.0/16'", prn=p_pkt)

