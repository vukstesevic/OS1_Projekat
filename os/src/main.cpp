#include "../lib/console.h"
#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/Riscv.hpp"
#include "../h/TCB.hpp"
#include "../h/syscall_cpp.hpp"
#include"../h/syscall_c.hpp"
#include "../h/_console.hpp"



void userMain();

void UserMainWrapper(void* arg){
    userMain();
}


void idle(void* args){
    while(true){ thread_dispatch();}
}

void main(){

     __asm__ volatile ("csrw stvec, %[vector]" : : [vector] "r"((uint64)&Riscv::supervisorTrap+1));

    Cbuffer *ob= new Cbuffer();
    Cbuffer* ib= new Cbuffer();

     _console::getConsole().setOutput(ob);
     _console::getConsole().setInput(ib);


    TCB* nit;
    thread_create(&nit, nullptr, nullptr);
    TCB::running=nit;


    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    TCB* idlethrd;
    thread_create(&idlethrd, idle, nullptr);

    TCB* consoleThread;
    thread_create (&consoleThread, &_console::outputBodyWrapper, nullptr);

    TCB* userMainThread;
    thread_create(&userMainThread, UserMainWrapper, nullptr);

    thread_join(userMainThread);


    mem_free(consoleThread);
    mem_free(userMainThread);
    mem_free(idlethrd);
    mem_free(nit);

}