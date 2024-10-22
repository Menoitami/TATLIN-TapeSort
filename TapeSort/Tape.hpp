#pragma once

#include <list>
#include <initializer_list>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <concepts>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <chrono>
#include <thread>

template <std::totally_ordered T>
class Tape {
public:
    
    Tape();

    T read();                     
    void write(T el);             
    void move(const int& i);     
    void goToFirst();                 
    void pop_front();             
    void pop_back();              
    int getDistanceFromStart();   
    int getSize();
    void goToLast();
    bool isEnd();
    bool isStart();

   
    using iterator = typename std::list<T>::iterator;
    using const_iterator = typename std::list<T>::const_iterator;

    Tape<T>& operator++();
    Tape<T>& operator--();
    
   

private:
    std::list<T> data;
    iterator iter;  
    int size;      

    int read_delay_us{};
    int write_delay_us{};
    int move_delay_us{};

    iterator begin() { return data.begin(); }
    iterator end() { return data.end(); }
    const_iterator begin() const { return data.begin(); }
    const_iterator end() const { return data.end(); }

    void waitMicroseconds(int microsec);
};


template <std::totally_ordered T>
Tape<T>::Tape() : size(0), iter(begin()), data({}) {
    boost::property_tree::ptree pt;
    try {
        boost::property_tree::read_json("../config.json", pt);
        read_delay_us = pt.get<int>("read_delay_us", 0);   
        write_delay_us = pt.get<int>("write_delay_us", 0);
        move_delay_us = pt.get<int>("move_delay_us", 0);
    } catch (const std::exception& e) {
        std::cerr << "Error reading JSON file: " << e.what() << std::endl;
    }
}




template <std::totally_ordered T>
T Tape<T>::read() {
    waitMicroseconds( read_delay_us);
    if (iter != end()) {
        return *iter;
    }
    throw std::out_of_range("End of tape reached.");
}

template <std::totally_ordered T>
void Tape<T>::write(T el) {
    waitMicroseconds( write_delay_us);
    if (iter != data.end()) {
        *iter = el;
    } 
    else{
        throw std::out_of_range("End of tape reached.");
    }
}

template <std::totally_ordered T>
void Tape<T>::move(const int& i) {
    
    if (i == 0) return;

    if (i>0){
        for (int _=0; _<i; _++){
            waitMicroseconds( move_delay_us);
            if (iter!= --data.end())++iter;
            else{
                data.emplace_back(T{});
                ++size;
                ++iter;
            }
        }
    }
    else{
        for (int _=0; _<-i; _++){
            waitMicroseconds( move_delay_us);
            if (iter!= data.begin()) --iter;
            else{
                data.emplace_front(T{});
                ++size;
                --iter;
            }
        }
    }

}
template <std::totally_ordered T>
void Tape<T>::goToFirst(){
    while(iter!= data.begin()){
        waitMicroseconds( move_delay_us);
        --iter;
    }
}

template <std::totally_ordered T>
void Tape<T>::goToLast(){
    auto n = --data.end();
    while(iter!= n){
        waitMicroseconds( move_delay_us);
        ++iter;
    }
}

template <std::totally_ordered T>
void Tape<T>::pop_front(){
    
    while (iter!=data.begin()){
        waitMicroseconds( move_delay_us);
        --iter;
    }
    waitMicroseconds( move_delay_us);
    ++iter;
    data.pop_front();
    --size;
}

template <std::totally_ordered T>
void Tape<T>::pop_back(){

    auto j = --data.end();
    while (iter!=j){
        waitMicroseconds( move_delay_us);
        ++iter;
    }
    waitMicroseconds( move_delay_us);
    --iter;
    data.pop_back();
    --size;
}

template <std::totally_ordered T>
int Tape<T>::getDistanceFromStart() {
    return std::distance(data.begin(), iter);
}


template <std::totally_ordered T>
Tape<T>& Tape<T>::operator++(){
    move(1);
    return *this;
}

template <std::totally_ordered T>
Tape<T>& Tape<T>::operator--(){
    move(-1);
    return *this;
}

template <std::totally_ordered T>
int Tape<T>::getSize(){
    return size;
}


template <std::totally_ordered T>
bool Tape<T>::isEnd(){

    if(this->getDistanceFromStart()==size-1) return true;
    return false;

}

template <std::totally_ordered T>
bool Tape<T>::isStart(){

    if(this->getDistanceFromStart()==0) return true;
    return false;

}


template <std::totally_ordered T>
void Tape<T>::waitMicroseconds(int microsec){

    auto start = std::chrono::high_resolution_clock::now();
    auto end = start;
    
    while (std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() < microsec) {
        end = std::chrono::high_resolution_clock::now();
    }

}