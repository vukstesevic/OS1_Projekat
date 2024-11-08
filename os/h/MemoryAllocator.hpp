#ifndef PROJECT_BASE_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

class MemoryAllocator{
private:
    struct FreeMem {
        FreeMem* next;
        FreeMem* prev;
        size_t size;
    };
    FreeMem *fmem_head=nullptr;
    MemoryAllocator();
public:
    static MemoryAllocator& getInstance();
    void* mem_alloc(size_t size);
    void tryToJoin(FreeMem* cur);
    int mem_free(void*);


};


#endif //PROJECT_BASE_MEMORYALLOCATOR_HPP
