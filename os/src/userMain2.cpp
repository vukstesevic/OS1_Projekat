//
// Created by os on 9/23/23.
//

#include "../h/syscall_c.hpp"
#include "../h/TCB.hpp"
#include "printing.hpp"

static const int N=50;
static sem_t waitForAll;
static thread_t tcb[N];

static void workerBodyA(void*) {
    for (int i = 0; i < 3; i++) {
        sem_wait(waitForAll);
        printString("USO U SEKCIJU ");
        printInt(TCB::running->getMyId());
        printString("\n");
        thread_dispatch();
        sem_signal(waitForAll);
        printString("IZISO IZ SEKCIJU ");
        printInt(TCB::running->getMyId());
        printString("\n");
    }
}

static bool isfinished(){
    for(int i=0;i<N;i++){
        if(!tcb[i]->isFinished()) return true;
    }
    return false;
}

void userMain2(){
    sem_open(&waitForAll, 5);

    for (int i = 0; i < N; i++) {
        thread_create(tcb+i, workerBodyA, nullptr);
    }

    thread_dispatch();

    while(isfinished())thread_dispatch();

    sem_close(waitForAll);
}
