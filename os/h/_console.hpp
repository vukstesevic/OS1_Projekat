//
// Created by os on 1/8/24.
//

#ifndef PROJECT_BASE_V1_1__CONSOLE_HPP
#define PROJECT_BASE_V1_1__CONSOLE_HPP
#include "../h/Cbuffer.hpp"


class _console{

    friend class _sem;

public:
    Cbuffer* inputBuff;
    Cbuffer* outputBuff;
    static _console& getConsole();
    Cbuffer* getInput();
    Cbuffer* getOutput();
    void setInput(Cbuffer* b);
    void setOutput(Cbuffer* b);
    static void outputBodyWrapper(void*);
    static void printString(char const *string);
    static void printInt(int xx);
};


#endif //PROJECT_BASE_V1_1__CONSOLE_HPP
