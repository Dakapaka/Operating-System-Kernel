//
// Created by os on 6/22/22.
//

#include "../h/PCB.hpp"

PCB* PCB::running = nullptr;

void PCB::kdispatch() {
    PCB *old = running;
    if (!old->isFinished()) { Scheduler::put(old); }
    running = Scheduler::get();

    PCB::contextSwitch(&old->context, &running->context);
}

void PCB::yield() {
    //Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    uint64 a0;
    __asm__ volatile("mv %0, a0" : "=r" (a0));
    uint64 t = 0x03;
    __asm__ volatile("mv a0, %0" : : "r" (t));
    __asm__ volatile("ecall");
    __asm__ volatile("mv a0, %0" : : "r" (a0));
}

void PCB::threadWrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    PCB::yield();
}

PCB* PCB::kthread_create(PCB::Body body, void *arg, uint64* stack_space) {
    return new PCB(body, arg, stack_space);
}

void PCB::setSystemStack(uint64 *pInt) {
    this->stack = pInt;
}