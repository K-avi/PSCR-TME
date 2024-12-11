#ifndef SRC_TCPSERVER_H_
#define SRC_TCPSERVER_H_

#include <thread>
#include <vector>
#include "ServerSocket.hpp"
#include "ConnectionHandler.hpp"
#include "Socket.hpp"

namespace pr {

// un serveur TCP, la gestion des connections est déléguée
class TCPServer {
	ServerSocket * ss; // la socket d'attente si elle est instanciee
	ConnectionHandler * handler; // le gestionnaire de session passe a la constru

	std::thread * waitingThread; // le thread d'attente

	std::vector<std::thread *> handlerThreads; // les threads de gestion de client

	int killpipe; //pipe used to kill the server's accept thread
public :
	TCPServer(ConnectionHandler * handler): ss(nullptr),handler(handler),waitingThread(nullptr) {}
	// Tente de creer une socket d'attente sur le port donné
	bool startServer (int port);

	void handleClient(Socket s);

	// stoppe le serveur
	void stopServer ();
};

} // ns pr

#endif /* SRC_TCPSERVER_H_ */
