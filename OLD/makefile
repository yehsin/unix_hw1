UP_CC= g++

CFLAG= -g -Wall

OBJ =hw.o net.o tcp.o tcp6.o udp.o udp6.o

all:hw1

hw1 :${OBJ}
	${UP_CC} ${CFLAG} ${OBJ} -o hw1
hw.o:hw.cpp
net.o : net.cpp net.h
tcp.o : tcp.cpp tcp.h
tcp6.o : tcp6.cpp tcp6.h
udp.o : udp.cpp udp.h
udp6.o : udp6.cpp udp6.h

.PYTHON :clean

clean:
	rm *.o hw1


