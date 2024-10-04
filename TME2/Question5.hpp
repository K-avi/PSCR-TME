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

        typename std::vector<std::forward_list<Entry<K,V>>>::iterator buckets_end ;

        typename std::vector<std::forward_list<Entry<K,V>>>::iterator buckets_it;
        typename std::forward_list<Entry<K,V>>::iterator list_it;

        public: 

        iterator(std::vector<std::forward_list<Entry<K,V>>>& buckets_ref){
            buckets_end = buckets_ref.end();
            buckets_it = buckets_ref.begin();
            list_it = (*buckets_it).begin();
        }
        
        iterator(std::vector<std::forward_list<Entry<K,V>>>& buckets_ref,
                 typename std::vector<std::forward_list<Entry<K,V>>>::iterator buckets_it  ){
            buckets_end = buckets_ref.end();
            this->buckets_it = buckets_it;
            list_it = (*(this->buckets_it)).begin();
        }

    
        iterator& operator++(){
            //increment the iterator in list
            //if end; go to next bucket
            if(++buckets_it == buckets_end){return *this;}
            if(++list_it == (*buckets_it).end() ){
                list_it = (*buckets_it).begin();
            }
            return *this ;
        }
        
        Entry<K, V>& operator*(){
            return *list_it;
        }
        
        bool operator!=(const iterator& other){
            return !(buckets_it == other.buckets_it 
                   && list_it == other.list_it) ;
        }
    };

    iterator& begin() {
        return *(new iterator(kvp_buckets));
    }
    iterator& end() {
        return *(new iterator(kvp_buckets, kvp_buckets.end()));
    }

};

} // namespace question5