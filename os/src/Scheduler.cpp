#include "../h/Scheduler.hpp"
#include "../lib/console.h"

Scheduler::Elem* Scheduler::head=nullptr;
Scheduler::Elem* Scheduler::tail=nullptr;

TCB *Scheduler::get()
{
    if(head == nullptr)return nullptr;
    Elem* elem = head;
    TCB* ret = elem->data;
    if(head->next) head->next->prev= nullptr;
    head = head->next;

    if(head == nullptr) tail = nullptr;

    MemoryAllocator::getInstance().mem_free((void*)elem);
    return ret;
}

void Scheduler::put(TCB *ccb)
{
    Elem* elem = (Elem*) MemoryAllocator::getInstance().mem_alloc((sizeof(Elem)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);
    if(elem==nullptr) {
        return;}
    elem->data = ccb;
    elem->next = 0;
    elem->prev=tail;
    if(tail){
        tail->next = elem;
        tail = elem;
    }else{
        head = tail = elem;
    }
}