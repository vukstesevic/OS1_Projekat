#ifndef PROJECT_BASE__SEM_HPP
#define PROJECT_BASE__SEM_HPP

#include "../h/TCB.hpp"


class _sem {
public:
    static int sem_open(_sem** s, int v);
    static int sem_close(_sem* s);

    static int sem_wait(_sem* id);
    static int sem_sgnal(_sem* id);

private:
    int value;

    void block();
    TCB* unblock();

    struct Elem{
        TCB* data;
        Elem* next=nullptr;
        Elem* prev=nullptr;
    };

    Elem* head;
    Elem* tail;


    friend class TCB;
    friend class Cbuffer;
    friend class Riscv;

};


#endif //PROJECT_BASE__SEM_HPP
