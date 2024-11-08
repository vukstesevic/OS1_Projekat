#include "../h/_console.hpp"
#include "../h/_sem.hpp"



int _sem::sem_open(_sem **s, int v) {
    _sem* sem = (_sem*) MemoryAllocator::getInstance().mem_alloc((sizeof(_sem)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);
    if(sem== nullptr) return -1;
    sem->value=v;
    sem->head=nullptr;
    sem->tail=nullptr;
    *s=sem;
    return 0;
}

int _sem::sem_close(_sem *s) {
    if(s== nullptr) return -1;
    TCB* ip=s->unblock();
    while(ip){
        ip=s->unblock();
    }
    MemoryAllocator::getInstance().mem_free(s);
    s=nullptr;
    return 0;
}



void _sem::block() {
    if(TCB::running->isSleeping()) return;
    TCB::running->blocked=true;
    Elem* newNode=(Elem*) MemoryAllocator::getInstance().mem_alloc((sizeof(Elem)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);
    newNode->data=TCB::running;
    newNode->next= nullptr;
    newNode->prev= nullptr;
    if(tail!=nullptr){
        newNode->prev=tail;
        tail->next=newNode;
        tail=newNode;
    }else{
        tail=head=newNode;
    }
    TCB::dispatch();
}

TCB* _sem::unblock() {
    if(head==nullptr) return nullptr;
    TCB* ep =head->data;
    Elem* old=head;
    if(head->next) head->next->prev=nullptr;
    head=head->next;
    if(head==nullptr) tail= nullptr;
    ep->blocked=false;
    Scheduler::put(ep);
    MemoryAllocator::getInstance().mem_free((void*)old);
    return ep;
}

int _sem::sem_wait(_sem* id) {
    if(id==nullptr) return-1;
    if(--(id->value)<0) {
        id->block();
    }
    return 0;
}

int _sem::sem_sgnal(_sem* id) {
    if(id==nullptr) return-1;
    if(++(id->value)<=0) {
        id->unblock();
    }
    return 0;
}
