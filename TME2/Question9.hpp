#pragma once

#include <cstddef>
#include <cstring>
#include <iostream>

namespace question9 {

template <typename T>
class Vector{

    std::size_t size, capa; 
    T* data;

    //called from push_back
    void realloc(){
        capa *= 2;
        T* new_data = new T[capa];
        for(std::size_t i = 0; i < size; i++){
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
    }
    
    public: 
    Vector(size_t capa) 
        : size(0),
        capa(capa),
        data(new T[capa]) 
        {}
    ~Vector(){
        delete[] data;
    }

    T& operator[](size_t index){
        return data[index];
    }

    T& operator[](size_t index) const{
        return data[index];
    }

    void push_back (const T& val){
        if(size == capa){
            realloc();
        }
        data[size++] = val;
    }

    size_t get_size() const {
        return size;
    }

    inline bool empty() const {
        return size == 0;
    }

    //ajoute pour le tme
    auto begin () {
        return data;
    }
    auto end() {
        return data + size;
    }

    //implementing iterator insert( const_iterator pos, InputIt first, InputIt last ); 
    //because it's the method I used to copy stuff
    template< class InputIt >
    void insert (T* pos, InputIt first, InputIt last){

        //first create a new array with the old values
        size_t pos_index = pos - data ;
        size_t tmp_size = end() - pos;
        // std::cerr << "tmp_size " << tmp_size << "end() " << end() - data << std::endl;
        T* tmp = new T[tmp_size];
        for(int i = 0; i < tmp_size; i++){
           // std::cerr << i << std::endl;
            tmp[i] = pos[i];
        }//I'm afraid of using memcpy with this because I'm not sure it works as intented w objects

        //then reallocate for new values
        size_t n = last - first ;

      

        while(n + size > capa){
            realloc();
        }//ugly 

        //then copy values from pos onwards
        size_t it = pos_index;
        for(; it != pos_index + n ; it++){//data+pos+n bc you start at this index and so on
            data[it] = *first++;
        }

        //copy old elements back after elements to insert
        size_t last_index = it + tmp_size;
        T* tmp_start = tmp;
        for(; it < last_index; it++ ){
            std::cerr << it << std::endl;
            data[it] = *tmp++;
        }

        delete[] tmp_start;
        size += n;
        return;
    }//memcpy and memmove would have been much simpler to use smh; I don't think this is a good way to do this.
    //I must have done something very wrong I think.
};


template <typename T> 
class Node{
    public:
    T data; 
    Node* next;

    Node(const T& val)
        :data(val),
        next(nullptr)
    {}

    Node(const Node& n){
        if(!n){
            next = nullptr; 

        }else{
            data = n.data;
            next = new Node(n.next);
        }
    }

    template<typename K>
    friend class List;
};

template <typename T>
class List{
    public:
    Node<T>* head;

    public : 

    //constructeurs / destructeurs : 
    List()
        :head(nullptr)
        {}

    List(Node<T>& n)
        :head(&n)
        {}

    ~List(){
        Node<T>* cur = head;
        while(cur){
            Node<T>* tmp = cur->next; 
            delete (cur);
            cur = tmp;
        }
    }

    //modificateurs : 

    void push_front (const T& val){
        Node<T>* newHead = new Node<T>(val);
        newHead->next = head; 
        head = newHead;
    }

    void push_back (const T& val){
        Node<T>* cur = head; 
        while(cur){
            if(!cur->next){
                cur->next = new Node<T>(val);
                return ;
            }
            cur = cur->next;
        }
        head = new Node<T>(val);//empty list case
    }

    //prédicats : 

    size_t size() const{
        Node<T>* cur = head; 
        size_t ret = 0; 
        while(cur) {
            ret++;
            cur = cur->next;
        }
        return ret ;
    }

    bool empty() const{
        return !head;
    }

    //accès : 

    T& operator[](size_t index){
        Node<T>* cur_node = head; 
        size_t cur_index = 0;
        while(cur_node && (cur_index != index)){
            index++; 
            cur_node = cur_node->next;
        }
        if(cur_node){
            return cur_node->data; 
        }else{
            std::cerr << "invalid access " << index << std::endl;
            exit(-1);
        }
    } 

    //iterateurs : 

    List<T>& begin(){
        return *new List<T>((Node<T>&)*head);
    }

    List<T>& end(){
        Node<T>* cur = head;
        while(cur->next){
            cur = cur->next;
        }
        return *new List<T>((Node<T>&)*cur);
    }

  
    List<T>& operator++(int){
        head = head->next;
        return *this;
    }

    T operator*(){
        return head->data;
    }

    bool operator==(List<T>& other){
        return head->data == other.data;
    }
    
    bool operator!=(List<T>& other){
        return head->data != other.head->data;
    }
    size_t operator-(List<T>& other){
        Node<T>* cur = head;
        size_t n = 0 ; 
        while (cur != other.head && cur){
            cur = cur->next ; 
            n++; 
        }
        if(!cur) return 0 ; 

        size_t size = n ; 
        while(cur){
            cur = cur->next; 
            size++;
        }
        return size - n;
    }
    
};

}