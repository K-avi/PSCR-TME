#include "Socket.hpp"
#include <arpa/inet.h>
#include <iostream>

using namespace std;
using namespace pr;

#define DEFAULT_PORT 12345
#define DEFAULT_HOST "localhost"


int main(int argc, char** argv){
    string host ;
    int port ;
    if(argc < 3){
        host = DEFAULT_HOST;
        port = DEFAULT_PORT;
    }else{
        host = argv[1];
        port = stoi(argv[2]);
    }

    Socket sock;
    sock.open(host, port);

    string s;
    while(true){
        cout << "Enter a string to send to the server" << endl;
        getline(cin, s);//discovered that this exists; so long fgets

        //count the get requests (this implem is wacky and I'm not proud of it)
        int get_requests = 0;
        short state = 3 ; // 0 = undetermined , 1 = set , 2 = get , 3 = init , 4 = quit
        for(auto c : s){
            if(!state || state == 3){
                if(c == 'g' || c == 'G'){
                    get_requests++;
                    state = 1 ; 
                }else if( c == 's' || c == 'S') {
                    state = 2 ; 
                }else if(c == 'q' || c == 'Q'){
                    state = 4 ;
                }
            }
            if( c == ';'){
                state = 0;
            }
        }
        if(state == 4) break;
        //send the string to the server
        sock.send(s);
        

        cout << "Receiving the response from the server" << endl;
        //receive the response from the server
        auto responses = sock.recv(get_requests);
        for(auto response : responses){
            cout << response << endl;
        }
    }
    cout << "goodbye ! " << endl ;
    return 0;
}
