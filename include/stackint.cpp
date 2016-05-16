#ifndef _stackint_CPP
#define _stackint_CPP

#include <iostream>
#include "stackint.h"

void StackInt::push(const int & _x) {
  if(theTop == capacity) {
    std::cout << "The queue is full. Folding the capacity ...\n";
    capacity = 2 * capacity;
    int *newQueue = new int[capacity];

    for(int i = 0; i < theTop; i++) {
      newQueue[i] = theQueue[i];
    }

    theQueue = newQueue;
  }

  theQueue[theTop] = _x;
  theTop++;

}

int StackInt::pop() {
  if(!isEmpty()) {
    int elementTop = theQueue[theTop-1];
    theTop--;
    return elementTop;
  }
  else {
    std::cout << "The queue is empty, so it's impossible to return the correct element.\n";
    return 0;
  }

}

int StackInt::top() {
  return theTop;
}

bool StackInt::isEmpty() {
  if( (theTop == 0) && (theQueue[theTop] == 0) ) {
    return true;
  }
  return false;
}

void StackInt::makeEmpty() {
  theTop = 0;
  std::cout << "Queue was cleaned.\n";
}

void StackInt::printQueue() {
  for(int i = 0; i < theTop; i++) {
    std::cout << theQueue[i] << std::endl ;
  }
}

#endif
