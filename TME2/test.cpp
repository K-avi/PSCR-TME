#include "Question5.hpp"
#include <string>
#include <iostream>

using namespace std;
int main(){
    	question5::HashTable<string, int> table_words(100);

        for(int i = 0 ; i < 100 ; i++){
            table_words.put(to_string(i), i);
        }

        vector <question5::Entry<string,int>> vec_occurrence ;

        for(auto it = table_words.begin();  it != table_words.end() ; ++it ){
		    vec_occurrence.push_back(*it);
           //cout << tmp++ << endl;
	    //}
        }

        return 0 ;
}