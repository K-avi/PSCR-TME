#include "Banque.h"
#include <cstddef>
#include <cstdlib>
#include <string>
#include <iostream>
#include <thread>

using namespace std;
using namespace pr;

#define NB_THREAD_DEFAULT 10
unsigned static NB_THREAD = NB_THREAD_DEFAULT;


//question 1 :
void transaction(Banque & b) {

	for(unsigned i = 0 ; i < 1000 ; i++) {
		unsigned deb = rand() % b.size();
		unsigned cred = rand() % b.size();

		unsigned montant = rand() % 100 + 1;

		b.transfert(deb, cred, montant );

		//cout << "transfert de " << montant << " de " << deb << " vers " << cred << endl;
		this_thread::sleep_for(chrono::milliseconds(rand()%20));
	}
}

int main (int argc, char** argv) {
	vector<thread> threads;
	//Passage du nombre de threads souhaité entre 1 et 16 dans argv[1]
	if(argc >= 2) {
		char* end; 
		NB_THREAD = strtol( argv[1],&end, 10);
		if(!NB_THREAD || *end != '\0') {
			cerr << "Usage: " << argv[0] << " [NB_THREAD]" << endl;
			return 1;
		}else if(NB_THREAD > 16){
			cerr << "Usage: " << argv[0] << " [NB_THREAD] (max 16)" << endl;
			return 1;
		}
	}




	Banque b(64, 1000);
	//question 9 : 
	thread thread_compte = thread(&Banque::comptabiliser, &b, 64000);


	for(size_t i = 0; i < NB_THREAD; i++) {
		threads.push_back(thread(transaction, ref(b)));
	}

	cout << "Attente de la fin des threads" << endl;

	for (auto & t : threads) {
		t.join();
	}
	//question 9 : 
	thread_compte.join();

	// TODO : tester solde = NB_THREAD * JP
	return 0;
}
