#include <iostream>

/* Class interface file (não pode ser instanciada!). */
template <class Object>
class AbsStack {
  public:
    AbsStack() { /* Empty */ } // Default construct
    virtual ~AbsStack() { /* Empty */ }  // default destructor

    //basic members
    virtual void push(const Object & x) = 0;
    virtual Object pop() = 0;
    virtual Object top() const = 0;

    virtual bool isEmpty() const = 0;
    virtual void makeEmpty() = 0;

  private:
    // Disable copy constructor
    AbsStack(const AbsStack & ) { /* Empty */ }
};
