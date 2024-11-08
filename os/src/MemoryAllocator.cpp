#include "../h/MemoryAllocator.hpp"
#include "../lib/console.h"


MemoryAllocator::MemoryAllocator() {
    uint64 BlockNumber=(uint64)(((uint64*)HEAP_END_ADDR-(uint64*)HEAP_START_ADDR-sizeof(MemoryAllocator)+1)/MEM_BLOCK_SIZE);
    fmem_head=(FreeMem*)HEAP_START_ADDR;
    fmem_head->next=nullptr;
    fmem_head->prev=nullptr;
    fmem_head->size=BlockNumber;
}

MemoryAllocator &MemoryAllocator::getInstance() {
    static MemoryAllocator instance;
    return instance;
}

void *MemoryAllocator::mem_alloc(size_t size) {
    FreeMem* alloc= nullptr;
    for (FreeMem* cur=fmem_head; cur!=nullptr; cur=cur->next) {
        if(cur->size<size)continue;
        alloc=cur;
        if(cur->size-size==0){
            if (cur->prev!= nullptr) cur->prev->next=cur->next;
            else fmem_head=cur->next;
            if (cur->next!=nullptr) cur->next->prev=cur->prev;
            cur->size=size;
            return (uint64*) alloc+ sizeof(FreeMem);
        }
        FreeMem* newBlock=(FreeMem*)((uint64*)cur+size*MEM_BLOCK_SIZE);
        if(cur->prev!=nullptr) cur->prev->next=newBlock;
        else fmem_head=newBlock;
        if(cur->next!=nullptr) cur->next->prev=newBlock;
        newBlock->prev=cur->prev;
        newBlock->next=cur->next;
        newBlock->size=cur->size-size;
        cur->size=size;
        return (uint64*) alloc+ sizeof(FreeMem);
        }
    return alloc;
}

int MemoryAllocator::mem_free(void *addr) {
    if((uint64*)addr>(uint64*)HEAP_END_ADDR || (uint64*)addr<(uint64*)HEAP_START_ADDR) return -1;
    FreeMem* cur=nullptr;
    if (!fmem_head || (char*)addr<(char*)fmem_head){
        cur = nullptr;}
    else{
        for (cur = fmem_head; cur->next != nullptr && (char*)addr > (char*)cur->next;
             cur=cur->next);
    }//trazi se poslednji segment pre ovog kojeg zelimo da dealociramo
    FreeMem* newSeg=(FreeMem*) ((uint64*)addr- sizeof(FreeMem));
    newSeg->prev=cur;
    if(cur!=nullptr) newSeg->next=cur->next;
    else newSeg->next=fmem_head;
    if(newSeg->next!= nullptr) newSeg->next->prev=newSeg;
    if(cur!=nullptr) cur->next=newSeg;
    else fmem_head = newSeg;
    if(newSeg!= nullptr)tryToJoin(newSeg);
    if(cur!=nullptr)tryToJoin(cur);
    return 0;
}

void MemoryAllocator::tryToJoin(MemoryAllocator::FreeMem *cur) {
    char* nxt=(char*)(cur->next);
    char*prob =(char*)cur+cur->size*MEM_BLOCK_SIZE;
    if(cur->next && nxt==prob){
        cur->size+=cur->next->size;
        cur->next=cur->next->next;
        if(cur->next) cur->next->prev=cur;
    }
}
