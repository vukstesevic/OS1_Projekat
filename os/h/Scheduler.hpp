#ifndef PROJECT_BASE_SCHEDULER_HPP
#define PROJECT_BASE_SCHEDULER_HPP

#include"../h/TCB.hpp"

class TCB;

class Scheduler {
private:
    struct Elem{
        TCB* data;
        Elem* next=nullptr;
        Elem* prev=nullptr;
    };

    static Elem* head;
    static Elem* tail;

    friend class _sem;

public:
    static TCB *get();

    static void put(TCB *ccb);
};


#endif //PROJECT_BASE_SCHEDULER_HPP
