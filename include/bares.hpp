#ifndef _BARES_H_
#define _BARES_H_

#include <iostream>
#include <string>

#include "erros.hpp"

class BARES {
  public:
    BARES() {
      queueInfx = new QueueAr<std::string>;
      queuePosfx = new QueueAr<std::string>;
      queueAux = new QueueAr<std::string>;
    }
    ~BARES() {
      delete this->queueInfx;
      delete this->queuePosfx;
      delete this->queueAux;
    }
    void InfxToPosfx();
    int avaliaPosfx();
    void run(int , const char);
    void pushLine(std::string);
    bool scopes(vector<std::string>)

    enum VALID {
      ADICAO = int('+'),
      SUBTRACAO = int('-'),
      MULTIPLICACAO = int('*'),
      DIVISAO = int('/'),
      POTENCIA = int('^'),
      MODULO = int('%'),
      PARENTESABRE = int('('),
      PARENTESFECHA = int(')')
    };

    enum PRECEDENCE {
      ADICAO = int('+'),
      SUBTRACAO = int('-'),
      MULTIPLICACAO = int('*'),
      DIVISAO = int('/'),
      POTENCIA = int('^'),
      MODULO = int('%'),
    };

  private:
    QueueAr<std::string> *queueInfx;
    QueueAr<std::string> *queuePosfx;
    QueueAr<std::string> *queueAux;
};

#include "bares.inl"

#endif
