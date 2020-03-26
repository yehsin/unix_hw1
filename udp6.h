#ifndef _UDP6_H_
#define _UDP6_H_

#include"net.h"

#define BUFFER_SIZE 4048
using namespace std;

class udp6:public net{
	public:
		udp6(char *s) : net(s){}

		virtual void turn_2_add(char *ipv6);
};

#endif
