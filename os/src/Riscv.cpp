#include "../h/Riscv.hpp"
#include "../h/TCB.hpp"
#include "../h/_sem.hpp"
#include "../h/sleepingTCB.hpp"
#include "../h/_console.hpp"



int ip =0;


void Riscv::popSppSpie(){
    Riscv::mc_sstatus(Riscv::SSTATUS_SPP);
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}



void Riscv::handleEEcallTrap() {
    uint64 scause = r_scause();
    uint64 a1;
    __asm__ volatile( "mv %0, a1" : "=r"(a1));
    uint64 a2;
    __asm__ volatile( "mv %0, a2" : "=r"(a2));
    uint64 a3;
    __asm__ volatile( "mv %0, a3" : "=r"(a3));
    uint64 a4;
    __asm__ volatile( "mv %0, a4" : "=r"(a4));
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        //enivroment call from S-mode
        uint64 sepc = r_sepc() + 4;
        uint64 sstatus = r_sstatus();
        uint64 num;
        __asm__ volatile( "mv %0, a0" : "=r"(num));

        if (num == 0x01) {  //mem_alloc
            void *rv = MemoryAllocator::getInstance().mem_alloc((size_t) a1);
            __asm__ volatile("mv a0, %0"::"r"(rv));

        } else if (num == 0x02) {  //mem_free
            int rv = MemoryAllocator::getInstance().mem_free((void *) a1);
            __asm__ volatile("mv a0, %0"::"r"(rv));

        }else if (num == 0x11) {  //thread_create
            char *stack = (char *) MemoryAllocator::getInstance().mem_alloc(
                    ((DEFAULT_STACK_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE));
            int rv = TCB::createThread((TCB **) a1, (TCB::Body) a2, (void *) a3, (void *) stack);
            __asm__ volatile("mv a0, %0"::"r"(rv));

        } else if (num == 0x12) {//thread_exit
            TCB::timeSliceCounter = 0;
            TCB::running->setFinished(true);
            if (TCB::running->semaphore != nullptr) {
                _sem::sem_close(TCB::running->semaphore);
            }
            TCB::dispatch();

        } else if (num == 0x13) {//thread_dispatch
            TCB::timeSliceCounter=0;
            TCB::dispatch();

        } else if (num == 0x14) {//thread_join
            TCB *handle = (TCB *) a1;
            if (handle != nullptr) {
                _sem::sem_open(&handle->semaphore, 0);
                _sem::sem_wait(handle->semaphore);
            }

        } else if (num == 0x21) {//sem_open
            int rv = _sem::sem_open((_sem **) a1, (int) a2);
            __asm__ volatile("mv a0, %0"::"r"(rv));

        } else if (num == 0x22) {//sem_close
            int rv = _sem::sem_close((_sem *) a1);
            __asm__ volatile("mv a0, %0"::"r"(rv));

        } else if (num == 0x23) {//sem_wait
            int rv = _sem::sem_wait((_sem *) a1);
             //if(TCB::running->blocked){TCB::dispatch();}
            __asm__ volatile("mv a0, %0"::"r"(rv));

        } else if (num == 0x24) {//sem_signal
            int rv = _sem::sem_sgnal((_sem *) a1);
            __asm__ volatile("mv a0, %0"::"r"(rv));

        } else if (num == 0x31) {//time_sleep
            int rv = -1;
            if(a1 > 0){
                rv = sleepingTCB::put((uint64) a1);
            }
            __asm__ volatile("mv a0, %0"::"r"(rv));
            TCB::timeSliceCounter=0;
            TCB::dispatch();

        } else if (num == 0x41) {//getc
            //char c = __getc();
            while(_console::getConsole().inputBuff->getCount() == 0){
                TCB::timeSliceCounter = 0;
                TCB::dispatch();
            }
            char c= _console::getConsole().inputBuff->getc();
            __asm__ volatile("mv a0, %0"::"r"(c));

        } else if (num == 0x42) {//putc
            //__putc((char) a1);
            while(_console::getConsole().outputBuff->getCount() == 2048){
                TCB::timeSliceCounter = 0;
                TCB::dispatch();
            }
            _console::getConsole().outputBuff->putc((char)a1);

        } else {
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
        }
        w_sstatus(sstatus);
        w_sepc(sepc);
        mc_sip(SIP_SSIP);
    } else {
        if (ip == 0 && scause==2) {
            /*putc('G');
            putc('r');
            putc('e');
            putc('s');
            putc('k');
            putc('a');
            putc('!');
            putc('\n');
            putc('S');
            putc('c');
            putc('a');
            putc('u');
            putc('s');
            putc('e');
            putc(':');
            char buf[16];
            int i=0;
            char digits[] = "0123456789ABCDEF";

            do{
                buf[i++] = digits[scause % 10];
            }while((scause /= 10) != 0);
            while(--i >= 0)
                putc(buf[i]);*/
            _console::printString("Greska!\nScause:");
            _console::printInt(scause);
        }

        ip++;
    }
}

void Riscv::handleTimerTrap() {
    sleepingTCB::wakeUp();
    //timer
    mc_sip(SIP_SSIP);

    TCB::timeSliceCounter++;
    if(TCB::timeSliceCounter >= TCB::running->getTimeSlice()){
        uint64 sepc=r_sepc();
        uint64 sstatus=r_sstatus();
        TCB::timeSliceCounter=0;
        TCB::dispatch();
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
}

void Riscv::handleConsoleTrap() {
    //console
    //console_handler();
    int irq = plic_claim();
    while (*((char*)(CONSOLE_STATUS)) & CONSOLE_RX_STATUS_BIT) {

        char c = (*(char *) CONSOLE_RX_DATA);
        _console::getConsole().inputBuff->putc(c);
    }
    plic_complete(irq);
}



