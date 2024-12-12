#pragma once 
#include <atomic>
#include <netinet/in.h>
#include <pthread.h>
#include <string>
#include <vector>
#ifndef SRC_SERVERUDP_H_
#define SRC_SERVERUDP_H_

#include <netinet/ip.h>
#include "Socket.hpp"
#include <unordered_map>
#include <vector>
#include <thread>

namespace pr{

/*
a better implementation would be to count the number of get requests and 
then build a string that contains all the values associated with the keys
and then send it to the client

what I will do is count (client side) the number of get requests and then 
recv that number of times the values associated with the keys which is 
strictly worse than the previous implementation
*/

class ServerUDP{   
    Socket serversock ;
    std::thread* server_thread;
    std::unordered_map<std::string, std::string> kv_map;
    std::atomic_bool running;

    /*
    creates a vector of commands from a string ; each command is separated by a ';' 
    doesn't check the validity of the commands
    doesn't support that the command contains a ';'
    */
    std::vector<std::string> commands_from_string(std::string s); 
    
    /*
    checks for the validity of a command 
    and sends the value associated with a key if the command is a get command
    or sets a key to a value if the command is a set command and there 
    is the correct number of arguments

    returns true on quit command 
    false otherwise
    */
    void process_command(sockaddr_in sin, std::string command);
    

    public : 

    ServerUDP(int port):serversock(Socket()), kv_map(std::unordered_map<std::string, std::string>()){
        serversock.bind(in_addr{INADDR_ANY}, port); 
    }

    inline void set(std::string key, std::string value){
        kv_map[key] = value;
    }

    inline std::string get(std::string key){
        return kv_map[key];
    }

    void StartServer();
    void StopServer();
};

};


#endif 