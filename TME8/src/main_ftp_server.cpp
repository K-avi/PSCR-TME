#include "TCPServer.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
#include "FTPHandler.hpp"


#define DEFAULT_DIR "/tmp"
#define DEFAULT_SOCKET 2000

using namespace std; 
using namespace pr;

int main(int argc, char** argv){

    int socket_num ; 
    string dir ; 
    if(argc < 3){
        socket_num = DEFAULT_SOCKET ; 
        dir = DEFAULT_DIR ;
    }else{
        socket_num = atoi(argv[1]);
        dir = argv[2];
    }

    FTPHandlerServer handler(dir);

    TCPServer server(&handler);
    server.startServer(socket_num);

    cout << "Server started on port " << socket_num << endl;
    std::string s ;
	std::cin >> s ;
    
    server.stopServer();
    return 0 ;
}