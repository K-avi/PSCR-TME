#include "ChatroomWorker.hpp"
#include <string>
#include <iostream>

#define DEFAULT_PORT 7200
#define DEFAULT_MULTICAST_IP "224.0.0.10"
#define DEFAULT_USERNAME "user"

using namespace std;

int main(int argc, char**argv){

    string username, ip;
    int port;
    if(argc < 3){
        username = DEFAULT_USERNAME;
        ip = DEFAULT_MULTICAST_IP;
        port = DEFAULT_PORT;
    }else{
        ip = argv[1];
        port = atoi(argv[2]);
        username = argv[3];
    }

    pr::ChatroomWorker worker(port, ip, username);
    worker.start();

    string message;
    cin >> message;

    worker.stop();
    return 0;
}