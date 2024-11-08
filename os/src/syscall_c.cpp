#include "../h/syscall_c.hpp"
#include "../lib/console.h"


void* mem_alloc (size_t size){
    uint64 num=0x01;
    size_t numOfBlks=(size+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;
    __asm__ volatile("mv a1, %0" :  : "r"(numOfBlks));
    __asm__ volatile("mv a0,%0" : :"r" (num));
    __asm__ volatile("ecall");
    uint64 addr;
    __asm__ volatile("mv %0, a0":"=r"(addr));
    return (void*)addr;
}

int mem_free (void* a){
    uint64 num=0x02;
    uint64 addr=(uint64) a;
    __asm__ volatile("mv a1, %0" :  : "r"(addr));
    __asm__ volatile("mv a0,%0" : :"r" (num));
    __asm__ volatile("ecall");
    uint64 free;
    __asm__ volatile("mv %0, a0":"=r"(free));
    return free;
}


int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg){
    uint64 num=0x11;
    uint64 fhandle=(uint64) handle;
    uint64 fsr=(uint64) start_routine;
    uint64 farg=(uint64) arg;
    __asm__ volatile("mv a3, %0" :  : "r"(farg));
    __asm__ volatile("mv a2,%0" : :"r" (fsr));
    __asm__ volatile("mv a1, %0" :  : "r"(fhandle));
     __asm__ volatile("mv a0,%0" : :"r" (num));
    __asm__ volatile("ecall");
    uint64 thread;
    __asm__ volatile("mv %0, a0":"=r"(thread));
    return thread;
}


int thread_exit (){
    uint64 num=0x12;
    __asm__ volatile("mv a0,%0" : :"r" (num));
    __asm__ volatile("ecall");
    uint64 exit;
    __asm__ volatile("mv %0, a0":"=r"(exit));
    return exit;
}

void thread_dispatch (){
    uint64 num=0x13;
    __asm__ volatile("mv a0,%0" : :"r" (num));
    __asm__ volatile("ecall");
}

void thread_join (thread_t handle){
    uint64 num=0x14;
    uint64 fhandle=(uint64) handle;
    __asm__ volatile("mv a1, %0" :  : "r"(fhandle));
    __asm__ volatile("mv a0,%0" : :"r" (num));
    __asm__ volatile("ecall");
}

int sem_open (sem_t* handle, unsigned init){
    uint64 num=0x21;
    uint64 fhandle=(uint64) handle;
    uint64 finit= (uint64) init;
    __asm__ volatile("mv a2, %0" :  : "r"(finit));
    __asm__ volatile("mv a1, %0" :  : "r"(fhandle));
    __asm__ volatile("mv a0,%0" : :"r" (num));
    __asm__ volatile("ecall");
    uint64 sem;
    __asm__ volatile("mv %0, a0":"=r"(sem));
    return sem;
}

int sem_close (sem_t handle){
    uint64 num=0x22;
    uint64 fhandle=(uint64) handle;
    __asm__ volatile("mv a1, %0" :  : "r"(fhandle));
    __asm__ volatile("mv a0,%0" : :"r" (num));
    __asm__ volatile("ecall");
    uint64 close;
    __asm__ volatile("mv %0, a0":"=r"(close));
    return close;
}

int sem_wait (sem_t id){
    uint64 num=0x23;
    uint64 fid=(uint64) id;
    __asm__ volatile("mv a1, %0" :  : "r"(fid));
    __asm__ volatile("mv a0,%0" : :"r" (num));
    __asm__ volatile("ecall");
    uint64 wait;
    __asm__ volatile("mv %0, a0":"=r"(wait));
    return wait;
}

int sem_signal (sem_t id){
    uint64 num=0x24;
    uint64 fid=(uint64) id;
    __asm__ volatile("mv a1, %0" :  : "r"(fid));
    __asm__ volatile("mv a0,%0" : :"r" (num));
    __asm__ volatile("ecall");
    uint64 signal;
    __asm__ volatile("mv %0, a0":"=r"(signal));
    return signal;
}

int time_sleep (time_t t){
    uint64 num=0x31;
    __asm__ volatile("mv a1, %0" :  : "r"((uint64)t));
    __asm__ volatile("mv a0,%0" : :"r" (num));
    __asm__ volatile("ecall");
    uint64 sleep;
    __asm__ volatile("mv %0, a0":"=r"(sleep));
    return sleep;
}

char getc(){
    uint64 num=0x41;
   __asm__ volatile("mv a0, %0": :"r"(num));
    __asm__ volatile("ecall");
    uint64 chr;
    __asm__ volatile("mv %0, a0":"=r"(chr));
    return (char)chr;
}

void putc(char c){
    uint64 num=0x42;
    uint64 chr=(uint64) c;
    __asm__ volatile("mv a1, %0" :  : "r"(chr));
    __asm__ volatile("mv a0,%0" : :"r" (num));
    __asm__ volatile("ecall");
}