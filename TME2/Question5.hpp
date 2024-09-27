#pragma once 

#include <cstddef>
#include <vector>
#include <forward_list>

namespace question5 {

template <typename  K, typename V>
class Entry { 
    public :
    K key;
    V value;

    public:
    Entry(K key, V value) : key(key), value(value) {}

    K getKey() const {
        return key;
    }

    V getValue() {
        return value;
    }
};

template <typename  K, typename V>
class HashTable {


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

    size_t getSize() const {
        return size;
    }
};

} // namespace question5