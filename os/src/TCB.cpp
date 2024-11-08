#include "../h/TCB.hpp"
#include "../h/_console.hpp"

TCB* TCB::running = nullptr;

int TCB::ID=0;

uint64 TCB::timeSliceCounter=0;



int TCB::createThread(TCB** handle,TCB::Body body, void* arg, void* stack_space) {
    TCB* thrd = (TCB*)MemoryAllocator::getInstance().mem_alloc((sizeof(TCB)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);
    if(thrd == nullptr){return -1;}
    thrd->body = body;
    thrd->arg= arg;
    thrd->blocked=false;
    thrd->sleeping=false;
    thrd->timeToSleep=0;
    thrd->timeSlice=DEFAULT_TIME_SLICE;
    thrd->myID=++ID;
    if(body==nullptr) {
        MemoryAllocator::getInstance().mem_free(stack_space);
        thrd->stack = nullptr;
    }
    else thrd->stack=(char*)stack_space;
    thrd->context={(uint64)&threadWrapper, thrd->stack!=nullptr ? (uint64)&thrd->stack[DEFAULT_STACK_SIZE]: 0};
    thrd->finished=false;
    thrd->semaphore=nullptr;
    *handle = thrd;
    if(body!= nullptr){Scheduler::put(*handle);}
    return 0;
}

void TCB::yield() {
    __asm__ volatile("li a0, 0x13");
    __asm__ volatile ("ecall");
}

void TCB::dispatch() {
    TCB* old=running;
    if(!old->isFinished() && !old->blocked && !old->sleeping){Scheduler::put(old);}
    running=Scheduler::get();
    contextSwitch(&old->context,&running->context);
}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    thread_exit();
}


int TCB::getMyId() const{
    return myID;
}
