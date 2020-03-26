#include "net.h"
#include "tcp.h"
#include "tcp6.h"
#include "udp.h"
#include "udp6.h"

#include<iostream>
#include<ctype.h>
#include<math.h>
#include<iomanip>
#include<getopt.h>
#include<vector>
#include<algorithm>
#include<unistd.h>
#include<arpa/inet.h>
#include<dirent.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<string.h>
#include<netdb.h>
#include<fcntl.h>
#include<string.h>
#include<regex.h>

#define BUFFER_SIZE 4048
using namespace std;

int main(int argc,char*argv[]){

        //set getopt
        int t=0,u=0,print_pro =0;
        int opt;
        int digit_optind = 0;
        int option_index = 0;
        char short_opt[4024] = "tu";
        static struct option long_options[] = {
                {"tcp",optional_argument,NULL,'t'},
                {"udp",optional_argument,NULL,'u'},
                {0,0,0,0}
        };
        opt = getopt_long(argc,argv,short_opt,long_options,NULL);
        char filter[BUFFER_SIZE] = {'\0'};
        for(int i = 2;i<argc;i++){
                strcat(filter,argv[i]);
                strcat(filter," ");
        }
        regex_t reg ;
        regmatch_t match[1];
        regcomp(&reg,filter,REG_EXTENDED);

        switch(opt){
                case 't':
                        t = opt;
                        break;
                case 'u':
                        u = opt;
                        break;
        }

        print_pro =abs( t - u);


        vector<string>v;
        char buffer[BUFFER_SIZE];
        char name[BUFFER_SIZE];
        char path_tcp[]  = "/proc/net/tcp";
        char path_tcp6[] = "/proc/net/tcp6";
        char path_udp[]  = "/proc/net/udp";
        char path_udp6[] = "/proc/net/udp6";
        char tmp[BUFFER_SIZE];

        FILE *OPENtcp,*OPENtcp6,*OPENudp,*OPENudp6;
        OPENtcp = fopen(path_tcp,"r");
        OPENtcp6 = fopen(path_tcp6,"r");
        OPENudp = fopen(path_udp,"r");
        OPENudp6 = fopen(path_udp6,"r");

        vector<tcp*> T;
        vector<tcp6*> T6;
        vector<udp*> U;
        vector<udp6*> U6;

        if(OPENtcp!=NULL){
                fgets(buffer,BUFFER_SIZE-1,OPENtcp);
                memset(buffer,'\0',BUFFER_SIZE);
                while(fgets(buffer,BUFFER_SIZE-1,OPENtcp)!=NULL){
                        int num;
                        char proc[10] = "tcp";
                        tcp *TCP = new tcp(proc);
                        num = sscanf(buffer,"%d: %[^:]:%s %[^:]:%s %s %[^:]:%s %[^:]:%s %s %s %s %d",
                                        (int*)&TCP->sl,
                                        TCP->Local_address,
                                        TCP->Local_port,
                                        TCP->Rem_address,
                                        TCP->Rem_port,
                                        TCP->st,
                                        TCP->tx_queue,
                                        TCP->rx_queue,
                                        TCP->tr,
                                        TCP->tm_when,
                                        TCP->retrnsmt,
                                        TCP->uid,
                                        TCP->timeout,
                                        (int *)&TCP->inode
                        );
                        T.push_back(TCP);
                        memset(buffer,'\0',BUFFER_SIZE);
                }

        }

        if(OPENtcp6!=NULL){
                fgets(buffer,1024,OPENtcp6);
                memset(buffer,'\0',1024);
                while(fgets(buffer,1024,OPENtcp6)!=NULL){
                        int num;
                        char proc[10] = "tcp6";
                        tcp6 *TCP6 = new tcp6(proc);
                        num = sscanf(buffer,"%d: %[^:]:%s %[^:]:%s %s %[^:]:%s %[^:]:%s %s %s %s %d",
                                        (int *)&TCP6->sl,
                                        TCP6->Local_address,
                                        TCP6->Local_port,
                                        TCP6->Rem_address,
                                        TCP6->Rem_port,
                                        TCP6->st,
                                        TCP6->tx_queue,
                                        TCP6->rx_queue,
                                        TCP6->tr,
                                        TCP6->tm_when,
                                        TCP6->retrnsmt,
                                        TCP6->uid,
                                        TCP6->timeout,
                                        (int *)&TCP6->inode
                        );
                        T6.push_back(TCP6);
                        memset(buffer,'\0',1024);
                }

        }

        if(OPENudp!=NULL){
                fgets(buffer,1024,OPENudp);
                memset(buffer,'\0',1024);
                while(fgets(buffer,1024,OPENudp)!=NULL){
                        int num;
                        char proc[10] = "udp";
                        udp *UDP = new udp(proc);
                        num = sscanf(buffer,"%d: %[^:]:%s %[^:]:%s %s %[^:]:%s %[^:]:%s %s %s %s %d",
                                        (int *)&UDP->sl,
                                        UDP->Local_address,
                                        UDP->Local_port,
                                        UDP->Rem_address,
                                        UDP->Rem_port,
                                        UDP->st,
                                        UDP->tx_queue,
                                        UDP->rx_queue,
                                        UDP->tr,
                                        UDP->tm_when,
                                        UDP->retrnsmt,
                                        UDP->uid,
                                        UDP->timeout,
                                        (int *)&UDP->inode
                        );
                        U.push_back(UDP);
                        memset(buffer,'\0',1024);
                }

        }

        if(OPENudp6!=NULL){
                fgets(buffer,1024,OPENudp6);
                memset(buffer,'\0',1024);
                while(fgets(buffer,1024,OPENudp6)!=NULL){
                        int num;
                        char proc[10] = "udp6";
                        udp6 *UDP6 = new udp6(proc);
                        num = sscanf(buffer,"%d: %[^:]:%s %[^:]:%s %s %[^:]:%s %[^:]:%s %s %s %s %d",
                                        (int*)&UDP6->sl,
                                        UDP6->Local_address,
                                        UDP6->Local_port,
                                        UDP6->Rem_address,
                                        UDP6->Rem_port,
                                        UDP6->st,
                                        UDP6->tx_queue,
                                        UDP6->rx_queue,
                                        UDP6->tr,
                                        UDP6->tm_when,
                                        UDP6->retrnsmt,
                                        UDP6->uid,
                                        UDP6->timeout,
                                        (int *)&UDP6->inode
                        );
                        U6.push_back(UDP6);
                        memset(buffer,'\0',1024);
                }

        }

        fclose(OPENtcp);
        fclose(OPENtcp6);
        fclose(OPENudp);
        fclose(OPENudp6);

        printf("%-20s %-20s %-20s %s/%s\n","protocol","Local adress","Foreign address","PID","Process name and argument");

        switch(print_pro){
                case 't':{
                                 if(argc >2){
                                        for(int i = 2;i<argc;i++){
                                                for(int j = 0;j<T.size();j++){
                                                        T[j]->info();
                                                        if(!regexec(&reg,T[j]->pname,1,match,0)){
                                                                T[j]->prtOUPUT();
                                                        }
                                                }
                                                for(int j = 0;j<T6.size();j++){
                                                        T6[j]->info();
                                                        if(!regexec(&reg,T6[j]->pname,1,match,0)){
                                                                T6[j]->prtOUPUT();
                                                         }
                                                }
                                        }
                                }

                                else {
                                        for(int i = 0;i<T.size();i++){
                                                T[i]->info();
                                                T[i]->prtOUPUT();
                                        }
                                        for(int i = 0;i<T6.size();i++){
                                                 T6[i]->info();
                                                T6[i]->prtOUPUT();
                                        }
                                }
                                 break;
                        }
                case 'u':{
                                if(argc >2){
                                        for(int i = 2;i<argc;i++){
                                                for(int j = 0;j<U.size();j++){
                                                        U[j]->info();
                                                        if(!regexec(&reg,U[j]->pname,1,match,0)){
                                                                U[j]->prtOUPUT();
                                                        }
                                                }
                                                for(int j = 0;j<U6.size();j++){
                                                        U6[j]->info();
                                                        if(!regexec(&reg,U6[j]->pname,1,match,0)){
                                                                U6[j]->prtOUPUT();
                                                         }
                                                }
                                        }
                                }

                                else {
                                        for(int i = 0;i<U.size();i++){
                                                U[i]->info();
                                                U[i]->prtOUPUT();
                                        }
                                        for(int i = 0;i<U6.size();i++){
                                                U6[i]->info();
                                                U6[i]->prtOUPUT();
                                        }
                                }
                                 break;

                         }
                // nothing
                case 0:{

                                for(int i = 0;i<T.size();i++){
                                        T[i]->info();
                                        T[i]->prtOUPUT();
                                }

                                for(int i = 0;i<T6.size();i++){
                                        T6[i]->info();
                                        T6[i]->prtOUPUT();
                                }

                                for(int i = 0;i<U.size();i++){
                                        U[i]->info();
                                        U[i]->prtOUPUT();
                                }

                                for(int i = 0;i<U6.size();i++){
                                        U6[i]->info();
                                        U6[i]->prtOUPUT();
                                }

                        }

                // -t -u
                case 1: break;
	}
        return 0;
}

