#pragma once
#include <cstddef>

namespace tme3{
    
//Question 1 : 
template <typename iterator>
size_t count(iterator begin, iterator end){
    size_t num = 0 ; 
    for(; begin != end; ++begin){
        ++num; 
    }
    return num;
}

//Question 2 : 
template <typename iterator, typename T> 
size_t count_if_equal(iterator begin, iterator end, const T& val){
    size_t num = 0 ; 
    for(; begin != end; ++begin){
        if(val == *begin)
            ++num; 
    }
    return num;
}

}