#ifndef _TCP6_H_
#define _TCP6_H_

#include"net.h"

class tcp6:public net{
	public:
		tcp6(char*s):net(s){}

		virtual void turn_2_add(char *ipv6);

};

#endif
