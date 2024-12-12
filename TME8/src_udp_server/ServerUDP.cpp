#include "ServerUDP.hpp"

#include <iostream>
#include <pthread.h>
#include <vector>

using namespace std;


namespace pr {

    vector<string> ServerUDP::commands_from_string(string s){
        vector<string> commands;
        string cur_command = "";
        for(auto c : s){
            if(c == ';'){
                commands.push_back(cur_command);
                cur_command = "";
            }else{
                cur_command += c;
            }
        }
        commands.push_back(cur_command);
        return commands;
    }

    void ServerUDP::process_command(sockaddr_in sin, string s){
        if(s[0] == 's' || 'S'){
            //goto the first non space character
            int i = 1 ; 
            while(s[i] == ' ') i++;

            string key = "";
            while( (s[i] != ' ' || s[i] != '\0') && i < s.size()){
                key += s[i];
                i++;
            }
            if(s[i] == '\0'){
                cout << "Invalid command" << endl;
                return;
            }

            //goto the first non space character
            while(s[i] == ' ') i++;

            string value = "";
            while( (s[i] != ' ' || s[i] != '\0') && i < s.size()){//we could allow ' ' characters in the value tbh 
                value += s[i];
                i++;
            }

            kv_map[key] = value;    
        }else if(s[0] == 'g' || 'G'){

            //goto the first non space character
            int i = 1 ; 
            while(s[i] == ' ') i++;

            string key = "";
            while( (s[i] != ' ' || s[i] != '\0') && i < s.size()){
                key += s[i];
                i++;
            }

            if(kv_map.find(key) != kv_map.end()){
                string value = kv_map[key];
                sendto(serversock.getFD(), value.c_str(), value.size(), 0, (struct sockaddr*)&sin, sizeof(sin));
            }else{//create empty key and send it 
                string value = "";
                sendto(serversock.getFD(), value.c_str(), value.size(), 0, (struct sockaddr*)&sin, sizeof(sin));
            }
            
        }else{
            cout << "Invalid command" << endl;
        }
    }

    void ServerUDP::StartServer(){
        running = true;
        server_thread = new std::thread([](ServerUDP* s){
        while(s->running){

            //declare the socket address 
            //to handle termination I should 
            //put the select thingy from the TCP version
            struct sockaddr_in sin;
            socklen_t sinlen = sizeof(sin);
            char buffer[1024];
            int n;
            if((n = recvfrom(s->serversock.getFD(), buffer, 1024, 0, (struct sockaddr*)&sin, &sinlen)) < 0){
                perror("recvfrom");
                return;
            }
            buffer[n] = '\0';
            cout << "Received: " << buffer << endl;

            string commands = string(buffer);
            vector<string> commands_vector = s->commands_from_string(commands);

            for(auto command : commands_vector){
               s->process_command(sin,command);
            }
        }
        }, this);
    }

    void ServerUDP::StopServer(){
        running = false;
        server_thread->join();
        delete server_thread;
        serversock.close();
    }
}