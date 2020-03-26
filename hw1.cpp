#include<iostream>
#include<regex.h>
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
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<netdb.h>
#include<fcntl.h>
#include<errno.h>
#include<string>
#include<regex>

#define BUFFER_SIZE 4048
using namespace std;

class net{
	private:
		int protocol = 4;
		unsigned char Real_add[sizeof(struct in6_addr)];
		char pro[10] ;
	public:
		
		int PID = -1;
		char PID_num[BUFFER_SIZE];

		int sl;
		char Local_address[BUFFER_SIZE];

		char Local_port[BUFFER_SIZE];
		int  local_port = 0;

		char Rem_address[BUFFER_SIZE];

		char Rem_port[BUFFER_SIZE];
		int  rem_port = 0;

		char  st[BUFFER_SIZE];
		char  tx_queue[BUFFER_SIZE];
		char  rx_queue[BUFFER_SIZE];
		char  tr[BUFFER_SIZE];
		char  tm_when[BUFFER_SIZE];
		char  retrnsmt[BUFFER_SIZE];
		char  uid[BUFFER_SIZE];
		char  timeout[BUFFER_SIZE];

		int   inode;
		char Local[BUFFER_SIZE];
		char Forigen[BUFFER_SIZE];
		char pname[BUFFER_SIZE];
	public:
		net(char *s) { strcpy(pro,s);};

		void a2i_port(){
			for(int i = 0;i<4;i++){
				if(Local_port[i]<65) local_port += (Local_port[i]-48)*pow(16,(3-i));
				else local_port += (Local_port[i] - 65 +10)*pow(16,(3-i));

				if(Rem_port[i]<65) rem_port += (Rem_port[i]-48)*pow(16,(3-i));
				else rem_port += (Rem_port[i] - 65 + 10)*pow(16,(3-i));
			}

			if(local_port == 0) strcpy(Local_port,"*");
			else{
				char tmp[BUFFER_SIZE];
				sprintf(tmp,"%d",local_port);
				strcpy(Local_port,tmp);
			}

			if(rem_port == 0) strcpy(Rem_port,"*");
                        else{
                                char tmp[BUFFER_SIZE];
                                sprintf(tmp,"%d",rem_port);
                                strcpy(Rem_port,tmp);
                        }

		}


		virtual void turn_2_add(char *ipv6) = 0;//virtual function
		
		void find_pname(){
			if(PID >0){

				char comm_path[BUFFER_SIZE],cmdline_path[BUFFER_SIZE];
				char cmd[BUFFER_SIZE] = {'\0'},comm[BUFFER_SIZE],argu[BUFFER_SIZE] = {'\0'};
				char tmp[BUFFER_SIZE] = {'\0'};
				FILE *comm_fd,*cmdline_fd;

				sprintf(comm_path,"/proc/%d/comm",PID);
				sprintf(cmdline_path,"/proc/%d/cmdline",PID);

				comm_fd = fopen(comm_path,"r");
				cmdline_fd = fopen(cmdline_path,"r");
				if(comm_fd != NULL){
					fgets(comm,sizeof(comm),comm_fd);
					comm[strlen(comm) -1] = ' ';
				}
				if(cmdline_fd != NULL){
					fgets(cmd,sizeof(cmd),cmdline_fd);
					sscanf(cmd,"%*[^ ]%*[ ]%[^\n]",argu);
				}

				sprintf(pname,"%s %s",comm,argu);
				fclose(comm_fd);
				fclose(cmdline_fd);
			}
		}

		void find_pid(){

			struct dirent *dir;
			DIR *proc_dir;
			vector<string> pids;
			vector<int> pid;
			proc_dir = opendir("/proc/");

			while(dir = readdir(proc_dir)){
				pids.push_back(dir->d_name);
			}
			closedir(proc_dir);

			for(int i = 0;i<pids.size();i++){
				int num = atoi(pids[i].c_str());
				if(num != 0) pid.push_back(num);
			}

			for(int i = 0;i<pid.size();i++){
				char Buffer[BUFFER_SIZE];
				struct dirent *fd;
				DIR *currentfile;
				
				memset(Buffer,'\0',BUFFER_SIZE);
				sprintf(Buffer,"/proc/%d/fd/",pid[i]);

				if(!eaccess(Buffer,R_OK)){
						
					currentfile = opendir(Buffer);
				

					while(fd = readdir(currentfile)){
						char str[3000];
						char link[2048];
						char link_buffer[2048];
						sprintf(link,"/proc/%d/fd/%s",pid[i],fd->d_name);
						sprintf(str,"socket:[%d]",inode);
						
						if(!eaccess(link,R_OK)){

							struct stat buf;
							int stmode;
							stat(link,&buf);
							if(!S_ISLNK(buf.st_mode)){
								if(readlink(link,link_buffer,sizeof(link_buffer))!= -1){
									int lnkbuf_prov = 0;
									sscanf(link_buffer,"socket:[%d]",&lnkbuf_prov);
									if(inode == lnkbuf_prov) PID = pid[i];
								}
							}

						}		
					}
					if(PID >0) sprintf(PID_num,"%d",PID);

					closedir(currentfile);
					currentfile = nullptr;
				}
									
			}
		}

		void info(){
			a2i_port();
			find_pid();
			turn_2_add(Local_address);
			turn_2_add(Rem_address);
			find_pname();
			if(PID<0){
				strcpy(PID_num,"-");
				strcpy(pname,"-");
			}

			sprintf(Local,"%s:%s",Local_address,Local_port);
			sprintf(Forigen,"%s:%s",Rem_address,Rem_port);
		}


		virtual void prtOUPUT(){
			printf("%-20s %-20s %-20s %s/%s\n",
					pro,
					Local,
					Forigen,
					PID_num,
					pname
					
			);	
		}
};

class tcp : public net{
	public:
		tcp(char *s):net(s){}
		
		void turn_2_add(char *ipv6){
			char pre[] = "0000000000000000FFFF0000";
			strcat(pre,ipv6);
			strcpy(ipv6,pre);

			for(int i = 0;i<strlen(ipv6);i+=8){
                                reverse(ipv6+i,ipv6+i+8);
                        }
                        for(int i = 0;i<strlen(ipv6);i+=2){
                                reverse(ipv6+i,ipv6+i+2);
                        }
                        int count = 0;
                        char tmp[BUFFER_SIZE];
                        for(int i = 0;i<strlen(ipv6);i++){
                                tmp[count++] = ipv6[i];
                                if(i!=0 && i!=strlen(ipv6)-1 && (i+1)%4 == 0) tmp[count++] = ':';
                        }
                        tmp[count] = '\0';
                        strcpy(ipv6,tmp);
                      //  cout<<ipv6<<endl;
                        int s;
                        char str[INET6_ADDRSTRLEN];
                        char buf[sizeof(struct in6_addr)];
                        s = inet_pton(AF_INET6,ipv6,buf);
                        if(s<=0) perror("error");
                        if(inet_ntop(AF_INET6,buf,str,INET6_ADDRSTRLEN) == NULL){
                                perror("error");
                        }

			strcpy(ipv6,str);
			char cut[BUFFER_SIZE];
			sscanf(ipv6,"::ffff:%s",cut);
			strcpy(ipv6,cut);


                }

};

class tcp6 : public net{

	public:
		tcp6(char *s):net(s){}
		virtual void turn_2_add(char *ipv6){
			for(int i = 0;i<strlen(ipv6);i+=8){
                		reverse(ipv6+i,ipv6+i+8);
        		}
        		for(int i = 0;i<strlen(ipv6);i+=2){
                		reverse(ipv6+i,ipv6+i+2);
        		}
        		int count = 0;
        		char tmp[BUFFER_SIZE];
        		for(int i = 0;i<strlen(ipv6);i++){
                		tmp[count++] = ipv6[i];
                		if(i!=0 && i!=strlen(ipv6)-1 && (i+1)%4 == 0) tmp[count++] = ':';
        		}
        		tmp[count] = '\0';
        		strcpy(ipv6,tmp);
        		//cout<<ipv6<<endl;
        		int s;
        		char str[INET6_ADDRSTRLEN];
        		char buf[sizeof(struct in6_addr)];
        		s = inet_pton(AF_INET6,ipv6,buf);
        		if(s<=0) perror("error");
        		if(inet_ntop(AF_INET6,buf,str,INET6_ADDRSTRLEN) == NULL){
                		perror("error");
        		}
			strcpy(ipv6,str);
	
		}
};

class udp : public net{
	public:
		udp(char *s) : net(s){}
		void turn_2_add(char *ipv6){
                        char pre[] = "0000000000000000FFFF0000";
                        strcat(pre,ipv6);
                        strcpy(ipv6,pre);

                        for(int i = 0;i<strlen(ipv6);i+=8){
                                reverse(ipv6+i,ipv6+i+8);
                        }
                        for(int i = 0;i<strlen(ipv6);i+=2){
                                reverse(ipv6+i,ipv6+i+2);
                        }
                        int count = 0;
                        char tmp[BUFFER_SIZE];
                        for(int i = 0;i<strlen(ipv6);i++){
                                tmp[count++] = ipv6[i];
                                if(i!=0 && i!=strlen(ipv6)-1 && (i+1)%4 == 0) tmp[count++] = ':';
                        }
                        tmp[count] = '\0';
                        strcpy(ipv6,tmp);
                        //cout<<ipv6<<endl;
                        int s;
                        char str[INET6_ADDRSTRLEN];
                        char buf[sizeof(struct in6_addr)];
                        s = inet_pton(AF_INET6,ipv6,buf);
                        if(s<=0) perror("error");
                        if(inet_ntop(AF_INET6,buf,str,INET6_ADDRSTRLEN) == NULL){
                                perror("error");
                        }

                        strcpy(ipv6,str);
                        char cut[BUFFER_SIZE];
                        sscanf(ipv6,"::ffff:%s",cut);
                        strcpy(ipv6,cut);

		}

};

class udp6 : public net{
	public:
		udp6(char *s) : net(s){}
		virtual void turn_2_add(char *ipv6){
                        for(int i = 0;i<strlen(ipv6);i+=8){
                                reverse(ipv6+i,ipv6+i+8);
                        }
                        for(int i = 0;i<strlen(ipv6);i+=2){
                                reverse(ipv6+i,ipv6+i+2);
                        }
                        int count = 0;
                        char tmp[BUFFER_SIZE];
                        for(int i = 0;i<strlen(ipv6);i++){
                                tmp[count++] = ipv6[i];
                                if(i!=0 && i!=strlen(ipv6)-1 && (i+1)%4 == 0) tmp[count++] = ':';
                        }
                        tmp[count] = '\0';
                        strcpy(ipv6,tmp);
                        //cout<<ipv6<<endl;
                        int s;
                        char str[INET6_ADDRSTRLEN];
                        char buf[sizeof(struct in6_addr)];
                        s = inet_pton(AF_INET6,ipv6,buf);
                        if(s<=0) perror("error");
                        if(inet_ntop(AF_INET6,buf,str,INET6_ADDRSTRLEN) == NULL){
                                perror("error");
                        }
			strcpy(ipv6,str);
                }

};

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

//	cout<<opt<<endl;
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
