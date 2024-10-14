#include "Question9.hpp"
#include <iostream>

//test file for q9
using namespace question9;

int main(void){

    Vector<int> vec1(10);

    for(int i = 0 ; i < 10 ; i ++){
        vec1.push_back(i);
    }

    /*Vector<int> vec2(10);
    for(int i = 10; i < 20 ; i++){
        vec2.push_back(i);
    }*/
    //vec1.insert(vec1.end() - 2,  vec2.begin(), vec2.end());
    /*for(int i = 0 ; i < vec1.get_size(); i++){
        std::cout << vec1[i] << " " ;
    }*/

    List<int> l1;
    for(int i = 10; i < 20 ; i++){
       // std::cout << l1[i] << " " ;
        l1.push_front(i);
    }

    vec1.insert(vec1.end() - 2,  l1.begin(), l1.end());
    for(int i = 0 ; i < vec1.get_size(); i++){
        std::cout << vec1[i] << " " ;
    }

  
    return 0 ;
}