#ifndef _BARES_HPP_
#define _BARES_HPP_

#include <iostream>
#include <vector>
#include <string>

#include "QueueAr.h"
#include "StackAr.h"

class BARES {
  public:
    BARES () {

    }
    ~BARES () {}
    void runFile(std::string, std::string);
    bool isNumber(std::string);
    // void errosLinha(QueueAr<std::string>);

  private:

};

#include "bares.inl"

#endif
