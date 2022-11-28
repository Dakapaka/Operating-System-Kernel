//
// Created by os on 6/22/22.
//

#ifndef PROJECT_BASEV1_1_SCHEDULER_HPP
#define PROJECT_BASEV1_1_SCHEDULER_HPP

#include "../h/list.hpp"

class PCB;

class Scheduler{
private:
    Scheduler(){}
public:

    static List<PCB> PCBQueue;

    void *operator new(size_t n) {
        MemoryAllocator &m = m.getInstance();
        return m.kmem_alloc(n);
        //return __mem_alloc(n);
    }

    void operator delete(void *p) {
        MemoryAllocator &m = m.getInstance();
        m.kmem_free(p);
        //__mem_free(p);
    }

    static Scheduler* instance;

    Scheduler(Scheduler& s) = delete;

    void operator=(const Scheduler& s) = delete;

    static Scheduler& getInstance();

    static void put(PCB* p);

    static PCB* get();
};


#endif //PROJECT_BASEV1_1_SCHEDULER_HPP
