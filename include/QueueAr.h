#ifndef _QUEUEAR_H_
#define _QUEUEAR_H_

#include <iostream>

#include "AbsQueue.h"

// #include "exeptionQueue.h"

/* Class interface file (não pode ser instanciada!). */
template < class Data >
class QueueAr : public AbsQueue< Data > {
  public:
    QueueAr(int size = 10) : capacity(size) {
      theFront = -1;
      theLast = -1;
      theQueue = new Data[size];
    }
    virtual ~QueueAr() {
      delete [] theQueue;
    }

    //basic members
    void enqueue(const Data & _x);
    Data dequeue();
    Data getFront() const;

    bool isEmpty() const;
    void makeEmpty();

    void printQueue();

  private:

    Data *theQueue;
    int theFront;
    int theLast;
    int capacity;
};

#include "QueueAr.inl"

#endif
