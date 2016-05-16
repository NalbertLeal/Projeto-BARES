#ifndef _BARES_HPP_
#define _BARES_HPP_

#include <iostream>
#include <vector>
#include <string>

#include "QueueAr.h"
#include "stackar.h"

class BARES {
  public:
    BARES () {
       queueInfx = new QueueAr<std::string>;
       queuePosfx = new QueueAr<std::string>;
    }
    ~BARES () {
      delete queueInfx;
      delete queuePosfx;
    }
    void runFile(std::string, std::string);
    bool isNumber(std::string);
    // void errosLinha(QueueAr<std::string>);
    void InfxToPosfx();
    int avaliaPosfx();

    struct Opened{
      long int index;
      int position;
    };

    struct Closed{
      long int index;
      int position;
    };

    enum PRECEDENCE {
      _ADICAO = 3,
      _SUBTRACAO = 3,
      _MULTIPLICACAO = 2,
      _DIVISAO = 2,
      _POTENCIA = 2,
      _MODULO = 1,
    };

  private:
    QueueAr<std::string> * queueInfx;
    QueueAr<std::string> * queuePosfx;
};

#include "bares.inl"

#endif
