#!/usr/bin/env python3
#

from scapy.all import *
a = IP()
a.ttl = int(sys.argv[1])
a.dst='1.2.3.4'
b=ICMP()
send(a/b)


