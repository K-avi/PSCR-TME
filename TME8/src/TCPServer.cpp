#include "TCPServer.hpp"
#include "ConnectionHandler.hpp"
#include "ServerSocket.hpp"
#include <sys/select.h>
#include <unistd.h>
#include <iostream>

using namespace std;

namespace pr {
    bool TCPServer::startServer(int port){
        if(!waitingThread){
            ss = new ServerSocket(port);
            if(ss->isOpen()){
                int pipefd[2];
                if(pipe(pipefd) < 0){
                    perror("pipe");
                    return false;
                }
                
                waitingThread = new std::thread([](TCPServer* s, int readpipe){
                    while(true){

                        //rfds is the set that will be used to wait for events
                        //(socket ready to read, termination of the server on pipe)
                        fd_set rfds; //set of file descriptors
                        FD_ZERO(&rfds); //clear the set
                        FD_SET(readpipe, &rfds); //add the pipe to the set
                        FD_SET(s->ss->getFD(), &rfds); //add the server socket to the set

                        //select allows to wait on BOTH the socket and the pipe to terminate the server
                        select(std::max(readpipe, s->ss->getFD()) + 1, &rfds, NULL, NULL, NULL); //wait for an event on one of the file descriptors

                        //check for termination first 
                        if(FD_ISSET(readpipe, &rfds)){//FD_ISSET -> checks that the file descriptor is in the set
                            cout << "Server terminated" << endl;
                            return ;
                        }

                        //if we reach this point, it means there was a connection on the server socket
                        Socket scom = s->ss->accept();
                        if(scom.isOpen()){
                           //delegate the connection to the handler
                           ConnectionHandler* copy = s->handler->clone();
                            if(copy){
                                 copy->handleConnection(scom);
                            } 
                            delete copy;
                        }
                    }
                },this,pipefd[0]);
                killpipe = pipefd[1];
                return true;
            }

        }
        return false;
    }

    void TCPServer::stopServer(){
        if(waitingThread){
            //write in the pipe to terminate the server
            int n = 1 ;
            write(killpipe, &n, sizeof(n));
            //wait for the thread to terminate
            waitingThread->join();
            delete waitingThread;
            waitingThread = nullptr;
            //close the socket
            delete ss;
            ss = nullptr;
            //close the pipe
            close(killpipe);
            killpipe = -1;
        }
    }
};