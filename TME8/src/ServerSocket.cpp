#include "ServerSocket.hpp"
#include <cstring>

#include <unistd.h>

namespace pr {
    ServerSocket::ServerSocket(int port){
        socketfd = socket(AF_INET, SOCK_STREAM, 0);
        if(socketfd < 0){
            perror("socket");
            return;
        }

        struct sockaddr_in sin;
        memset((char*)&sin, 0, sizeof(sin));

        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);
        sin.sin_addr.s_addr = INADDR_ANY;

        if(bind(socketfd, (struct sockaddr*)&sin, sizeof(sin)) < 0){
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
        int scom = ::accept(socketfd, (struct sockaddr*)&sin, &len);
        if(scom < 0){
            perror("accept");
            return Socket();
        }
        return Socket(scom);
    }

	void ServerSocket::close(){
        if(socketfd != -1){
            ::close(socketfd);
            socketfd = -1;
        }
    }
};