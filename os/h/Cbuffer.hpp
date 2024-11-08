//
// Created by os on 1/8/24.
//

#ifndef PROJECT_BASE_V1_1_CBUFFER_HPP
#define PROJECT_BASE_V1_1_CBUFFER_HPP
#include "../h/_sem.hpp"


class Cbuffer {
private:
    static const uint64 KB_SIZE=2048;
    char buffer[KB_SIZE];
    uint64 head, tail, count;
    _sem* itemAvailable;
    _sem* spaceAvailable;
public:


    Cbuffer() : head(0), tail(0), count(0) {
        _sem::sem_open(&itemAvailable, 0);
    }
    uint64 getCount() const{return count;}
    void putc(char c);
    char getc();
};


#endif //PROJECT_BASE_V1_1_CBUFFER_HPP
