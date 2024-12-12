#include "ServerSocket.hpp"
#include <cstring>

#include <unistd.h>

namespace pr {
    ServerSocket::ServerSocket(int port){
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if(fd < 0){
            perror("socket");
            return;
        }
        socketfd = fd ;//if socket was created succesfully, set Server fd to newly opened fd

        //declare the port and adress and so on
        struct sockaddr_in sin;
        memset((char*)&sin, 0, sizeof(sin));

        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);
        sin.sin_addr.s_addr = INADDR_ANY; //accept any adress on the machine

        //try to bind
        if(bind(fd, (struct sockaddr*)&sin, sizeof(sin)) < 0){
            //close the socket if it fails
            ::close(fd); 
            perror("bind");
            return;
        }

        if(listen(socketfd, 5) < 0){
            perror("listen");
            return;
        }

    }

    Socket ServerSocket::accept(){
        struct sockaddr_in sin;
        socklen_t len = sizeof(sin);

        //accept the connection on the socket
        int scom = ::accept(socketfd, (struct sockaddr*)&sin, &len);
        if(scom < 0){ //check for errors
            perror("accept");
            return Socket(); //return an empty socket ; maybe wrong
        }
        //return the socket
        return Socket(scom);
    }

	void ServerSocket::close(){
        if(socketfd != -1){
            ::close(socketfd);
            socketfd = -1;
        }
    }
};