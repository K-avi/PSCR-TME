#include <thread>
#include <string>
#include "Socket.hpp"

namespace pr {
    class ChatroomWorker {
        std::thread readThread;
        std::thread writeThread;

        Socket socket_read;
        Socket socket_write;
        std::string username;

        public: 
            ChatroomWorker(int port, std::string ip, std::string username);
            void start();
            void stop();
            void read();
            void write();
    };
}