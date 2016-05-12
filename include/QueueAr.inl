#ifndef _QUEUEAR_INL_
#define _QUEUEAR_INL_

#include <iostream>

#include "QueueAr.h"

template < class Data >
void QueueAr< Data >::enqueue(const Data & _x) {
  int e = 0;
  if( ((theFront == -1) and (theLast == -1)) ) {
    theFront++;
  }
  else if(theLast < theFront) {
    std::cout << "theLast = " << theLast << " ,theFront = " << theFront << std::endl;
    theFront = 0;
    theLast = 0;
    theQueue[theLast + 1] == _x;
  }
  else if(theLast == (capacity - 1) ) {
    if(theFront == 0) {
      std::cout << ">>> The queue is full. Folding the size ... \n";
      capacity = 2 * capacity;
      Data *newQueue = new Data[capacity];

      // add elements of theQueue to newQueue
      for(int i = theFront; i <= theLast; i++) {
        newQueue[e] = theQueue[i];
        e++;
      }

      delete [] theQueue;
      theQueue = new Data[capacity];

      for(int i = 0; i < capacity; i++) {
        theQueue[i] = newQueue[i];
      }

      delete [] newQueue;
    }
    else {
      for(int i = 0; i <= (theLast - theFront) ; i++) {
        theQueue[i] = theQueue[theFront];
        e++;
      }
    }
  }

  theQueue[theLast + 1] = _x;
  theLast++;
}

template < class Data >
Data QueueAr< Data >::dequeue() {
  if(!isEmpty()) {
    Data elementTop = theQueue[theFront];
    theFront++;
    return elementTop;
  }
  else {
    std::cout << ">>> The queue is empty, so it's impossible to return the correct element.\n";
    return 0;
  }
}

template < class Data >
Data QueueAr< Data >::getFront() const {
  return theFront;
}

template < class Data >
bool QueueAr< Data >::isEmpty() const {
  if( (theLast < theFront) or ((theFront == -1) and (theLast == 0)) ) {
    return true;
  }
  return false;
}

template < class Data >
void QueueAr< Data >::makeEmpty() {
  theFront = -1;
  theLast = -1;
  std::cout << ">>> Queue was cleaned.\n";
}

template < class Data >
void QueueAr< Data >::printQueue() {
  std::cout << ">>> The elements that still in the queue are: \n [ ";
  for(int i = theFront; i <= theLast; i++) {
    if( (theFront == -1 and theLast == -1) or (theLast < theFront) ) {
      break;
    }
    std::cout << theQueue[i] << " " ;
  }
  std::cout << "] \n";
}

#endif
