//
// Created by os on 6/24/22.
//

#include "../h/ksemaphore.hpp"

Ksemaphore *Ksemaphore::ksem_create(unsigned int init) {
    return new Ksemaphore(init);
}

void Ksemaphore::ksem_close() {
    while(blocked->peekFirst()){
        PCB* p = blocked->removeFirst();
        p->setReleased(true);
        Scheduler::put(p);
    }
    this->setOpen(false);
    delete this;
}

int Ksemaphore::ksem_wait() {
    if(--val < 0){
        PCB *old = PCB::running;
        if (!old->isFinished()) { blocked->addLast(old); }
        PCB::running = Scheduler::get();

        PCB::contextSwitch(&old->context, &PCB::running->context);
    }
    //if(PCB::running->isReleased()) return -1;
    return 0;
}

void Ksemaphore::ksem_signal() {
    if (++val <= 0) {
        PCB *p = blocked->removeFirst();
        Scheduler::put(p);
    }
}
