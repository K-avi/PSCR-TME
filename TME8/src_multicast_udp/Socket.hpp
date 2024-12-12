//unchanged from TCP version ; would have been cleaner to be able to either 
//create UDP or TCP sockets but eh, as long as it works
#ifndef SRC_SOCKET_H_
#define SRC_SOCKET_H_

#include <netinet/ip.h>
#include <string>
#include <iosfwd>
#include <vector>

namespace pr {
class Socket {
	int fd;
	struct sockaddr_in addr; //the send / recv methods will be supported 
	//in the socket so store the address of the server here

public :
	Socket():fd(-1){}
	Socket(int fd):fd(fd){}
	Socket(struct sockaddr_in& addr):fd(-1), addr(addr){}

	void open_multicast_sender(const std::string & host, int port);
	void open_multicast_receiver(const std::string & host, int port);
	
	
	bool isOpen() const {return fd != -1;}
	int getFD() { return fd ;}

	std::string recv();

	void send(const std::string & s);

	void close();
};

std::ostream & operator<< (std::ostream & os, struct sockaddr_in * addr);

}

#endif /* SRC_SOCKET_H_ */
