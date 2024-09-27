#ifndef __MyString__H 
#define __MyString__H

#include <cstddef>
#include <cstring>
#include <ostream>

namespace pr {
    class String{
        char* str;
        
        public :
            
            //TME : question 10
            String(char* s, size_t size);
            String(char* s);
            ~String(){
                delete[] str;
            }
            
            /*TME : Question 9 : 
            String(char* s)
                :str(s)
            {}
            */
            //TME : 
            int compare(const String & s) const;
            friend std::ostream & operator << (std::ostream & os, const String & str);
            friend bool operator==(const String &a,const String &b);
            friend bool operator<(const String &a,const String &b);

    };

    size_t length(const char* str);
    char* newcopy(char* s);
}
#endif