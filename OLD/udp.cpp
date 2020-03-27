#include"udp.h"
#include<iostream>
#include<algorithm>
#include<string.h>
#include<dirent.h>

using namespace std;
#define BUFFER_SIZE 4048


void udp::turn_2_add(char *ipv6){
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


