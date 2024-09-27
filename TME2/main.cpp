#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include <utility>

#include "Question5.hpp"

using namespace std;


//////////question 2, fonction utilitaire

/*
static void append_new_word_vecstr(vector<string> &vec_words, string word) {

	vector<string>::iterator it;
	for(it = vec_words.begin(); it != vec_words.end(); it++){
		if(*it == word){
			return;
		}
	}
	vec_words.push_back(word);
}
*/

//////////question 3, fonctions utilitaires :
/*
static void append_new_word_pair(vector<pair<string,int>>& vec, string str) {
	for(auto & [word, count] : vec){
        if(word == str){
			count++;
			return;
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
*/


int main () {
	using namespace std;
	using namespace std::chrono;

	ifstream input = ifstream("/tmp/WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	//question 2 :
	//vector<string> vec_words = std::vector<string>(); 

	//question 3 : 
	//vector<pair<string,int>> vec_words = std::vector<pair<string,int>>();

	//question 6 : 
	question5::HashTable<string, int> vec_words(1000);

	size_t nombre_lu = 0;
	// prochain mot lu
	string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);

		// word est maintenant "tout propre"
		if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": "<< word << endl;
		nombre_lu++;
		//question 2 : 
		//append_new_word(vec_words, word);

		//question 3 : 
		//append_new_word_pair(vec_words, word);

		//question 5 :
	
		if(vec_words.put(word, 1))
		{
			int* count = vec_words.get(word);
			(*count)++;
			vec_words.put(word, *count);
		}
	}
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;
	
	//question 3 :
	/*
	cout << "Found a total of " << vec_words.size() << " different words." << endl;
	cout << "Found a total of " << getCount(vec_words, "war") << " occurences of the word 'war'." << endl;
	cout << "Found a total of " << getCount(vec_words, "peace") << " occurences of the word 'peace'." << endl;
	cout << "Found a total of " << getCount(vec_words, "toto") << " occurences of the word 'toto'." << endl;
	*/

	//question 5 :

	int* count = vec_words.get("war");

	if(count)
		cout << "Found a total of " << *count << " occurences of the word 'war'." << endl;
	count = vec_words.get("peace");
	if(count)
		cout << "Found a total of " << *count << " occurences of the word 'peace'." << endl;
	
	count = vec_words.get("toto");
	if(count)
		cout << "Found a total of " << *count << " occurences of the word 'toto'." << endl;
	else
		cout << "Word 'toto' not found." << endl;
    return 0;
}


