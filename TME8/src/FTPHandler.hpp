#include "ConnectionHandler.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>

namespace pr {
    class FTPHandlerServer : public ConnectionHandler {
        std::string dir_name;
    public:
        FTPHandlerServer(std::string dir_name) : dir_name(dir_name) {}

        void handleConnection(Socket s) override ;
        ConnectionHandler * clone() const override {
            return new FTPHandlerServer(dir_name);
        }
    };

    class FTPHandlerClient : ConnectionHandler {

    public:
        void handleConnection(Socket s) override ;
        ConnectionHandler * clone() const override {
            return new FTPHandlerClient();
        }
    };
};