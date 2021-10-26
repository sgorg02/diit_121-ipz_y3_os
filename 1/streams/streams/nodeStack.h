#ifndef __NodeStack__
#define __NodeStack_H__

#include <windows.h>
#include <process.h>
#include <iostream>
#include <conio.h>

template <typename T>
struct NodeStack {
	T item;
	NodeStack<T>* next;
};

template <typename T>
class StackList {
private:
	NodeStack<T>* pTop; // pointer to the top of the stack

public:
	StackList() {
		pTop = nullptr;
	}

    void Push(T item) {
        NodeStack<T>* p;

        // 1. Form element
        try {
            p = new NodeStack<T>; // trying to allocate memory
        }
        catch (const std::bad_alloc& e) {
            // if memory is not allocated, then exit
            std::cout << e.what() << std::endl;
            return;
        }

        p->item = item;
        p->next = pTop; // p points to 1st element

        // 2. Redirect pTop to p
        pTop = p;
    }

    void Empty() {
        NodeStack<T>* p;
        NodeStack<T>* p2;

        p = pTop;

        while (p != nullptr) {
            p2 = p; // make a copy of p
            p = p->next; // go to the next element of the stack
            delete p2; // delete memory allocated for the previous item
        }
        pTop = nullptr; // fix the top of the stack
    }

    void Print() {

        if (pTop == nullptr)
            std::cout << "stack is empty." << std::endl;
        else {
            NodeStack<T>* p;
            p = pTop;

            while (p != nullptr) {
                std::cout << p->item << "\t";
                p = p->next;
            }
            std::cout << std::endl;
        }
    }

    ~StackList() {
        Empty();
    }
};

#endif