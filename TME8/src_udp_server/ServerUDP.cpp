#include "ServerUDP.hpp"

#include <iostream>
#include <pthread.h>
#include <vector>
#include <unistd.h>
#include <sys/select.h> 


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

    void ServerUDP::process_command(sockaddr_in sin, string str){
        //remove spaces from s 
        int i = 0 ; 
        while(str[i]==' ') i++; 
        
        string s = str.substr(i);
        

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
            cout << "Invalid command " << s <<  endl;
        }
    }

    void ServerUDP::StartServer(){
        running = true;
        int pipefd[2];
        if(pipe(pipefd) < 0){
            perror("pipe");
            return ;
        }

        server_thread = new std::thread([](ServerUDP* s, int readpipe){
        while(s->running){

            //declare the socket address 
            //to handle termination I should 
            //put the select thingy from the TCP version

        //rfds is the set that will be used to wait for events
                        //(socket ready to read, termination of the server on pipe)
            fd_set rfds; //set of file descriptors
            FD_ZERO(&rfds); //clear the set
            FD_SET(readpipe, &rfds); //add the pipe to the set
            FD_SET(s->serversock.getFD(), &rfds); //add the server socket to the set

            //select allows to wait on BOTH the socket and the pipe to terminate the server
            select(std::max(readpipe, s->serversock.getFD()) + 1, &rfds, NULL, NULL, NULL); //wait for an event on one of the file descriptors
            cout << "woke up from select" << endl;
            //check for termination first 
            if(FD_ISSET(readpipe, &rfds)){//FD_ISSET -> checks that the file descriptor is in the set
                cout << "Server terminated" << endl;
                return ;
            }


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

               cout << "processing : " << command << endl ; 
               s->process_command(sin,command);
            }
        }
        }, this,pipefd[0]);
        killpipe = pipefd[1];//set killpipe to write end of the pipe
    }

    void ServerUDP::StopServer(){
        int n = 1 ;
        write(killpipe, &n, sizeof(n));
        //wait for the thread to terminate

        running = false;
        server_thread->join();
        delete server_thread;
        serversock.close();
    }
}