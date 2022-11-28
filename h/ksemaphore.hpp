//
// Created by os on 6/24/22.
//

#ifndef PROJECT_BASEV1_1_KSEMAPHORE_HPP
#define PROJECT_BASEV1_1_KSEMAPHORE_HPP


#include "../lib/hw.h"
#include "../lib/mem.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/list.hpp"
#include "../h/PCB.hpp"

//extern "C" uint64 copy_and_swap(uint64 &lock, uint64 expected, uint64 desired);

class Ksemaphore {
public:
    void *operator new(size_t n){
        MemoryAllocator& m = m.getInstance();
        return m.kmem_alloc(n);
        //return __mem_alloc(n);
    }

    void operator delete(void* p){
        MemoryAllocator& m = m.getInstance();
        m.kmem_free(p);
        //__mem_free(p);
    }

    typedef Ksemaphore* sem_t;

    static Ksemaphore* ksem_create(unsigned init);

    void ksem_close();

    int ksem_wait();

    void ksem_signal();

    bool isOpen() const{ return open; }

    void setOpen(bool status){ Ksemaphore::open = status; }

private:
    Ksemaphore(int init) : val(init), open(true), blocked(new List<PCB>()){}

    int val;
    bool open;
    List<PCB>* blocked;
};

#endif //PROJECT_BASEV1_1_KSEMAPHORE_HPP
