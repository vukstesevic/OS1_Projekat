//
// Created by os on 1/9/24.
//

#ifndef PROJECT_BASE_V1_1_SLEEPINGTCB_HPP
#define PROJECT_BASE_V1_1_SLEEPINGTCB_HPP
#include"../h/TCB.hpp"

class TCB;

class sleepingTCB {
private:
    struct Elem{
        TCB* data;
        Elem* next=nullptr;
        Elem* prev=nullptr;
    };

    static Elem* head;
    static Elem* tail;

    friend class Riscv;

public:

static int put(uint64 time);
static void wakeUp();

};


#endif //PROJECT_BASE_V1_1_SLEEPINGTCB_HPP
