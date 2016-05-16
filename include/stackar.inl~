#ifndef _STACKAR_INL_
#define _STACKAR_INL_

#include <iostream>
#include <cstring>

#include "stackar.h"

template < class Data >
void StackAr< Data >::push(const Data & _x) {
  if(theTop == capacity) {
    std::cout << ">>> The stack is full. Folding the capacity ...\n";
    capacity = 2 * capacity;
    Data *newStack = new int[capacity];

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
  theStack[theTop] = _x;
  theTop++;

}

template < class Data >
Data StackAr< Data >::pop() {
  if(!isEmpty()) {
    Data elementTop = theStack[theTop-1];
    theTop--;
    return elementTop;
  }
  else {
    std::cout << ">>> The stack is empty, so it's impossible to return the correct element.\n";
    return 0;
  }
}

template < class Data >
Data StackAr< Data >::top() const {
  return theTop;
}

template < class Data >
bool StackAr< Data >::isEmpty() const {
  if( (theTop == 0) && (theStack[theTop] == 0) ) {
    return true;
  }
  return false;
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
