#include "ServerUDP.hpp"
#include <string>
#include <iostream>

using namespace std;
using namespace pr;

#define DEFAULT_PORT 12345


int main(int argc, char** argv){

    int port ;

    if(argc < 2){
        port = DEFAULT_PORT;
    }else{
        port = stoi(argv[1]);
    }

    ServerUDP server(port);

    cout << "Starting server" << endl;
    server.StartServer();
    
    string s;
    cin >> s;
    
    cout << "Stopping server" << endl;
    server.StopServer();

    return 0;
}