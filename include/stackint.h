#ifndef _StackInt_h
#define _StackInt_h

#include <iostream>
#include <vector>

class StackInt{
  public:
    StackInt(int tamanho = 50) {
      theQueue = new int[tamanho];
      capacity = tamanho;
      theTop = 0;
    }
    virtual ~StackInt() {
      delete [] theQueue;
    }

    void push(const int & _x);
    int pop();
    int top();

    bool isEmpty();
    void makeEmpty();

    void printQueue(); // print in the console the stack elements

  private:
    using Data = int;
    int capacity = 50;
    int *theQueue;
    int theTop;
};

#include "stackint.cpp"

#endif
