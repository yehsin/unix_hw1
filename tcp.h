#ifndef _TCP_H_
#define _TCP_H_

#include"net.h"

class tcp : public net{
	public:
		tcp(char *s) : net(s) {}
		virtual void turn_2_add(char*ipv6);
};

#endif
