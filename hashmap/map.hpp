#pragma once

#include <iostream> 
#include <cassert>
namespace Hashmap_space{
//--------------------------------------
//--------------------------------------
template <typename T> class Hashmap_node {
public:
    Hashmap_node* next_ = nullptr;
    Hashmap_node* prev_ = nullptr;
    T key_ = 0;
public:
    Hashmap_node(T key) : key_(key) {}

    void erase_node_after() {// удаляет next и далее
        Hashmap_node* cur = next_;
        Hashmap_node* prev = nullptr;
        while(cur) {
            prev = cur;
            cur = cur->next_;
            delete prev;
        }
    }
};
//--------------------------------------
//--------------------------------------
template <typename keyT> class Hashmap {    
private: // data
    int len_;
    Hashmap_node<keyT>** hmap_;

public: // big 5
    Hashmap(int len = 0) : len_(len), hmap_(new Hashmap_node<keyT>*[len_]{}) {}
    //....................................
    Hashmap(const Hashmap& rhs) : len_(rhs.len_) , hmap_(new Hashmap_node<keyT>*[len_]{}) { // copy ctor
        for(int i = 0; i < len_ ; i++) 
        {
            Hashmap_node<keyT>* ptr = rhs.hmap_[i];
            while(ptr)
            {
                insert_back(ptr->key_);
                ptr = ptr->next_;
            } 
        }
    }
    //....................................
    Hashmap& operator= (const Hashmap& rhs) { // copy assign
        if(this == &rhs) { return *this; }
        
        // удаляем старое состояние
        Hashmap_node<keyT>* cur = nullptr;
        for(int i = 0; i < len_; i++) 
        {
            cur = hmap_[i];
            if(cur) 
            {
                cur->erase_node_after();
                delete cur;
            }
        }
        delete [] hmap_;
        // удалили старое состояние

        len_ = rhs.len_;
        hmap_ = new Hashmap_node<keyT>*[len_]{};

        for(int i = 0; i < len_ ; i++) 
        {
            Hashmap_node<keyT>* ptr = rhs.hmap_[i];
            while(ptr)
            {
                insert_back(ptr->key_);
                ptr = ptr->next_;
            } 
        }
        
    }
    //....................................
    Hashmap(Hashmap&& rhs) : len_(rhs.len_), hmap_(rhs.hmap_) {
        rhs.map_ = nullptr;
    } //move ctor;
    //....................................
    Hashmap& operator= (Hashmap&& rhs) { // move assign
        if(this == &rhs) { return *this; }

        len_ = rhs.len_;
        std::swap(hmap_, rhs.hmap_);
        
    }
    //....................................
    ~Hashmap() { 
        Hashmap_node<keyT>* cur = nullptr;
        for(int i = 0; i < len_; i++) 
        {
            cur = hmap_[i];
            if(cur) 
            {
                cur->erase_node_after();
                delete cur;
            }
        }
        delete [] hmap_;
        return;
    }
    //..................................................//
    //..................................................//
public: // methods
    size_t hash_func (keyT key) {
        size_t hash_value = std::hash<keyT>()(key);
        hash_value = hash_value % len_;
        return hash_value;
    }
    //...................................... 
    void insert(keyT key) {  // working

        size_t hash_value = hash_func(key);
        Hashmap_node<keyT>* ptr = hmap_[hash_value];
        Hashmap_node<keyT>* newElem = new Hashmap_node<keyT>{key};
       
       // std::cout << newElem->prev_ << std::endl;

        if(!hmap_[hash_value]) {
            hmap_[hash_value] = newElem; 
            return;
        }
        // вставляем в начало
        newElem->next_ = ptr;
        newElem->prev_ = nullptr;
        hmap_[hash_value]->prev_ = newElem;
        hmap_[hash_value] = newElem;
        return;
    }
    //......................................
    void insert_back(keyT key) {

        size_t hash_value = hash_func(key);
        Hashmap_node<keyT>* ptr = hmap_[hash_value];
        Hashmap_node<keyT>* newElem = new Hashmap_node<keyT>{key};

        if(!hmap_[hash_value]) 
        {
            hmap_[hash_value] = newElem; 
            return;
        }

        while(ptr->next_) 
        {
            ptr = ptr->next_;
        }
        // в ptr последний элемент в цепочке 
        ptr->next_ = newElem;
        newElem->prev_ = ptr;
        

    }
    //......................................
    void erase(keyT key) {
        size_t hash_value = hash_func(key);
        Hashmap_node<keyT>* cur = hmap_[hash_value];
        Hashmap_node<keyT>* prev = nullptr;

        while(cur) 
        {
            if(cur->key_ == key)
                break;
            cur = cur->next_;
        }
        if(cur == nullptr) 
        { 
            std::cout << "did not find" << std::endl;
            return;
        }

        // в cur то что надо удалить
        if(!(cur->prev_))  // if cur == hmap_[hash_value]
        {
            prev = cur;
            if(!cur->next_) {
                hmap_[hash_value] = nullptr;
                delete cur;
                return;
            }
            hmap_[hash_value] = cur->next_;
            hmap_[hash_value]->prev_ = nullptr;
            delete prev;
            return;
        }

        if(!(cur->next_)) 
        {
            prev = cur;
            cur->prev_->next_ = nullptr;
            delete prev;
            return;
        }

        cur->next_->prev_ = cur->prev_;
        cur->prev_->next_ = cur->next_;
        delete cur;
        return;
    }
    //...................................... 
    Hashmap_node<keyT>* find(keyT key) {
        size_t hash_value = hash_func(key);
        Hashmap_node<keyT>* ptr = hmap_[hash_value];

        while(ptr) 
        {
            if(ptr->key_ == key)
                return ptr;
            ptr = ptr->next_;
        }
        return nullptr;
    }
    //..................................................//
    //..................................................//
public:  // selectors
    int get_len() const {
        return len_;
    }

    Hashmap_node<keyT>** get_map() const {
        return hmap_;
    }
public: // operators
    //..................................................//
    //..................................................//
    Hashmap_node<keyT>* operator[](int n) const {
        if(n > len_ - 1) 
        { 
            std::cout << "error: n > len" << std::endl; 
            return nullptr;
        }
        return hmap_[n];
    }

}; // end class Map

}// end namespace


template <typename keyT> std::ostream& operator << (std::ostream &out, const Hashmap_space::Hashmap<keyT> &map) {
    int len = map.get_len();
    Hashmap_space::Hashmap_node<keyT>* cur = nullptr;
    Hashmap_space::Hashmap_node<keyT>* ptr_lst = nullptr;

    for(int i = 0; i < len; i++) {
        cur = map[i];
        if(cur) {
            out << "h[" << i << "]: ";
            ptr_lst = cur;
            while(ptr_lst) {
                out << ptr_lst->key_ << " ";
                ptr_lst = ptr_lst->next_;
            }
            out << "\n"; 
        }
    }


    return out;
} 
//......................................
template <typename keyT> std::istream& operator >> (std::istream &in, Hashmap_space::Hashmap<keyT> &map) {
    keyT tmp;
    in >> tmp;
    map.insert(tmp);
    return in;
}