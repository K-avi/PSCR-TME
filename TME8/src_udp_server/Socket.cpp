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

    void Socket::bind(const string & host, int port) {
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
        struct hostent *hostinfo = gethostbyname(host.c_str());
        if(hostinfo == NULL){
            perror("gethostbyname");
            return;
        }

        //connect
        memset((char*)&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        memcpy((char*)&addr.sin_addr, (char*)hostinfo->h_addr, hostinfo->h_length);

        if(::bind(sc, (struct sockaddr*)&addr, sizeof(addr)) < 0){
            perror("bind");
            return;
        }
        fd = sc;
    }//dubious

    void Socket::open(const string & host, int port) {
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
        struct hostent *hostinfo = gethostbyname(host.c_str());
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
    }//dubious

	void Socket::bind(in_addr ipv4, int port){
        //create a socket
        int s = socket(AF_INET, SOCK_DGRAM, 0);
        if (s == -1) {
            perror("socket");
            return ;
        }

        //bind it
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port); //converts the port number to network byte order
        addr.sin_addr = ipv4;

        if(::bind(s, (struct sockaddr*)&addr, sizeof(addr)) == -1){
            perror("connect");
            return;
        }        
        fd = s;
    }

    void Socket::open(in_addr ipv4, int port){
        //create a socket
        int s = socket(AF_INET, SOCK_DGRAM, 0);
        if (s == -1) {
            perror("socket");
            return ;
        }

        //bind it
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port); //converts the port number to network byte order
        addr.sin_addr = ipv4;
            
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

    vector<string> Socket::recv(int nb_msg){
        vector<string> res;
        socklen_t addr_size = sizeof(addr);
        for(int i = 0 ; i < nb_msg ; i++){
            char buffer[1024];
            int n;
            if((n = recvfrom(fd, buffer, 1024, 0, (struct sockaddr*)&addr, &addr_size) < 0)){
                perror("recvfrom");
                cout << "error" ;
                return res;
            }
            buffer[n+1] = '\0';
            res.push_back(string(buffer));
        }
        return res;
    }

    void Socket::send(const string & s){
        sendto(fd, s.c_str(), s.size(), 0, (struct sockaddr*)&addr, sizeof(addr));
    }
};
