#ifndef _UDP_H_
#define _UDP_H_

#include"net.h"
#define BUFFER_SIZE 4048
using namespace std;

class udp:public net{
	public:
		udp(char*s):net(s){}
		virtual void turn_2_add(char *ipv6);
};

#endif
