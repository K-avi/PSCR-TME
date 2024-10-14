#include "Banque.h"

#include <iostream>
#include <mutex>

using namespace std;

namespace pr {

void Banque::transfert(size_t deb, size_t cred, unsigned int val) {
	
	//Question 9 
	//unique_lock<recursive_mutex> g(m);

	
	Compte & debiteur = comptes[deb];
	Compte & crediteur = comptes[cred];

	recursive_mutex & mdeb = debiteur.getMutex();
	recursive_mutex & mcred = crediteur.getMutex();

	//Question 6
	while(!mdeb.try_lock() || !mcred.try_lock()) {
		mdeb.unlock();
		mcred.unlock();
		this_thread::yield();
	}

	if (debiteur.debiter(val)) {
		crediteur.crediter(val);
	}

	mcred.unlock();
	mdeb.unlock();
}

size_t Banque::size() const {
	return comptes.size();
}

bool Banque::comptabiliser (int attendu) {
	int bilan = 0;
	int id = 0;
	
	//Question 9 
	//unique_lock<recursive_mutex> g(m);

	for (auto & compte : comptes) {

		//Question 10 :
		recursive_mutex & mcompte = compte.getMutex();
		while(!mcompte.try_lock()) {
			mcompte.unlock();
			this_thread::yield();
		}

		if (compte.getSolde() < 0) {
			cout << "Compte " << id << " en négatif : " << compte.getSolde() << endl;
		}
		bilan += compte.getSolde();
		id++;
		mcompte.unlock();
	}
	if (bilan != attendu) {
		cout << "Bilan comptable faux : attendu " << attendu << " obtenu : " << bilan << endl;
	}
	return bilan == attendu;
}

}
