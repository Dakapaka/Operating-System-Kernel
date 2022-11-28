//
// Created by os on 6/22/22.
//

#ifndef PROJECT_BASEV1_1_PCB_HPP
#define PROJECT_BASEV1_1_PCB_HPP
#include "../h/MemoryAllocator.hpp"
#include "../h/riscv.hpp"
#include "../h/Scheduler.hpp"
#include "../lib/mem.h"

class PCB {
public:
    ~PCB(){
        this->setFinished(true);
        MemoryAllocator& m = m.getInstance();
        m.kmem_free(stack);
        //__mem_free(stack);
    }

    typedef PCB *thread_t;

    using Body = void (*)(void *);

    bool isFinished() const {
        return finished;
    }

    void setFinished(bool value) {
        finished = value;
    }

    bool isReleased() const {
        return released;
    }

    void setReleased(bool value) {
        released = value;
    }

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

    static PCB* kthread_create(Body body, void* arg, uint64* stack_space);

    static PCB *running;

    static void yield();

    static void threadWrapper();

    Scheduler& s = s.getInstance();

    void setSystemStack(uint64 *pInt);

private:

    PCB(Body body, void *arg, uint64* stack_space){
        this->body = body;
        this->stack = (this->body != nullptr) ? stack_space : nullptr;
        this->context.ra = (uint64) &threadWrapper;
        this->context.sp = (this->stack != nullptr) ? (uint64)&stack[DEFAULT_STACK_SIZE] : 0;
        this->finished = false;
        this->arg = arg;
        this->released = false;
        if (body != nullptr){
            Scheduler::put(this);
        }
    }

    struct Context{
        uint64 ra;
        uint64 sp;
    };

    Body body;
    Context context;
    uint64* stack;
    bool finished;
    void* arg;
    bool released;

    friend class Riscv;
    friend class Ksemaphore;

    static void contextSwitch(Context* oldContext, Context* runningContext);

    static void kdispatch();
};


#endif //PROJECT_BASEV1_1_PCB_HPP