#include "../h/syscall_c.hpp"
#include "../h/riscv.hpp"
#include "../h/PCB.hpp"
#include "../h/Scheduler.hpp"

void *mem_alloc(size_t size) {
    Arg a;
    a.a0 = 0x01;
    a.a1 = size;
    sysCall(&a);
    void* ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int mem_free(void * addr) {
    Arg a;
    a.a0 = 0x02;
    a.a1 = (uint64)addr;
    sysCall(&a);
    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

void sysCall(void* arg) {
    Arg* a = (Arg*)arg;
    uint64 a0 = a->a0;
    uint64 a1 = a->a1;
    uint64 a2 = a->a2;
    uint64 a3 = a->a3;
    uint64 a4 = a->a4;
    __asm__ volatile("mv a0, %0" : : "r" (a0));
    __asm__ volatile("mv a1, %0" : : "r" (a1));
    __asm__ volatile("mv a2, %0" : : "r" (a2));
    __asm__ volatile("mv a3, %0" : : "r" (a3));
    __asm__ volatile("mv a4, %0" : : "r" (a4));
    __asm__ volatile("ecall");
}

void thread_dispatch() {
    Arg a;
    a.a0 = 0x13;
    sysCall(&a);
}

int thread_create(PCB::thread_t* handle, PCB::Body body, void *arg) {
    uint64* stack_space = (uint64*) mem_alloc(sizeof(uint64) * DEFAULT_STACK_SIZE);
    Arg a;
    a.a0 = 0x11;
    a.a1 = (uint64)handle;
    a.a2 = (uint64)body;
    a.a3 = (uint64)arg;
    a.a4 = (uint64)stack_space;
    sysCall(&a);
    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int thread_exit() {
    Arg a;
    a.a0 = 0x12;
    sysCall(&a);
    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_open(Ksemaphore::sem_t *handle, unsigned int init) {
    Arg a;
    a.a0 = 0x21;
    a.a1 = (uint64)handle;
    a.a2 = (uint64) init;
    sysCall(&a);
    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_close(Ksemaphore::sem_t handle) {
    Arg a;
    a.a0 = 0x22;
    a.a1 = (uint64)handle;
    sysCall(&a);
    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_wait(Ksemaphore::sem_t id) {
    Arg a;
    a.a0 = 0x23;
    a.a1 = (uint64)id;
    sysCall(&a);
    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_signal(Ksemaphore::sem_t id) {
    Arg a;
    a.a0 = 0x24;
    a.a1 = (uint64)id;
    sysCall(&a);
    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

char getc() {
    Arg a;
    a.a0 = 0x41;
    sysCall(&a);
    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

void putc(char c) {
    Arg a;
    a.a0 = 0x42;
    a.a1 = (uint64)c;
    sysCall(&a);
}
