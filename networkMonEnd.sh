#!/bin/bash
##################################################################
#  Source:      networkMonEnd.sh
#  Project:     NeMonServd
#  Subject:     Aplicacion y diseño de sistemas embebidos
#  Univerisity: Universidad Autonoma de Guadalajara
#  Description: Kill the NetMonServd deamon and kill tcpdump process
#               
#

echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
echo "Finishing Network Monitoring"
echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
ps -ef | grep "NetMonServd"| grep -v grep | awk '{print $2}' | xargs kill -9
ps -ef | grep "tcpdump" | grep -v grep | awk '{print $2}' | xargs kill -9N
