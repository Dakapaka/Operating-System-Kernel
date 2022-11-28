#include "../h/MemoryAllocator.hpp"

MemoryAllocator* MemoryAllocator::instance = nullptr;

void *MemoryAllocator::kmem_alloc(size_t size) {

    Mem* curO = 0;
    Mem* curF = 0;

    if(occuMemHead != 0)
        for(curO = occuMemHead; curO->next != nullptr; curO = curO->next);

    for(curF = freeMemHead; curF != 0; curF = curF->next){
        if(curF->size < size) continue;

        if(curF->size - size <= sizeof(Mem)){
            if(curF->prev) curF->prev->next = curF->next;
            else freeMemHead = curF->next;
            if(curF->next) curF->next->prev = curF->prev;
            if(occuMemHead != 0){
                curO->next = curF;
                curF->prev = curO;
                curF->next = 0;
                curF->size = size;
                void* addr = (void*)((size_t)curF + sizeof(Mem));
                return addr;
            }else{
                occuMemHead = curF;
                occuMemHead->next = occuMemHead->prev = 0;
                occuMemHead->size = size;
                void* addr = (void*)((size_t)occuMemHead + sizeof(Mem));
                return addr;
            }
        }
        else{
            Mem* newFree = (Mem*)((size_t)curF + size + sizeof(Mem));
            if(curF->prev) curF->prev->next = newFree;
            else freeMemHead = newFree;
            if (curF->next) curF->next->prev = newFree;
            newFree->prev = curF->prev;
            newFree->next = curF->next;
            newFree->size = curF->size-size;
            if(occuMemHead != 0){
                curO->next = curF;
                curF->prev = curO;
                curF->next = 0;
                curF->size = size;
                void* addr = (void*)((size_t)curF + sizeof(Mem));
                return addr;
            }else{
                occuMemHead = curF;
                occuMemHead->next = occuMemHead->prev = 0;
                occuMemHead->size = size;
                void* addr = (void*)((size_t)occuMemHead + sizeof(Mem));
                return addr;
            }
        }
    }
    return nullptr;
}

int MemoryAllocator::kmem_free(void * addr) {
    int k = 0;
    Mem* curO = 0;
    Mem* curF = 0;

    if(occuMemHead == 0) return -1;

    for(curO = occuMemHead; curO != 0; curO = curO->next){
        if((size_t)curO + sizeof(Mem) == (size_t)addr) {
            k = 1;
            break;
        }
        else continue;
    }

    if(!k) return -1;
    else{
        if(curO->prev) curO->prev->next = curO->next;
        else occuMemHead = curO->next;
        if(curO->next) curO->next->prev = curO->prev;
    }

    if((size_t)curO < (size_t)freeMemHead){
        Mem* tmp = freeMemHead;
        curO->next = tmp;
        tmp->prev = curO;
        freeMemHead = curO;
        freeMemHead->prev = 0;
    }

    else{
        curF = freeMemHead;
        while(((size_t)curF->next < (size_t)curO) && curF->next){
            curF = curF->next;
        }
        curO->next = curF->next;
        if( curF->next->prev ) curF->next->prev = curO;
        curO->prev = curF;
        curF->next = curO;
    }

    if(curO->next && (size_t)curO + curO->size == (size_t)curO->next){
        curO->size += curO->next->size;
        curO->next = curO->next->next;
        if(curO->next) curO->next->prev = curO;
    }

    if(curO->prev && (size_t)curO->prev + curO->prev->size == (size_t)curO) {
        curO->prev->size += curO->size;
        curO->prev->next = curO->next;
        if (curO->next) curO->next->prev = curO->prev;
    }

    return 0;
}

MemoryAllocator &MemoryAllocator::getInstance() {
    if (!instance) {
        instance = (MemoryAllocator*) HEAP_START_ADDR;
        instance->freeMemHead = (Mem*) ((size_t)HEAP_START_ADDR + sizeof(MemoryAllocator));
        instance->occuMemHead = 0;
        instance->freeMemHead->size = (size_t)HEAP_END_ADDR - (size_t)instance->freeMemHead + 1;
        instance->freeMemHead->next = instance->freeMemHead->prev = 0;
    }

    return *instance;
}