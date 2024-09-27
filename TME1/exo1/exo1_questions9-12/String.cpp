#include "String.hpp"

namespace pr {

    // TME : question 9 

    size_t length(const char* str) {
        const char* p = str;
        while(*p) p++;

        return p - str;
    }

    char* newcopy(char* s){
        char* p = new char[strlen(s) + 1];
        memcpy(p, s, length(s) + 1);
        return p;
    }

    // TME : q10
    String::String(char* s, size_t size){
        str = new char[strnlen(s,size) + 1];
        strncpy(str, s, size);
    }

    String::String(char* s){
        str = new char[strlen(s) + 1];
        strcpy(str, s);
    }

    //TME : q11
    int String::compare(const String & s) const{
        const char* tmp = str;
        const char* tmp1 = s.str;
        while(*tmp && *tmp1 && *tmp == *tmp1){
            
            tmp++;
            tmp1++;
        }
        if(!tmp && !tmp1){ // str == s.str
            return 0;
        }else if (tmp) { // str > s.str
            return tmp - str;
        }else{ // str < s.str
            return -1;
        }
    }

    //TME : q9
    std::ostream & operator << (std::ostream & os, const String & str){
        os << str.str;
        return os;
    }


    //TME : q12 
    bool operator==(const String &a,const String &b){
        return a.compare(b) == 0;
    }

    bool operator<(const String &a,const String &b){
        return a.compare(b) < 0;
    }
}