#ifndef _BARES_H_
#define _BARES_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "QueueAr.h"
#include "StackAr.h"
#include "errors.hpp"

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
    void run(int &argc, const char *argv[]);
    void pushLine(std::string );
    void lineErrors(std::vector<std::string> );
    int scopes(std::vector<std::string> );
    bool isValidOperand(char);
    bool isNumber(std::string);

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
      _ADICAO = 3,
      _SUBTRACAO = 3,
      _MULTIPLICACAO = 2,
      _DIVISAO = 2,
      _POTENCIA = 2,
      _MODULO = 1,
    };

  private:
    long int resultAux;

    QueueAr<std::string> *queueInfx;
    QueueAr<std::string> *queuePosfx;
    QueueAr<std::string> *queueAux;

    std::vector<std::string> expression;
    std::vector<int> lastOpenedScope; // stores index (of tokens) of the position of every opened parentsis
    std::vector<int> lastClosedScope; // stores index (of tokens) of the position of every closed parentsis
};

#include "bares.inl"

#endif
