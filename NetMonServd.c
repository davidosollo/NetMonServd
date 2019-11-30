/////////////////////////////////////////////////////////////////////////////
//  Source:      NetMonServd.c
//  Project:     NeMonServd
//  Subject:     Aplicacion y diseño de sistemas embebidos
//  Univerisity: Universidad Autonoma de Guadalajara
//  Description: Deamon to monitor the network activity
//               
//

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "linked_list.h"

#define MAXCHAR 1000
#define MIN_LEN_VALID 159

struct node *head = NULL;
struct node *current = NULL;

FILE *fp_tcpmon= NULL;
FILE *fpNetLogOut= NULL;

//////////////////////////////////////////////////////////
// read_records() 
//////////////////////////////////////////////////////////
// Read the outputs generated dor tcpdum command
// and add the connection to a linked list and
// write the /tcp_logs/netMonReport.log to 
// report the network activity
//////////////////////////////////////////////////////////
void read_records()
{
   char str[MAXCHAR];
   char str_rec_time[MAXCHAR];
   char str_rec_mac_ori[MAXCHAR];
   char str_rec_mac_dst[MAXCHAR];
   char str_len_pack[MAXCHAR];
   char str_ip_sender[MAXCHAR];
   char str_ip_receiver[MAXCHAR];

   time_t current_time;
   char* c_time_string;

   static int i=0;
   char *position_ptr= NULL;
   char *position_ptr2= NULL; 

   fp_tcpmon = fopen ("./tcp_logs/tcpmon.log_p", "r");
   fpNetLogOut = fopen ("./tcp_logs/netMonReport.log", "w+");

   while (fgets(str, MAXCHAR, fp_tcpmon) != NULL)
   {

      
      /* Obtain current time. */
      current_time = time(NULL);
      /* Convert to local time format. */
      c_time_string = ctime(&current_time);

      if(strlen(str) > MIN_LEN_VALID)
      { 
         strncpy(str_rec_time,str,15);
         strncpy(str_rec_mac_ori ,str + 16,17);
         position_ptr = strchr(str,  '>');
         strncpy(str_rec_mac_dst ,position_ptr +2 ,17);
         position_ptr = strstr(position_ptr, "length");
         position_ptr2 = strstr(position_ptr,":");

         if(!position_ptr) break;
         if(!position_ptr2) break;

         strncpy(str_len_pack ,position_ptr +8 ,position_ptr2 - position_ptr - 8);
         position_ptr = strchr(position_ptr2 +1,  '>');
         if(!position_ptr) break;

         memset(str_ip_sender,0,MAXCHAR);
         strncpy(str_ip_sender ,position_ptr2 +2 ,position_ptr - position_ptr2 -2);
         position_ptr2 = strchr(position_ptr + 1,':');
         if(!position_ptr2) break;

         memset(str_ip_receiver,0,MAXCHAR);
         strncpy(str_ip_receiver ,position_ptr +2 ,position_ptr2 - position_ptr -2);

         current = search(head, str_rec_mac_ori, str_rec_mac_dst, str_ip_sender,str_ip_receiver);

         //New Connection
         if(!current)
         {
            head = prepend(head, i);
            memset(head->sTimeIni,0,NODE_FIELD_LEN);
            strncpy(head->sTimeIni,c_time_string,19);
            memset(head->sTimeLast,0,NODE_FIELD_LEN);
            strncpy(head->sTimeLast,c_time_string,19);
            strcpy(head->sMacSend,str_rec_mac_ori);
            strcpy(head->sMacRec,str_rec_mac_dst);
            strcpy(head->sIPSend,str_ip_sender);
            strcpy(head->sIPRec,str_ip_receiver);
            head->iNumBytes = atoi(str_len_pack);
            head->iNumPaquetes = 1;
            i++;
         }   
         else
         {
            //If connection already exist Just Update
            current->iNumPaquetes ++;
            current->iNumBytes = current->iNumBytes + atoi(str_len_pack);
            memset(current->sTimeLast,0,NODE_FIELD_LEN);
            strncpy(current->sTimeLast,c_time_string,19);

         }      
      }
   }
   display_all(head,fpNetLogOut);
   fclose(fp_tcpmon);
   fclose(fpNetLogOut);
}


//////////////////////////////////////////////////////////
// Main  
//////////////////////////////////////////////////////////
// Launch NetMonServd Deamon itself
// And launch the tcpdump -e runnig in background 
//////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
   FILE *fp= NULL;
   pid_t process_id = 0;
   pid_t sid = 0;
   // Create child process
   process_id = fork();
   // Indication of fork() failure
   if (process_id < 0)
   {
     printf("fork failed!\n");
     // Return failure in exit status
     exit(1);
   } 
   // PARENT PROCESS. Need to kill it.
   if (process_id > 0)
   {
      printf("process_id of child process %d \n", process_id);
      // return success in exit status
      exit(0);
   }
   //unmask the file mode
   umask(0);
   //set new session
   sid = setsid();
   if(sid < 0)
   {
      // Return failure
      exit(1);
   }
   // Change the current working directory to root.
   //chdir("/");
   // Close stdin. stdout and stderr
   close(STDIN_FILENO);  
   close(STDOUT_FILENO);
   close(STDERR_FILENO);

   int i=0;
   char cmd[200]; 
   int cmdRes;

   // Open a log file in write mode.
   fp = fopen ("./tcp_logs/tcpmon.log_p", "w+");
   fprintf(fp, "Logging info...%d\n",i);
   cmdRes=system("tcpdump -e> ./tcp_logs/tcpmon.log &");

   fprintf(fp, "System Result ...%d\n",i);

   while (1)
   {
     //Dont block context switches, let the process sleep for some time
     sleep(20);
     fprintf(fp, "Logging info...%d\n",i);
     fflush(fp);   
     system("ps -ef | grep tcpdump | grep -v grep | awk '{print $2}' | xargs kill -9 >/dev/null 2>&1");

     sprintf(cmd,"mv ./tcp_logs/tcpmon.log ./tcp_logs/tcpmon.log_p >/dev/null 2>&1",i);
     system(cmd);
     cmdRes=system("tcpdump -e> ./tcp_logs/tcpmon.log &");
     sleep(2);
     read_records();
     // Implement and call some function that does core work for this daemon.
     i++;
   }
   
   fclose(fp);   
   return (0);

}
