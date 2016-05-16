#ifndef _STACKAR_INL_
#define _STACKAR_INL_

#include <iostream>
#include <cstring>
#include <string>

#include "stackar.h"

template < class Data >
void StackAr< Data >::push(const Data & _x) {
  if(theTop == capacity) {
    std::cout << ">>> The stack is full. Folding the capacity ...\n";
    capacity = 2 * capacity;
    Data *newStack = new Data[capacity];

    // Add elements of theStack to newStack
    for(int i = 0; i < theTop; i++) {
      newStack[i] = theStack[i];
    }

    delete [] theStack;
    theStack = new Data[capacity];

    for(int i = 0; i < theTop; i++) {
      theStack[i] = newStack[i];
    }

    delete [] newStack;
  }
  theTop++;
  theStack[theTop] = _x;

}

template < class Data >
Data StackAr< Data >::pop() {
  if(!isEmpty()) {
    Data elementTop = theStack[theTop];
    theTop--;
    return elementTop;
  }
}

template < class Data >
Data StackAr< Data >::top() const {
  return theStack[theTop];
}

template < class Data >
bool StackAr< Data >::isEmpty() const {
  return theTop <= 0;
}

template < class Data >
void StackAr< Data >::makeEmpty() {
  theTop = 0;
  std::cout << ">>> Stack was cleaned.\n";
}

template < class Data >
void StackAr< Data >::printStack() {
  std::cout << ">>> The elements that still in the stack are: \n [ ";
  for(int i = 0; i < theTop; i++) {
    std::cout << theStack[i] << " " ;
  }
  std::cout << "] \n";
}

#endif
