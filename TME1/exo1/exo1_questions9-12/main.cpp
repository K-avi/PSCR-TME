#include "String.hpp"
#include <cstddef>
#include <iostream>

int main(void){

    //Question 9
    pr::String s((char*)"Hello");
    size_t l = pr::length("Hello");

    //Question 10
    std::cout << s << " et " << l << std::endl;

    return 0;
}

/*
Question 11 : 

Si l'on ne copie pas la string passée en paramètre du constructeur,
on utilise alors de la mémoire dont la durée de vie nous est inconnue. Elle 
pourrait être modifiée ou libérée par d'autres fonctions du programme, ce qui
est une source d'erreur.

Question 13 : 

on causerait soit une fuite de mémoire, si le destructeur n'est pas modifié, 
soit un free invalide si le constructeur n'est pas modifié pour libérer la mémoire
*/