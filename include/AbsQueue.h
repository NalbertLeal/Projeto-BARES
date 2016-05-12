#ifndef _ABSQUEUE_H_
#define _ABSQUEUE_H_

#include <iostream>

/* Class interface file (não pode ser instanciada!). */
template <class Object>
class AbsQueue {
  public:
    AbsQueue() { /* Empty */ } // Default construct
    virtual ~AbsQueue() { /* Empty */ }  // default destructor

    //basic members
    virtual void enqueue(const Object & x) = 0;
    virtual Object dequeue() = 0;
    virtual Object getFront() const = 0;

    virtual bool isEmpty() const = 0;
    virtual void makeEmpty() = 0;

  private:
    // Disable copy constructor
    AbsQueue(const AbsQueue & ) { /* Empty */ }
};

#endif
