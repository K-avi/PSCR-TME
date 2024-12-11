#ifndef JOBHANDLER_HPP
#define JOBHANDLER_HPP

#include "Job.hpp"
#include "ConnectionHandler.hpp"
#include "Socket.hpp"

class JobHandler : public Job {
    pr::ConnectionHandler* handler;
    pr::Socket s;

public:
    JobHandler(pr::ConnectionHandler* h, pr::Socket s) : s(s) {
        handler = h->clone();
    }  
    void run() override {
        handler->handleConnection(s);
        delete handler;
    }
};
#endif