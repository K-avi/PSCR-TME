#pragma once 

#include <cstddef>
#include <vector>
#include <forward_list>

namespace question5 {

template <typename  K, typename V>
class Entry { 
 
    K key;
    V value;

    public:
    Entry(K key, V value) : key(key), value(value) {}

    K getKey() const {return key;}
    V getValue() const {return value;}

    K& operator*(){return key;} 

    bool operator==(const Entry<K,V>& other) const{
        return key == other.key && value == other.value;
    }
    
    template <typename  L, typename M>
    friend class HashTable;
};

template <typename  K, typename V>
class HashTable {

    public:
    std::vector<std::forward_list<Entry<K,V>>> kvp_buckets; 
    size_t size;

    public:
    HashTable(size_t size) : size(size){
        kvp_buckets = std::vector<std::forward_list<Entry<K,V>>>(size);

        for(auto & bucket : kvp_buckets){
            bucket = std::forward_list<Entry<K,V>>();
        }
    }

    V* get(const K& key){
        size_t hash = std::hash<K>{}(key) % size;

        std::forward_list<Entry<K,V>> & bucket = kvp_buckets[hash];

        for(auto & [k, v] : bucket){
            if(k == key){
                return &v;
            }
        }
        return nullptr;
    }
    
    bool put(const K& key, V value){
        size_t hash = std::hash<K>{}(key) % size;
        std::forward_list<Entry<K,V>> & bucket = kvp_buckets[hash];

        for(auto & [k, v] : bucket){
            if(k == key){
                return true;
            }
        }
        bucket.push_front(Entry(key, value));
        return false;
    }

    size_t getSize() const {return size;}

    class iterator{
        
        //iterateur positionne sur la fin du vecteur de buckets
        typename std::vector<std::forward_list<Entry<K,V>>>::iterator buckets_end ;
        //iterateur positionne sur le bucket courant
        typename std::vector<std::forward_list<Entry<K,V>>>::iterator buckets_it ;
        //iterateur positionne sur l'element courant de la liste chainee du bucket courant
        typename std::forward_list<Entry<K,V>>::iterator list_it;

        public: 

        iterator(std::vector<std::forward_list<Entry<K,V>>> & buckets, size_t index = 0) : buckets_end(buckets.end()){
            buckets_it = buckets.begin() + index;

            if(buckets_it != buckets_end){
                list_it = (*buckets_it).begin();
            }else{
               list_it = (buckets_it - 1)->end();
            }   
        }

        iterator& operator++(){
            if(buckets_it == buckets_end){
                return *this;
            }
            if(list_it != (*buckets_it).end()){
                ++list_it;
            }else if(buckets_it != buckets_end){
                ++buckets_it;
                if(buckets_it != buckets_end){
                    list_it = (*buckets_it).begin();
                }
            }
            return *this;
        }

        bool operator!=(const iterator& other) const{

            return !( buckets_it == other.buckets_it && list_it == other.list_it);
        }

        Entry<K,V>& operator*(){
            return *list_it;
        }     
    };

    iterator& begin() {
        return *(new iterator(kvp_buckets));
    }
    iterator& end() {
        return *(new iterator(kvp_buckets, kvp_buckets.size()));
    }

};

} // namespace question5