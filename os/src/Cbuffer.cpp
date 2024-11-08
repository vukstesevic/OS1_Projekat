//
// Created by os on 1/8/24.
//

#include "../h/Cbuffer.hpp"


void Cbuffer::putc(char c) {
    if(count<KB_SIZE) {
        buffer[tail] = c;
        tail = (tail + 1) % KB_SIZE;
        count++;
        _sem::sem_sgnal(itemAvailable);
    }
}

char Cbuffer::getc() {
    sem_wait(itemAvailable);
    char c = buffer[head];
    head = (head + 1) % KB_SIZE;
    count--;
    return c;
}


