#ifndef _NET_H_
#define _NET_H_

#include<arpa/inet.h>
#define BUFFER_SIZE 4048
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
		net(char*s);

		void a2i_port();

		virtual void turn_2_add(char *ipv6) = 0;

		void find_pname();

		void find_pid();

		void info();

		void prtOUPUT();
};
#endif
