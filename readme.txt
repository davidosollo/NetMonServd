
Compile the Source:
gcc -o NetMonServd  NetMonServd.c linked_list.c


Lauch:
Where is tpcdump command copy these files 

networkMon.sh
networkMonEnd.sh
NetMonServd

Start Monitoring the Network Activity:

pi@raspberrypi:~/tcpdump/tcpdump $ sudo ./networkMon.sh
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Starting Network Monitoring
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
process_id of child process 16365 

The logs will be in ./tcp_logs

pi@raspberrypi:~/tcpdump/tcpdump/tcp_logs $ ll
total 216
-rw-rw-rw- 1 root root 88281 nov 30 19:00 netMonReport.log
-rw-rw-rw- 1 root root 40960 nov 30 19:00 tcpmon.log
-rw-rw-rw- 1 root root 90112 nov 30 19:00 tcpmon.log_p

The file netMonReport.log contain the Network activity log

This is an example of output log

-------------------------------------------
Mac Sender:     70:03:7e:86:92:f2
IP Sender:      dns1-LEON-megacable.com.mx.domain 
Mac Receiver:   dc:a6:32:0b:3e:db
IP Receiver:    192.168.0.12.46368
Package Count:  1
Bytes Number:   624
First Time:     Sat Nov 30 17:19:36
Last Time:      Sat Nov 30 17:19:36
-------------------------------------------
Mac Sender:     dc:a6:32:0b:3e:db
IP Sender:      192.168.0.12.46368 
Mac Receiver:   70:03:7e:86:92:f2
IP Receiver:    dns1-LEON-megacable.com.mx.domain
Package Count:  1
Bytes Number:   524
First Time:     Sat Nov 30 17:19:36
Last Time:      Sat Nov 30 17:19:36
-------------------------------------------

To stop Monitoring the network activity execute this command:

pi@raspberrypi:~/tcpdump/tcpdump $ sudo ./networkMonEnd.sh
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Finishing Network Monitoring
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



