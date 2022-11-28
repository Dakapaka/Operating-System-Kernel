#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/PCB.hpp"
#include "../lib/console.h"
#include "../lib/mem.h"
#include "../h/ksemaphore.hpp"

void Riscv::popSppSpie() {
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}

void Riscv::handleSupervisorTrap(){

    uint64 scause = r_scause();

    if(scause == 0x0000000000000008UL){
        uint64 k;

        //__asm__ volatile("ld a0, 10 * 8(s0)");
        __asm__ volatile("mv %0, a0" : "=r" (k));

        switch(k){
            //mem_alloc
            case 0x01: {
                size_t size;
                __asm__ volatile("mv %0, a1" : "=r" (size));
                if(size % MEM_BLOCK_SIZE)
                    size = (size / MEM_BLOCK_SIZE + 1) * MEM_BLOCK_SIZE;
                MemoryAllocator &m = m.getInstance();
                void* addr = m.kmem_alloc(size);
                //void* addr = __mem_alloc(size);
                __asm__ volatile("mv a0, %0" : : "r" (addr));
                uint64 sepc = r_sepc() + 4;
                w_sepc(sepc);
                break;
            }
                //mem_free
            case 0x02: {
                void* addr;
                __asm__ volatile("mv %0, a1" : "=r" (addr));
                MemoryAllocator &m = m.getInstance();
                int ret = m.kmem_free(addr);
                //int ret = __mem_free(addr);
                __asm__ volatile("mv a0, %0" : : "r" (ret));
                uint64 sepc = r_sepc() + 4;
                w_sepc(sepc);
                break;
            }
                //yield
            case 0x03:{
                uint64 sepc = r_sepc() + 4;
                uint64 sstatus = r_sstatus();
                PCB::kdispatch();
                w_sstatus(sstatus);
                w_sepc(sepc);
                break;
            }
                //thread_create
            case 0x11:{
                __asm__ volatile("ld a1, 11 * 8(s0)");
                __asm__ volatile("ld a2, 12 * 8(s0)");
                __asm__ volatile("ld a3, 13 * 8(s0)");
                __asm__ volatile("ld a4, 14 * 8(s0)");
                int ret;
                PCB::thread_t* handle;
                PCB::Body body;
                uint64* stack_space;
                void* arg;
                __asm__ volatile("mv %0, a1" : "=r" (handle));
                __asm__ volatile("mv %0, a2" : "=r" (body));
                __asm__ volatile("mv %0, a3" : "=r" (arg));
                __asm__ volatile("mv %0, a4" : "=r" (stack_space));
                PCB* p = PCB::kthread_create(body, arg, stack_space);
                *handle = p;
                if (!handle) ret = -1;
                else ret = 0;
                //__asm__ volatile("mv a1, %0" : : "r" (handle));
                __asm__ volatile("mv a0, %0" : : "r" (ret));
                uint64 sepc = r_sepc() + 4;
                w_sepc(sepc);
                break;
            }
                //thread_dispatch
            case 0x13:{
                uint64 sepc = r_sepc() + 4;
                uint64 sstatus = r_sstatus();
                PCB::kdispatch();
                w_sstatus(sstatus);
                w_sepc(sepc);
                break;
            }
                //thread_exit
            case 0x12: {
                int ret = 0;
                PCB::running->setFinished(true);
                uint64 sepc = r_sepc() + 4;
                uint64 sstatus = r_sstatus();
                PCB::kdispatch();
                w_sstatus(sstatus);
                w_sepc(sepc);
                __asm__ volatile("mv a0, %0" : : "r" (ret));
                break;
            }
                //sem_open
            case 0x21: {
                int ret = 0;
                Ksemaphore::sem_t *handle;
                unsigned init;
                __asm__ volatile("mv %0, a1" : "=r" (handle));
                __asm__ volatile("mv %0, a2" : "=r" (init));
                Ksemaphore* s = Ksemaphore::ksem_create(init);
                *handle = s;
                if (!handle) ret = -1;
                else ret = 0;
                __asm__ volatile("mv a0, %0" : : "r" (ret));
                uint64 sepc = r_sepc() + 4;
                w_sepc(sepc);
                break;
            }
                //sem_close
            case 0x22:{
                int ret = 0;
                Ksemaphore::sem_t handle;
                __asm__ volatile("mv %0, a1" : "=r" (handle));
                if(!handle->isOpen()) ret = -1;
                else {
                    handle->ksem_close();
                    ret = 0;
                }
                __asm__ volatile("mv a0, %0" : : "r" (ret));
                uint64 sepc = r_sepc() + 4;
                w_sepc(sepc);
                break;
            }
                //sem_wait
            case 0x23:{
                int ret = 0;
                Ksemaphore::sem_t id;
                __asm__ volatile("mv %0, a1" : "=r" (id));
                if(!id->isOpen()) ret = -1;
                else {
                    ret = id->ksem_wait();
                }
                __asm__ volatile("mv a0, %0" : : "r" (ret));
                uint64 sepc = r_sepc() + 4;
                w_sepc(sepc);
                break;
            }
                //sem_signal
            case 0x24:{
                int ret = 0;
                Ksemaphore::sem_t id;
                __asm__ volatile("mv %0, a1" : "=r" (id));
                if(!id->isOpen()) ret = -1;
                else {
                    id->ksem_signal();
                    ret = 0;
                }
                __asm__ volatile("mv a0, %0" : : "r" (ret));
                uint64 sepc = r_sepc() + 4;
                w_sepc(sepc);
                break;
            }
                //getc
            case 0x41: {
                uint64 sepc = r_sepc() + 4;
                uint64 sstatus = r_sstatus();
                Riscv::w_stvec((uint64) &Riscv::consoleTrap);
                __asm__ volatile("mv a1, %0" : : "r" (__getc()));
                Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
                w_sepc(sepc);
                w_sstatus(sstatus);
                break;
            }
                //putc
            case 0x42:{
                char c;
                __asm__ volatile("mv %0, a1" : "=r" (c));
                uint64 sepc = r_sepc() + 4;
                uint64 sstatus = r_sstatus();
                Riscv::w_stvec((uint64) &Riscv::consoleTrap);
                __putc(c);
                Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
                w_sepc(sepc);
                w_sstatus(sstatus);
                break;
            }
        }
    }
    else if(scause == 0x0000000000000009UL){
        uint64 sepc = r_sepc();
        uint64 sstatus = r_sstatus() & ~(SSTATUS_SPP);

        w_sepc(sepc + 4);
        w_sstatus(sstatus);
    }
    else if(scause == 0x8000000000000001UL){
        mc_sip(SIP_SSIP);
    }
    else if (scause == 0x8000000000000009UL){
        console_handler();
    } else{
        // unexpected trap cause
    }
}

void Riscv::handleConsoleTrap() {
    uint64 scause = r_scause();
    if (scause == 0x8000000000000009UL){
        console_handler();
        mc_sip(SIP_SEIP);
    }
    else if(scause == 0x8000000000000001UL){
        mc_sip(SIP_SSIP);
    }
}