//
// Created by os on 6/22/22.
//

#include "../h/Scheduler.hpp"

Scheduler* Scheduler::instance = nullptr;

List<PCB> Scheduler::PCBQueue;

void Scheduler::put(PCB *p) {
    PCBQueue.addLast(p);
}

PCB *Scheduler::get() {
    return PCBQueue.removeFirst();
}

Scheduler &Scheduler::getInstance() {
    if(!instance){
        instance = new Scheduler();
    }

    return *instance;
}
