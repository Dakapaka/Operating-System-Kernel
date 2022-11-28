//
// Created by os on 6/17/22.
//

#ifndef PROJECT_BASEV1_1_MEMORYALLOCATOR_HPP
#define PROJECT_BASEV1_1_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

class MemoryAllocator {
private:

    typedef struct Mem{
        Mem* next;
        Mem* prev;
        size_t size;
    }Mem;

    Mem* freeMemHead;
    Mem* occuMemHead;

    MemoryAllocator(){}

public:

    static MemoryAllocator* instance;

    MemoryAllocator(MemoryAllocator& m) = delete;

    void operator=(const MemoryAllocator& m) = delete;

    static MemoryAllocator& getInstance();

    void* kmem_alloc(size_t);

    int kmem_free(void*);

};


#endif //PROJECT_BASEV1_1_MEMORYALLOCATOR_HPP
