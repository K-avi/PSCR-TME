# TME2 

## Q1) 
d'après la dernière ligne du programme, il y a 566193 mots

## Q2) 

on implèmente une fonction 
```cpp
static void append_new_word(std::vector<std::string> &vec_words, std::string word) {

	vector<string>::iterator it;
	for(it = vec_words.begin(); it != vec_words.end(); it++){
		if(*it == word){
			return;
		}
	}
	vec_words.push_back(word);
}
 
```
On initialise un vecteur au début du main en appelant dans la fonction précedante
dans la boucle du main qui compte les mots, et en affichant la taille du vecteur à la fin du main, 
on obtient 20333 mots différents.

## Q3)

On implémente les fonctions utilitaires : 

```cpp 
static void append_new_word_pair(vector<pair<string,int>>& vec, string str) {
	for(auto & [word, count] : vec){
        if(word == str){
			count++;
		}
    }
	vec.push_back(make_pair(str, 1));
}

static size_t getCount(vector<pair<string,int>>& vec, string str){
    for(auto & [word, count] : vec){
        if(word == str){
			return count;
		}
    }
    return 0;
}
``` 

On appelle append\_new\_word\_pair dans la boucle principale.
On voit alors qu'il y a 114 occurences de peace, 298 de 
war et 0 de toto

## Q4)

Ce programme a une complexité O(n\*m) où n est le nombre 
de mots dans Guerre et paix. En effet, parcourir le livre 
mot par mot est en O(n) et l'insertion des mots dans 
le vecteur est en O(m) où m est le nombre de mots 
distincts dans guerre et paix. On suppose que m~=log(n). 
On a donc une complexité en O(n\*log(n)). 
 
