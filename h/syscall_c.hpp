//
// Created by os on 6/21/22.
//

#ifndef PROJECT_BASEV1_1_SYSCALL_C_HPP
#define PROJECT_BASEV1_1_SYSCALL_C_HPP

#include "../lib/hw.h"
#include "../h/PCB.hpp"
#include "../h/ksemaphore.hpp"
#include "../lib/console.h"

typedef struct Arg{
    uint64 a0;
    uint64 a1;
    uint64 a2;
    uint64 a3;
    uint64 a4;
}Arg;

const int EOF = -1;

typedef Ksemaphore* sem_t;
typedef PCB* thread_t;

void* mem_alloc(size_t size);

int mem_free(void*);

int thread_create(PCB::thread_t* handle, PCB::Body body, void* arg);

void thread_dispatch();

int thread_exit();

int sem_open(Ksemaphore::sem_t* handle, unsigned init);

int sem_close(Ksemaphore::sem_t handle);

int sem_wait(Ksemaphore::sem_t id);

int sem_signal(Ksemaphore::sem_t id);

char getc();

void putc(char);

void sysCall(void*);

#endif //PROJECT_BASEV1_1_SYSCALL_C_HPP
