//
// Created by os on 6/26/22.
//

#ifndef PROJECT_BASEV1_1_SYSCALL_CPP_HPP
#define PROJECT_BASEV1_1_SYSCALL_CPP_HPP

#include "../h/syscall_c.hpp"

class Thread {
public:
    Thread (void (*body)(void*), void* arg){
        thread_create(&myHandle, body, arg);
    }
    virtual ~Thread (){
        myHandle=0;
    }
    int start (){
        if(!myHandle) {
            thread_create(&myHandle, &wrapper, this);
            return 0;
        }
        else return -1;
    }
    static void dispatch (){
        thread_dispatch();
    }
    static int sleep (time_t);
protected:
    Thread (){
        myHandle=0;
    }
    virtual void run () {}
private:
    static void wrapper(void* arg){
        Thread* t = (Thread*)arg;
        t->run();
    }
    thread_t myHandle;
};

class Semaphore {
public:
    Semaphore (unsigned init = 1){
        sem_open(&myHandle, init);
    }
    virtual ~Semaphore (){
        sem_close(myHandle);
    }
    int wait (){
        return sem_wait(myHandle);
    }
    int signal (){
        return sem_signal(myHandle);
    }
private:
    sem_t myHandle;
};

class PeriodicThread : public Thread {
protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}
};

class Console {
public:
    static char getc (){
        return ::getc();
    }
    static void putc (char c){
        ::putc(c);
    }
};

#endif //PROJECT_BASEV1_1_SYSCALL_CPP_HPP
