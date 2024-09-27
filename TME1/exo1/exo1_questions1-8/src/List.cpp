
#include "List.h"
//FAUTE : List.h n'était pas inclus

namespace pr {

// ******************* Chainon
Chainon::Chainon (const std::string & data, Chainon * next):data(data),next(next) {};

size_t Chainon::length() {
	
	Chainon * it = this;

	size_t len = 1;
	while (it->next) {
		it = it->next;
		len++;
	}
	return len;
}

void Chainon::print (std::ostream & os) const {
	os << data ;
	if (next != nullptr) {
		os << ", ";
		next->print(os);
	}else{
		os << "";
	}//FAUTE pas de cas de base à la récursivité
	
}

// ******************  List
const std::string & List::operator[] (size_t index) const  {
	Chainon * it = tete;
	for (size_t i=0; i < index ; i++) {
		it = it->next;
	}
	return it->data;
}

void List::push_back (const std::string& val) {
	if (tete == nullptr) {
		tete = new Chainon(val);
	} else {
		Chainon * fin = tete;
		while (fin->next) {
			fin = fin->next;
		}
		fin->next = new Chainon(val);
	}
}

void List::push_front (const std::string& val) {
	tete = new Chainon(val,tete);
}

//FAUTE : il n'était pas précisé que la méthode empty était une méthode de la classe List
bool List::empty() {
	return tete == nullptr;
}

size_t List::size() const {
	if (tete == nullptr) {
		return 0;
	} else {
		return tete->length();
	}
}

//FAUTE : la définition de la surchage de l'opérateur << était hors du namespace pr
std::ostream & operator<< (std::ostream & os, const List & vec) 
{
	os << "[";
	if (vec.tete != nullptr) {
		vec.tete->print (os) ;
	}
	os << "]";
	return os;
}

} // namespace pr




