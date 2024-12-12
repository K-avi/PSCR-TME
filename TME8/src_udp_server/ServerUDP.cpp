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
        cout << "string given to process is " << s << " first char is " << s[0] << endl ; 
        //remove '\n'
        for(auto &c : s) if(c == '\n') c = '\0';

        if(s[0] == 's' || s[0] == 'S'){
            //goto the first non space character
            int i = 1 ; 
            while(s[i] == ' ') i++;
            cout << "current char is " << s[i] << endl ;
            string key = "";
            while( (s[i] != ' ' && s[i] != '\0') && (unsigned)i < s.size()){
                key += s[i];
                i++;
            }
            cout << "key parsed is " << key << endl ;
            if(s[i] == '\0'){
                cout << "Invalid command set requires 2 arguments" << endl;
                return;
            }

            //goto the first non space character
            while(s[i] == ' ') i++;

            string value = "";
            while( (s[i] != ' ' && s[i] != '\0' && s[i] != '\n') && i < s.size()){//we could allow ' ' characters in the value tbh 
                value += s[i];
                i++;
            }

            kv_map[key] = value; 
            cout << "first command is: " << s[0] << endl;   
        }else if(s[0] == 'g' ||  s[0] == 'G'){

            //goto the first non space character
            int i = 1 ; 
            while(s[i] == ' ') i++;

            string key = "";
            while( (s[i] != ' ' && s[i] != '\0' && s[i] != '\n') && i < s.size()){
                key += s[i];
                i++;
            }

            if( (kv_map.find(key) != kv_map.end()) ){
                cout << "key found; value is " << kv_map[key] << endl;

                string value = kv_map[key];
                sendto(serversock.getFD(), value.c_str(), value.size(), 0, (struct sockaddr*)&sin, sizeof(sin));

            }else{//create empty key and send it 
                cout << "key not found " << endl;
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
            cout << "commands : " << buffer;
            vector<string> commands_vector = s->commands_from_string(commands);
            cout << "number of commands is " << commands_vector.size() << endl ;
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