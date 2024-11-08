#include "../h/syscall_cpp.hpp"
#include "../h/Scheduler.hpp"
#include "../h/_sem.hpp"
#include "../h/syscall_c.hpp"

void* operator new (size_t s){
    return mem_alloc(s);
}

void operator delete (void*adr){
    mem_free(adr);
}

Thread::Thread(void (*body)(void *), void *arg) {
    myHandle=nullptr;
    this->body=body;
    this->arg=arg;
}

Thread::Thread() {
    myHandle= nullptr;
    arg=this;
    body=nullptr;
}

void Thread::runner(void *t) {
    Thread* thrd=(Thread*)t;
    thrd->run();
}


Thread::~Thread(){
    delete myHandle;
}

int Thread::start(){
    int ret;
    if(this->body){
        ret=thread_create(&myHandle,body, arg);
    }else{
        ret=thread_create(&myHandle,&Thread::runner,(void*)this);
    }
    return ret;
}

void Thread::dispatch(){
    thread_dispatch();
}

void Thread::join() {
    return thread_join(this->myHandle);
}

int Thread::sleep(time_t t) {
   return time_sleep(t);
}


Semaphore::Semaphore(unsigned int init) {

    int ret =sem_open(&myHandle,init);
    if(ret!=0){
        myHandle=0;
    }
}

Semaphore::~Semaphore(){
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}


