//
// Created by os on 1/9/24.
//

#include "../h/sleepingTCB.hpp"
#include "../h/TCB.hpp"

sleepingTCB::Elem* sleepingTCB::head=nullptr;
sleepingTCB::Elem* sleepingTCB::tail=nullptr;


int sleepingTCB::put(uint64 time) {
    Elem* elem = (Elem*) MemoryAllocator::getInstance().mem_alloc((sizeof(Elem)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);
    if(elem==nullptr || time==0) {
        return -1;}
    TCB::running->setTime(time);
    TCB::running->setSleeping(true);
    elem->data = TCB::running;
    elem->next = nullptr;
    elem->prev=tail;
    if(tail){
        tail->next = elem;
        tail = elem;
    }else{
        head = tail = elem;
    }
    return 0;
}

void sleepingTCB::wakeUp() {
    if (head == nullptr) return;
    Elem *curr=head;
    while(curr){
        curr->data->timeToSleep--;
        curr=curr->next;
    }
    Elem *curr1=head;
    while(curr1){
        if(curr1->data->getTime()<=0){
            Elem* elem=curr1;
            curr1 = curr1->next;
            if(elem==head) head=head->next;
            if(elem==tail) tail=tail->prev;
            if(elem->prev)elem->prev->next=elem->next;
            if(elem->next) elem->next->prev=elem->prev;
            elem->data->setSleeping(false);
            Scheduler::put(elem->data);
            MemoryAllocator::getInstance().mem_free((void*)elem);
        }else curr1 = curr1->next;
    }

}




