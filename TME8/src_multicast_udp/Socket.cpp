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

//very similar to the TCP version; main difference is that we use 
//SOCK_DGRAM instead of sock stream and use bind instead of connect
using namespace std;

namespace pr {

    void Socket::open_multicast_receiver(const string & ip, int port) {
        int sc; 

        //create socket 
        if((sc = socket(AF_INET, SOCK_DGRAM,0)) < 0){
            perror("socket"); 
            return;
        }


        int yes = 1 ; 
        if(setsockopt(sc, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0){
            perror("setsockopt");
            return;
        }

        struct ip_mreq imr ; //for setsockopt
        memset(&imr, 0, sizeof(ip_mreq));

        imr.imr_multiaddr.s_addr = inet_addr(ip.c_str());
        imr.imr_interface.s_addr = htonl(INADDR_ANY);

        if(setsockopt(sc, IPPROTO_IP, IP_ADD_MEMBERSHIP, &imr, sizeof(ip_mreq))<0){
            perror("setsockopt");
            return;
        }
        //get host
        //could be done with getaddrinfo ; might be better?
        struct hostent *hostinfo = gethostbyname(ip.c_str());
        if(hostinfo == NULL){
            perror("gethostbyname");
            return;
        }

    
        memset((char*)&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        memcpy((char*)&addr.sin_addr, (char*)hostinfo->h_addr, hostinfo->h_length);

        //bind the socket
        if(bind(sc, (struct sockaddr*)&addr, sizeof(addr)) < 0){
            perror("bind");
            return;
        }
        fd = sc;
    }//dubious

    void Socket::open_multicast_sender(const string& ip, int port){
        int sc; 
        int scom; 
        int cpt; 
        char buffer[1024];

        //create socket 
        if((sc = socket(AF_INET, SOCK_DGRAM,0)) < 0){
            perror("socket"); 
            return;
        }

        //get host
        //could be done with getaddrinfo ; might be better?
        struct hostent *hostinfo = gethostbyname(ip.c_str());
        if(hostinfo == NULL){
            perror("gethostbyname");
            return;
        }

        //connect
        memset((char*)&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        memcpy((char*)&addr.sin_addr, (char*)hostinfo->h_addr, hostinfo->h_length);

        fd = sc;

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

    string Socket::recv(){
        socklen_t addr_size = sizeof(addr);
        char buffer[1024];
        int n;
        if((n = recvfrom(fd, buffer, 1024, 0, (struct sockaddr*)&addr, &addr_size) < 0)){
            perror("recvfrom");
            cout << "error fd is : " << fd << endl;
            exit(1);
        }
        buffer[n+1] = '\0';
        return string(buffer);
    }

    void Socket::send(const string & s){
        sendto(fd, s.c_str(), s.size(), 0, (struct sockaddr*)&addr, sizeof(addr));
    }
};
