#include "../h/syscall_c.hpp"

extern void userMain(void*);

int main(){

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    Riscv::mc_sstatus(Riscv::SSTATUS_SPIE);

    MemoryAllocator& m = m.getInstance();
    PCB* mainThread = (PCB*)m.kmem_alloc(sizeof(PCB));

    PCB::running = mainThread;

    auto* systemStack = (uint64*)m.kmem_alloc(DEFAULT_STACK_SIZE);

    mainThread->setSystemStack(systemStack);

    __asm__ volatile("ecall");

    PCB* thread;

    thread_create(&thread, userMain, nullptr);

    while(!thread->isFinished()) {
        thread_dispatch();
    }

    return 0;
}
