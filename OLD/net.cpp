#include"net.h"
#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<iostream>
#include<math.h>
#include<vector>
#include<unistd.h>
#include<sys/stat.h>

#define BUFFER_SIZE 4048
using namespace std;

net::net(char *s) { strcpy(pro,s);};

void net::a2i_port(){
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

void net::find_pname(){
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


void net::find_pid(){
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

void net::info(){
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




void net::prtOUPUT(){

	printf("%-20s %-20s %-20s %s/%s\n",

			pro,

			Local,

			Forigen,

			PID_num,

			pname

      	      );

}


