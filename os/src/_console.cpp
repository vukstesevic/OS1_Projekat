//
// Created by os on 1/8/24.
//

#include "../h/_console.hpp"


Cbuffer *_console::getInput() {
    return inputBuff;
}

Cbuffer *_console::getOutput() {
    return outputBuff;
}

_console& _console::getConsole() {
    static _console inst;
    return inst;
}

void _console::outputBodyWrapper(void * args) {
    while(true){
        while((*((char*)(CONSOLE_STATUS)) & CONSOLE_TX_STATUS_BIT)){
            char c = _console::getConsole().outputBuff->getc();
            *((char*)CONSOLE_TX_DATA) = c;
        }
    }
}

void _console::setInput(Cbuffer* b) {
    inputBuff=b;
}

void _console::setOutput(Cbuffer *b) {
    outputBuff=b;
}

void _console::printString(const char *string) {
    uint64 sstatus = Riscv::r_sstatus();
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
    if(TCB::running->getMyId()>4) {
        while (*string != '\0') {
            //outputBuff->putc(*string);
            putc(*string);
            string++;
        }
    }
    Riscv::ms_sstatus(sstatus & Riscv::SSTATUS_SIE ? Riscv::SSTATUS_SIE : 0);
}

void _console::printInt(int xx) {
    uint64 sstatus = Riscv::r_sstatus();
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
    if(TCB::running->getMyId()>4) {
        char digits[] = "0123456789";
        char buf[16];
        int i, neg;
        uint x;

        neg = 0;
        if (xx < 0) {
            neg = 1;
            x = -xx;
        } else {
            x = xx;
        }

        i = 0;
        do {
            buf[i++] = digits[x % 10];
        } while ((x /= 10) != 0);
        if (neg)
            buf[i++] = '-';

        while (--i >= 0)
            putc(buf[i]);
    }
    Riscv::ms_sstatus(sstatus & Riscv::SSTATUS_SIE ? Riscv::SSTATUS_SIE : 0);
}






