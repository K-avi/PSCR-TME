#include <forward_list>
#include <iostream>
#include <fstream>
#include <iterator>
#include <regex>
#include <chrono>
#include <unordered_map>
#include <vector>
#include <utility>

#include "Question5.hpp"
#include "tme3.hpp"

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

	//TME 2 : question 2 :
	//vector<string> vec_words = std::vector<string>(); 

	//TME 2 : question 3 : 
	//vector<pair<string,int>> vec_words = std::vector<pair<string,int>>();

	//TME 2 : question 6 : 
	//question5::HashTable<string, int> table_words(30000);

	//TME 3 : question 7 :
	unordered_map<string, int> table_words;

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
		//TME 2 : question 2 : 
		//append_new_word(vec_words, word);

		//TME 2 : question 3 : 
		//append_new_word_pair(vec_words, word);

		//TME 2 question 6 :
		/*
		if(table_words.put(word, 1))
		{
			int* count = table_words.get(word);
			(*count)++;
			table_words.put(word, *count);
		}*/
		table_words[word]++;
	}
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;
	
	//TME 2 : question 3 :
	/*
	cout << "Found a total of " << vec_words.size() << " different words." << endl;
	cout << "Found a total of " << getCount(vec_words, "war") << " occurences of the word 'war'." << endl;
	cout << "Found a total of " << getCount(vec_words, "peace") << " occurences of the word 'peace'." << endl;
	cout << "Found a total of " << getCount(vec_words, "toto") << " occurences of the word 'toto'." << endl;
	*/

	//TME 2 : question 6 :
	/*
	int* count = table_words.get("war");

	if(count)
		cout << "Found a total of " << *count << " occurences of the word 'war'." << endl;
	count = table_words.get("peace");
	if(count)
		cout << "Found a total of " << *count << " occurences of the word 'peace'." << endl;
	
	count = table_words.get("toto");
	if(count)
		cout << "Found a total of " << *count << " occurences of the word 'toto'." << endl;
	else
		cout << "Word 'toto' not found." << endl;
	*/

	//TME 2 :question 7 : 
	
	vector <question5::Entry<string,int>> vec_occurrence ;
	/*
	for(auto forward_list = table_words.begin(); 
			 forward_list != table_words.end();
	 		 forward_list++){
		vec_occurrence.insert(vec_occurrence.end(),
									forward_list->begin(),
									forward_list->end());
	}
	*/

	//TME 3 : Question 5
	
	for(auto it = table_words.begin(); it != table_words.end(); ++it ){
		question5::Entry<string, int> entry(it->first, it->second);
		vec_occurrence.push_back(entry);
	}
	
	//TME 2 question 8  / TME 3 Question 6
	sort(vec_occurrence.begin(),
		 vec_occurrence.end(),
		 [](const question5::Entry<string,int>& a, const question5::Entry<string,int>& b){ return a.getValue() > b.getValue();}
		);

	for(int i = 0 ; i < 10 ; i++){
		cout << vec_occurrence[i].getKey() << " : " << vec_occurrence[i].getValue() << endl;
	}	

	//TME 3 : Question 3 

	question5::Entry<string , int> entree_0(vec_occurrence[0].getKey(), vec_occurrence[0].getValue());

	cout << "invocation de count : " << tme3::count(vec_occurrence.begin(), vec_occurrence.end()) << endl; 
	cout << "la taille de vec_occurence est : " << vec_occurrence.size() << endl ; 
	cout << "invocation de count equal pour \"this\" : " << tme3::count_if_equal(vec_occurrence.begin(), vec_occurrence.end(),entree_0) << endl; 

	//TME 3 : Question 8 :

	unordered_map<int, forward_list<string>> map_inversee ;

	for(auto it =  table_words.begin();  it != table_words.end(); ++it){
		map_inversee[it->second].push_front(it->first);
	}

    return 0;
}


