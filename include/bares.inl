#ifndef _BARES_INL_
#define _BARES_INL_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

#include "QueueAr.h"
#include "StackAr.h"
#include "bares.hpp"
#include "errors.hpp"

void BARES::run(int &argc, const char *argv[]) {
    if(argc != 3) {
      std::cout << ">>> Error. please use this notation:" << std::endl;
      std::cout << ">>> ./bares NameOfTheFileToRead NameOfTheFileToOut" << std::endl;
    }

    std::string namefileIn = argv[1];
    std::string namefileOut = argv[2];

    std::ifstream fileIn;
    fileIn.open(namefileIn);

    if(!(fileIn.is_open()) ) {
      std::cout << ">>> Error. Sorry but Wasn't posible open the file " << namefileIn << std::endl;
    }

    std::string finalResult = "";
    std::string line;
    while( !(fileIn.eof()) ) {
      getline(fileIn, line); // get a line of the file and push into the line string variable
      if (line == "") {
        continue;
      }
      try{
        delete queueAux;
        delete queueInfx;
        delete queuePosfx;
        queueAux = new QueueAr<std::string>;
        queueInfx = new QueueAr<std::string>;
        queuePosfx = new QueueAr<std::string>;

        this->expression.clear();
        this->lastOpenedScope.clear();
        this->lastClosedScope.clear();

        pushLine(line);
        std::string auxString = std::to_string(resultAux);
        auxString = auxString + "\n";
        finalResult = finalResult + auxString;
      }
      catch(Error err) {
        std::string auxString = err.printError();
        finalResult = finalResult + auxString;

        delete queueAux;
        delete queueInfx;
        delete queuePosfx;
        queueAux = new QueueAr<std::string>;
        queueInfx = new QueueAr<std::string>;
        queuePosfx = new QueueAr<std::string>;

        this->expression.clear();
        this->lastOpenedScope.clear();
        this->lastClosedScope.clear();
      }
  }
  std::ofstream fileOut;
  fileOut.open(namefileOut);

  fileOut << finalResult;

  fileOut.close();
}

/**
*
*
*/
void BARES::pushLine(std::string line) {
  std::vector<std::string> tokens;
  std::string symb = "";
  int slow = 0;
  bool lastIsNumber = false;
  bool lastParentesClosed = false;
  for(int i = 0; i < line.size(); i++) {
    if(line[i] == ' ') { // don't push space to vector
      continue;
    }
    else if((line[i] == char(BARES::VALID::SUBTRACAO)) && (!lastIsNumber || !lastParentesClosed)) {
      symb = symb + line[i];
      lastParentesClosed= false;
    }
    else if(isdigit(line[i])) {
      symb = symb + line[i];
      lastIsNumber = true;
      lastParentesClosed= false;
    }
    else {
      if(i == 0) { // especial case, the first element is a operand. This "if" don't let the tokens have a void string
        tokens[slow] = line[i];
        slow++;
        continue;
      }
      lastParentesClosed = false;
      if(line[i] == char(BARES::VALID::PARENTESFECHA)) {
        lastParentesClosed = true;
      }
      // if(isValidOperator(line[i]) && (i == 0)) { // especial case, the first element is a operand. This if don't let the tokens have a void string
      //   tokens[slow] = line[i];
      //   slow++;
      //   continue;
      // }
      tokens[slow] = symb;
      symb = "";
      slow++;
      tokens[slow] = line[i];
      slow++;
      lastIsNumber = false;
    }
  }

  lineErrors(tokens);
  resultAux = scopes();
}

bool BARES::isNumber(std::string theNumber) {
  for(int i = 0; i < theNumber.size(); i++) {
    if(isdigit(theNumber[i])) {
      return true;
    }
  }
  return false;
}

/**
*
*
*/
void BARES::lineErrors(std::vector<std::string> tokens) {
  std::string symb;
  int number = 0;
  bool lastIsNumber = false;
  // long unsigned int count = 0;
  int slowClose = 0;
  int slowOpen = 0;
  std::string ehParentAberto = "(";
  std::string ehParentFecha = ")";
  for(long unsigned int i = 0; i < tokens.size(); i++) {
    symb = tokens[i];
    if((isNumber(symb)) && (symb[0] == char(BARES::VALID::SUBTRACAO)) ) { // number is negative
      if(lastIsNumber) {
        throw(Error( i+1, Error::Errors::ExtraneousSymbol));
      }
      std::string aux;
      int e;
      for(e = 0; e < symb.size(); e++) { // get the index just after the last symbol of less
        if(symb[e] != char(BARES::VALID::SUBTRACAO)) {
          break;
        }
      }
      if((e-1) % 2 == 0) { // confirmation if the number is negative
        aux[0] = char(BARES::VALID::SUBTRACAO);
      }
      for(int r = e; r < symb.size(); r++) { //push the number without the less symbol
        aux = aux + symb[r];
      }
      number = stoi(aux);
      if(32767 < number || -32767 > number) { // if the number is out of ranger
        throw(Error( i+1, Error::Errors::NumericConstantOutOfRange));
      }
      this->expression[i] = std::to_string(number);
      number = 0;
      // count++;
      lastIsNumber = true;
    }
    else if(isNumber(symb)) { // number is positive
      if(lastIsNumber) {
        throw(Error( i+1, Error::Errors::ExtraneousSymbol));
      }
      number = stoi(symb);
      if(32767 < number) { // if the number is out of ranger
        throw(Error( i+1, Error::Errors::NumericConstantOutOfRange));
      }
      this->expression[i] = std::to_string(number);
      number = 0;
      // count++;
      lastIsNumber = true;
    }
    else if((i == 0) && (1 == symb.size() && !(isdigit(symb[0])) ) ) { // first element is a unexpected operand
      lastIsNumber = false;
      if(isValidOperand(symb[0])) { // is a valid operand
        throw(Error(i+1, Error::Errors::LostOperator));
      }
      else { // is a invalid operand
        throw(Error(i+1, Error::Errors::InvalidOperand));
      }
      expression[i] = tokens[i];
      // count++;
    }
    else {
      switch(symb[0]) {
        case char(BARES::VALID::ADICAO) :
          if(!lastIsNumber) { // operand of plus without the fisrt operator
            throw(Error(i+1, Error::Errors::LostOperator));
          }
          if(i == tokens.size()-1) {
            throw(Error(i+1, Error::Errors::LostOperator));
          }
          else {
            // if(tokens[i+1] != ehParentAberto || !(isNumber(tokens[i+1])) ) {
            //   throw(Error(i+1, Error::Errors::LostOperator));
            // }
            if(tokens[i-1] != ehParentFecha || !lastIsNumber) {
              throw(Error(i+1, Error::Errors::LostOperator));
            }
          }
          this->expression[i] = tokens[i];
          // count++;
          lastIsNumber = false;
          break;
        case char(BARES::VALID::SUBTRACAO) :
          if(!lastIsNumber) { // operand of less without the fisrt operator
            throw(Error(i+1, Error::Errors::LostOperator));
          }
          if(i == tokens.size()-1) {
            throw(Error(i+1, Error::Errors::LostOperator));
          }
          else {
            // if(tokens[i+1] != ehParentAberto || !(isNumber(tokens[i+1])) ) {
            //   throw(Error(i+1, Error::Errors::LostOperator));
            // }
            if(tokens[i-1] != ehParentFecha || !lastIsNumber) {
              throw(Error(i+1, Error::Errors::LostOperator));
            }
          }
          this->expression[i] = tokens[i];
          // count++;
          lastIsNumber = false;
          break;
        case char(BARES::VALID::MULTIPLICACAO) :
          if(!lastIsNumber) { // operand of multply without the fisrt operator
            throw(Error(i+1, Error::Errors::LostOperator));
          }
          if(i == tokens.size()-1) {
            throw(Error(i+1, Error::Errors::LostOperator));
          }
          else {
            // if(tokens[i+1] != ehParentAberto || !(isNumber(tokens[i+1])) ) {
            //   throw(Error(i+1, Error::Errors::LostOperator));
            // }
            if(tokens[i-1] != ehParentFecha || !lastIsNumber) {
              throw(Error(i+1, Error::Errors::LostOperator));
            }
          }
          this->expression[i] = tokens[i];
          // count++;
          lastIsNumber = false;
          break;
        case char(BARES::VALID::DIVISAO) :
          if(!lastIsNumber) { // operand of multply without the fisrt operator
            throw(Error(i+1, Error::Errors::LostOperator));
          }
          if(i == tokens.size()-1) {
            throw(Error(i+1, Error::Errors::LostOperator));
          }
          else {
            // if(tokens[i+1] != ehParentAberto || !(isNumber(tokens[i+1])) ) {
            //   throw(Error(i+1, Error::Errors::LostOperator));
            // }
            if(tokens[i-1] != ehParentFecha || !lastIsNumber) {
              throw(Error(i+1, Error::Errors::LostOperator));
            }
          }
          this->expression[i] = tokens[i];
          // count++;
          lastIsNumber = false;
          break;
        case char(BARES::VALID::POTENCIA) :
          if(!lastIsNumber) { // operand of multply without the fisrt operator
            throw(Error(i+1, Error::Errors::LostOperator));
          }
          if(i == tokens.size()-1) {
            throw(Error(i+1, Error::Errors::LostOperator));
          }
          else {
            // if(tokens[i+1] != ehParentAberto || !(isNumber(tokens[i+1])) ) {
            //   throw(Error(i+1, Error::Errors::LostOperator));
            // }
            if(tokens[i-1] != ehParentFecha || !lastIsNumber) {
              throw(Error(i+1, Error::Errors::LostOperator));
            }
          }
          this->expression[i] = tokens[i];
          // count++;
          lastIsNumber = false;
          break;
        case char(BARES::VALID::MODULO) :
          if(!lastIsNumber) { // operand of multply without the fisrt operator
            throw(Error(i+1, Error::Errors::LostOperator));
          }
          if(i == tokens.size()-1) {
            throw(Error(i+1, Error::Errors::LostOperator));
          }
          else {
            // if(tokens[i+1] != ehParentAberto || !(isNumber(tokens[i+1])) ) {
            //   throw(Error(i+1, Error::Errors::LostOperator));
            // }
            if(tokens[i-1] != ehParentFecha || !lastIsNumber) {
              throw(Error(i+1, Error::Errors::LostOperator));
            }
          }
          this->expression[i] = tokens[i];
          // count++;
          lastIsNumber = false;
          break;
        case char(BARES::VALID::PARENTESFECHA) :
          if(!lastIsNumber) {
            throw(Error(i+1, Error::Errors::ExtraneousSymbol));
          }
          lastClosedScope[slowClose] = i;
          this->expression[i] = tokens[i];
          lastIsNumber = true;
          continue;
          break;
        case char(BARES::VALID::PARENTESABRE) :
          if(lastIsNumber) {
            throw(Error(i+1, Error::Errors::ExtraneousSymbol));
          }
          lastOpenedScope[slowOpen] = i;
          this->expression[i] = tokens[i];
          lastIsNumber = false;
          continue;
          break;
        default: // invalid operand
          if(symb[0] == '=' || symb[0] == '.') {
            throw(Error(i+1, Error::Errors::InvalidOperand));
          }
          else {
            throw(Error(i+1, Error::Errors::IllFormedExpression));
          }
      }
    }
  }
  if(lastClosedScope.size() < lastOpenedScope.size()) {
    throw(Error(lastOpenedScope[lastOpenedScope.size() - lastClosedScope.size()], Error::Errors::MissingClosing));
  }
  if(lastClosedScope.size() > lastOpenedScope.size()) {
    throw(Error(lastClosedScope[lastClosedScope.size() - lastOpenedScope.size()], Error::Errors::Mismatch));
  }
}

bool BARES::isValidOperand(char symb) {
  switch(symb) {
    case char(BARES::VALID::ADICAO) :
      return true;
      break;
    case char(BARES::VALID::SUBTRACAO) :
      return true;
      break;
    case char(BARES::VALID::MULTIPLICACAO) :
      return true;
      break;
    case char(BARES::VALID::DIVISAO) :
      return true;
      break;
    case char(BARES::VALID::POTENCIA) :
      return true;
      break;
    case char(BARES::VALID::MODULO) :
      return true;
      break;
    default:
      return false;
  }
}

int BARES::scopes() {
  int result = 0;
  unsigned long int lastOpenedScopeSize = lastOpenedScope.size();  // size of vector lastOpenedScope
  unsigned long int lastClosedScopeSize = lastClosedScope.size();  // size of vector lastClosedScope
  for(int i = 0; i < lastClosedScopeSize; i++) {
    for(unsigned long int e = lastOpenedScope[lastOpenedScopeSize-1-i]; e < lastClosedScope[i]; e++) {
      this->queueInfx->enqueue(expression[e+1]);
    }
    InfxToPosfx();
    result = avaliaPosfx();

    delete queueAux;
    delete queueInfx;
    delete queuePosfx;
    queueAux = new QueueAr<std::string>;
    queueInfx = new QueueAr<std::string>;
    queuePosfx = new QueueAr<std::string>;

    // if(lastOpenedScope[i] == -1) {
    //   break;
    // }

    unsigned long int r;
    std::vector<std::string> expressionAux;
    for(r = 0; r < lastOpenedScope[lastOpenedScopeSize-1-i]; r++) {
      expressionAux[r] = expression[r];
    }

    unsigned long int rA = r+1;
    expressionAux[r+1] = result;

    for(r = i; r < expression.size(); r++) {
      expressionAux[rA] = expression[r];
      rA++;
    }

    this->expression.clear();
    this->lastOpenedScope.clear();
    this->lastClosedScope.clear();

    for(r = 0; r < expressionAux.size(); r++) {
      expression[r] = expressionAux[r];
    }

    // int aux = lastOpenedScope[lastOpenedScopeSize-1-i];
    // expression[aux] = result;
    // for(int r = lastClosedScope[i]+1; r < lastOpenedScope[lastOpenedScopeSize-1-i]+1 && lastClosedScope[i]+1 != tokens.size(); i--) {
    //   expression[aux] = expression[r];
    //   aux++;
    // }
  }
  // what is not into a scope
  for(int e = 0; e < expression.size(); e++) {
    this->queueInfx->enqueue(expression[e]);
  }
  InfxToPosfx();
  result = avaliaPosfx();

  delete queueAux;
  delete queueInfx;
  delete queuePosfx;
  queueAux = new QueueAr<std::string>;
  queueInfx = new QueueAr<std::string>;
  queuePosfx = new QueueAr<std::string>;

  this->expression.clear();
  this->lastOpenedScope.clear();
  this->lastClosedScope.clear();

  return result;
}

bool prcd(std::string top, std::string symb) {
  switch(top[0]) {
    case char(BARES::VALID::ADICAO):
      top = BARES::PRECEDENCE::_ADICAO;
      break;
    case char(BARES::VALID::SUBTRACAO):
      top = BARES::PRECEDENCE::_SUBTRACAO;
      break;
    case char(BARES::VALID::MULTIPLICACAO):
      top = BARES::PRECEDENCE::_MULTIPLICACAO;
      break;
    case char(BARES::VALID::DIVISAO):
      top = BARES::PRECEDENCE::_DIVISAO;
      break;
    case char(BARES::VALID::MODULO):
      top = BARES::PRECEDENCE::_MODULO;
      break;
    case char(BARES::VALID::POTENCIA):
      top = BARES::PRECEDENCE::_POTENCIA;
      break;
  }
  switch(symb[0]) {
    case char(BARES::VALID::ADICAO):
      symb = BARES::PRECEDENCE::_ADICAO;
      break;
    case char(BARES::VALID::SUBTRACAO):
      symb = BARES::PRECEDENCE::_SUBTRACAO;
      break;
    case char(BARES::VALID::MULTIPLICACAO):
      symb = BARES::PRECEDENCE::_MULTIPLICACAO;
      break;
    case char(BARES::VALID::DIVISAO):
      symb = BARES::PRECEDENCE::_DIVISAO;
      break;
    case char(BARES::VALID::MODULO):
      symb = BARES::PRECEDENCE::_MODULO;
      break;
    case char(BARES::VALID::POTENCIA):
      symb = BARES::PRECEDENCE::_POTENCIA;
      break;
  }
  return (top >= symb);
}

void BARES::InfxToPosfx() {
  std::string symb;
  StackAr<std::string> stack;
  while(this->queueInfx->isEmpty()) {
    symb = this->queueInfx->dequeue();
    if(isNumber(symb)) {
      this->queuePosfx->enqueue(symb);
    }
    else {
      while(!(stack.isEmpty()) && prcd(stack.top(), symb)) {
        if(prcd(stack.top(), symb)) {
          this->queuePosfx->enqueue(stack.pop());
        }
      }
      stack.push(symb);
    }
  }
  while( !(stack.isEmpty()) ) {
    this->queuePosfx->enqueue(stack.pop());
  }
}

int BARES::avaliaPosfx(){
  StackAr<int> stack;
  long int opnd1 = 0, opnd2 = 0, result = 0, i = 0;
  std::string symb;

  while( !(queuePosfx->isEmpty()) ) {
    symb = queuePosfx->dequeue();
    if(isNumber(symb)) {
      stack.push(std::stoi(symb));
    }
    else {
      opnd2 = stack.pop();
      opnd1 = stack.pop();
      switch(symb[0]) {
        case char(BARES::VALID::ADICAO):
          result = opnd1 + opnd2;
          break;
        case char(BARES::VALID::SUBTRACAO):
          result = opnd1 - opnd2;
          break;
        case char(BARES::VALID::MULTIPLICACAO):
          result = opnd1 * opnd2;
          break;
        case char(BARES::VALID::DIVISAO):
          if(opnd2 == 0) {
            stack.makeEmpty();
            throw(Error(i+1, Error::Errors::DivisionByZero));
          } else {
            result = opnd1 / opnd2;
          }
          break;
        case char(BARES::VALID::MODULO):
          result = opnd1 % opnd2;
          break;
        case char(BARES::VALID::POTENCIA):
          result = pow(opnd1, opnd2);
          break;
      }
    }
    stack.push(result);
    i++;
  }
  result = stack.pop();
  return result;
}

#endif
