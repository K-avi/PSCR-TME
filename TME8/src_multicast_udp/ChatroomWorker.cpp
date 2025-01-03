#include "ChatroomWorker.hpp"
#include "Socket.hpp"
#include <iostream>

using namespace std;
using namespace pr;

using namespace std;

namespace pr {
    ChatroomWorker::ChatroomWorker(int port, string ip, string username) : username(username) {
        socket.open_multicast_receiver(ip, port);
        socket2.open_multicast_sender(ip, port);
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
            string message = socket.recv();
            cout << "received: " << message << endl;
        }
    }

    void ChatroomWorker::write() {
        while (true) {
            string message;
            getline(cin, message);
            cout << "writing :" <<  message << endl ; 
            socket2.send(username + ":" + message);
        }
    }
}