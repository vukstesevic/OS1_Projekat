#ifndef PROJECT_BASE_TCB_HPP
#define PROJECT_BASE_TCB_HPP

#include "MemoryAllocator.hpp"
#include "Scheduler.hpp"
#include "../lib/hw.h"
#include "../h/Riscv.hpp"
#include"syscall_c.hpp"

class Scheduler;
class TCB {
public:
    ~TCB(){ MemoryAllocator::getInstance().mem_free(stack);}
    bool isFinished() const {return finished;}
    void setFinished(bool value) {finished=value;}
    bool isSleeping(){return sleeping;}
    void setSleeping(bool v){sleeping=v;}
    void setTime(uint64 t){timeToSleep=t;}
    uint64 getTime(){return timeToSleep;}
    uint64 getTimeSlice() const{
        return timeSlice;
    }

    using Body= void(*)(void*);
    static int createThread(TCB** handle,Body body, void* arg, void* stack_space);
    static int createThreadCon(TCB** handle,Body body, void* arg, void* stack_space);
    static void yield();
    static TCB* running;

    int getMyId() const;

private:
    struct Context{
        uint64 ra;
        uint64 sp;
    };


    static int ID;
    static uint64 timeSliceCounter;
    int myID;
    Body body;
    void* arg;
    uint64 timeSlice;
    uint64 timeToSleep;
    char* stack;
    Context context;
    bool finished;
    bool blocked;
    bool sleeping;
    _sem* semaphore;

    static void contextSwitch(Context* oldContext, Context *runningContext);


    static void dispatch();

    friend class Riscv;

    friend class Scheduler;

    friend class sleepingTCB;

    static void threadWrapper();
    static void threadWrapperCon();

    friend class _sem;

};


#endif //PROJECT_BASE_TCB_HPP
