#ugly makefile :(
SERVER_UDP :
	g++ -o server_udp main_server_udp.cpp ServerUDP.cpp Socket.cpp -std=c++11 -pthread
CLIENT_UDP :
	g++ -o client_udp ClientUDP.cpp Socket.cpp -std=c++11 -pthread

all : SERVER_UDP CLIENT_UDP