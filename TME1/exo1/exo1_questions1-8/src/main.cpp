#include "List.h"
#include <string>
#include <iostream>
#include <cstring>

int main () {

	std::string abc = "abc";

	//FAUTE il faut allouer 4 caractères pour terminer la string
	char * str = new char [4]; 
	str[0] = 'a';
	str[1] = 'b';
	str[2] = 'c';
	//FAUTE : ajout du caractère de fin de string
	str[3] = '\0'; 
	//suppression de size_t i car non utilisé

	if (! strcmp (str, abc.c_str())) {
		std::cout << "Equal !";
	}

	pr::List list;
	std::string str2 = "def";
	
	list.push_front(abc);
	list.push_front(abc);
	//test avec push_back et une autre chaine
	list.push_back(str2);

	std::cout << "Liste : " << list << std::endl;
	std::cout << "Taille : " << list.size() << std::endl;

	// Affiche à l'envers
	//FAUTE : underflow de size_t car i est toujours >= 0
	//declaration de i en int dans le corps du fot car non utilisé en dehors
	for (int i= list.size()-1 ; i >= 0 ; i--) {
		std::cout << "elt " << i << ": " << list[i] << std::endl;
	}
	//FAUTE
	//pas besoin de désalouer chaque caractère

	delete[] str;
}
