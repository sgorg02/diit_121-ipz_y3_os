#include <process.h>
#include <iostream>
#include <conio.h>
#include <random>
#include "nodeStack.h"

// Stack based on a dynamic list
CRITICAL_SECTION critical_section;
StackList<int> SL;

void fullStack(void *);
void print(void *);
void empty(void *);

int main() {
    InitializeCriticalSection(&critical_section); {

        HANDLE FullStack = (HANDLE)_beginthread(fullStack, 1024, NULL);
        HANDLE Print = (HANDLE)_beginthread(print, 1024, NULL);
        HANDLE Empty = (HANDLE)_beginthread(empty, 1024, NULL);

        Sleep(5000);
    }
    DeleteCriticalSection(&critical_section);
}

void fullStack(void *) {
    EnterCriticalSection(&critical_section);
    std::cout << "Full Stack!" << std::endl;
    std::random_device generator;
    std::uniform_int_distribution<int> distribution(0, 100);
    for (int i = 0; i < 10; i++) {
        int number = distribution(generator);
            SL.Push(number);
    }
    std::cout << std::endl;

    LeaveCriticalSection(&critical_section);
    _endthread();
}

void print(void *) {
    EnterCriticalSection(&critical_section);

    std::cout << "Print Stack!" << std::endl;
    SL.Print();
    std::cout << std::endl;

    LeaveCriticalSection(&critical_section);
    _endthread();
}

void empty(void *) {
    EnterCriticalSection(&critical_section);

    std::cout << "Empty Stack!" << std::endl;
    SL.Empty();

    LeaveCriticalSection(&critical_section);
    _endthread();
}
