#include "TCPServer.hpp"
#include "FTPHandler.hpp"
#include "Socket.hpp"
#include <cstdlib>
#include <cstring>

using namespace std;

#define DEFAULT_SERVER_IP "localhost"
#define DEFAULT_SERVER_PORT 2000

int main(int argc, char** argv){
    
        int socket_num ; 
        string server_ip ;
        if(argc < 3){
            socket_num = 2000 ; 
            server_ip = DEFAULT_SERVER_IP ;
        }else{
            socket_num = atoi(argv[1]);
            server_ip = argv[2];
        }
        pr::Socket s;
        s.connect(server_ip, socket_num);

        cout << "Connected to server" << endl;

        pr::FTPHandlerClient handler;
        handler.handleConnection(s);    
        
        return 0 ;
}