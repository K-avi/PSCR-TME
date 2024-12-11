#include "Socket.hpp"

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>

#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>


using namespace std;

namespace pr {

    void Socket::connect(const string & host, int port) {
        struct sockaddr_in sin; 
        int sc; 
        int scom; 
        int cpt; 
        char buffer[1024];

        //create socket 
        if((sc = socket(AF_INET, SOCK_STREAM,0)) < 0){
            perror("socket"); 
            return;
        }

        //get host
        //could be done with getaddrinfo ; might be better?
        struct hostent *hostinfo = gethostbyname(host.c_str());
        if(hostinfo == NULL){
            perror("gethostbyname");
            return;
        }

        //connect
        memset((char*)&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);
        memcpy((char*)&sin.sin_addr, (char*)hostinfo->h_addr, hostinfo->h_length);

        if(::connect(sc, (struct sockaddr*)&sin, sizeof(sin)) < 0){
            perror("connect");
            return;
        }
        fd = sc;
    }//dubious

	void Socket::connect(in_addr ipv4, int port){
        //create a socket
        int s = socket(AF_INET, SOCK_STREAM, 0);
        if (s == -1) {
            perror("socket");
            return ;
        }

        //connect it
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port); //converts the port number to network byte order
        addr.sin_addr = ipv4;

        if(::connect(s, (struct sockaddr*)&addr, sizeof(addr)) == -1){
            perror("connect");
            return;
        }        
        fd = s;
    }

	void Socket::close(){
        if (fd != -1) {
            ::close(fd);
            fd = -1;
        }
    }

    ostream & operator<< (ostream & os, struct sockaddr_in * addr) {
        string ip = inet_ntoa(addr->sin_addr);
        char* host = new char[NI_MAXHOST];
        char* serv = new char[NI_MAXSERV];//numeric serv allows to get the port number
        getnameinfo((sockaddr*) addr, sizeof(sockaddr_in), host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICSERV);
        
        os << "IP: " << ip << " Host: " << host << " Port: " << serv << endl;
        delete[] host;
        delete[] serv;

        return os;
    }//very dubious 
};
