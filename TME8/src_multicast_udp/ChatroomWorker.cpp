#include "ChatroomWorker.hpp"
#include "Socket.hpp"
#include <iostream>

using namespace std;
using namespace pr;

using namespace std;

namespace pr {
    ChatroomWorker::ChatroomWorker(int port, string ip, string username) : username(username) {
        socket_read.open_multicast_receiver(ip, port);
        socket_write.open_multicast_sender(ip, port);

    }

    void ChatroomWorker::start() {
        readThread = thread(&ChatroomWorker::read, this);
        writeThread = thread(&ChatroomWorker::write, this);
    }

    void ChatroomWorker::stop() {
        cout << "goodbye" << endl;
        readThread.join();
        writeThread.join();
    }

    void ChatroomWorker::read() {
        while (true) {
            cout << "received " << endl ;
            string message = socket_read.recv();
            cout << message << endl;
        }
    }

    void ChatroomWorker::write() {
        while (true) {
            cout << "writing " << endl ; 
            string message;
            getline(cin, message);
            socket_write.send(username + ": " + message);
        }
    }
}