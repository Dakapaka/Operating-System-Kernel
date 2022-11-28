//
// Created by os on 6/27/22.
//

#include "../lib/mem.h"

extern void* mem_alloc(size_t);
extern int mem_free(void*);

void* operator new (size_t n){
    return mem_alloc(n);
}
void operator delete (void* p){
    mem_free(p);
}