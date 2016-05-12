#ifndef _BARES_INL_
#define _BARES_INL_

#include <iostream>
#include <string>
#include <vector>

#include "bares.hpp"
#include "errors.hpp"

void BARES::run(int argc, const char argv[]) {
    if(argc != 2) {
      std::cout << ">>> Error. please use this notation:" << estd::endl;
      std::cout << ">>> ./bares NameOfTheFileToRead NameOfTheFileToOut" << std::endl;
    }

    std::string namefileIn = argv[1];
    std::string namefileOut = argv[2];

    std::ifstream fileIn;
    fileIn.open(namefileIn);

    std::ofstream fileOut;
    fileOut.open(namefileOut);

    if(!(fileIn.is_open()) ) {
      std::cout << ">>> Error. Sorry but Wasn't posible open the file " << namefileIn << std::endl;
    }

    while( !(fileIn.eof()) ) {
      getline(fileIn, line); // get a line of the file and push into the line string variable
      if (line == "") {
        continue;
      }
      try{
        pushLine(std::string line, fileOut);
      }
      catch(Error err) {
        err.printError();
        this->queueAux->makeEmpty();
        this->queueInfx->makeEmpty();
        this->queuePosfx->makeEmpty();

        this->expression.clear();
        this->lastOpenedScope.clear();
        this->lastClosedScope.clear();
      }
  }
  fileOut.close();
}

void BARES::pushLine(std::string line, ofstream fileOut) {
  vector<std::string> tokens;
  std::string symb = "";
  int slow = 0;
  bool lastIsNumber = false;
  for(int i = 0; i < line.size(); i++) {
    if(line[i] == " ") { // don't push space to vector
      continue;
    }
    else if((line[i] == char(BARES::VALID::SUBTRACAO)) && !lastISNumber) {
      symb = symb + line[i];
    }
    else if(isdigit(line[i])) {
      symb = symb + line[i];
      lastISNumber = true;
    }
    else {
      if(i == 0) { // especial case, the first element is a operand. This "if" don't let the tokens have a void string
        tokens[slow] = line[i];
        slow++;
        continue;
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
      lastISNumber = false;
    }
  }

  lineErros(tokens);
  result = scopes(tokens);

  fileOut >> result;
}

bool BARES::isNumber(std::string theNumber) {
  for(int i = 0; i < theNumber.size(); i++) {
    if(isdigit(theNumber[i])) {
      return true;
    }
  }
  return false;
}

void BARES::lineErros(vector<std::string> tokens) {
  std::string symb;
  int number = 0;
  bool lastIsNumber = false;
  long unsigned int count = 0;
  int slowClose = 0;
  int slowOpen = 0;
  for(int i = 0; i < tokens.size(); i++) {
    symb = tokens[i];
    if((isNumber(symb)) && (symb[0] == char(BARES::VALID::SUBTRACAO)) ) { // number is negative
      lastIsNumber = true;
      std::sting aux;
      int e;
      for(e = 0; e < symb.size(); e++) { // get the index just after the last symbol of less
        if(symb[e] != char(BARES::VALID::SUBTRACAO)) {
          break;
        }
      }
      for(int r = e; r < symb.size(); r++) { //push the number without the less symbol
        aux = symb[r];
      }
      number = stoi(aux);
      if(32767 < number) { // if the number is out of ranger
        throw(Error( i+1, Error::Errors::NumericConstantOutOfRange));
      }
      number = 0;
      expression[count] = line[i];
      count++;
    }
    else if(isNumber(symb)) { // number is positive
      lastIsNumber = true;
      number = stoi(symb);
      if(32767 < number) { // if the number is out of ranger
        throw(Error( i+1, Error::Errors::NumericConstantOutOfRange));
      }
      number = 0;
      expression[count] = line[i];
      count++;
    }
    else if((i == 0) && (1 == symb.size() && !(isdigit(symb[0])) ) ) { // first element is a unexpected operand
      if(isValidOperand(symb[0])) { // is a valid operand
        throw(Error(i+1, Error::Errors::LostOperator));
      }
      else { // is a invalid operand
        throw(Error(i+1, Error::Errors::InvalidOperand));
      }
      expression[count] = line[i];
      count++;
    }
    else {
      switch(symb[0]) {
        case char(BARES::VALID::ADICAO) :
          if(!lastIsNumber) { // operand of plus without the fisrt operator
            throw(i+1, Error(Error::Errors::LostOperator));
          }
          if(i == tokens.size()-1) {
            throw(i+1, Error(Error::Errors::LostOperator));
          }
          else {
            if(tokens[i+1] != char(BARES::VALID::PARENTESABRE) || isdigit(tokens[i+1])) {
              throw(i+1, Error(Error::Errors::LostOperator));
            }
            else if(tokens[i-1] != char(BARES::VALID::PARENTESFECHA) || isdigit(tokens[i-1])) {
              throw(i+1, Error(Error::Errors::LostOperator));
            }
          }
          this->expression[count] = line[i];
          count++;
          break;
        case char(BARES::VALID::SUBTRACAO) :
          if(!lastIsNumber) { // operand of less without the fisrt operator
            throw(i+1, Error(Error::Errors::LostOperator));
          }
          if(i == tokens.size()-1) {
            throw(i+1, Error(Error::Errors::LostOperator));
          }
          else {
            if(tokens[i+1] != char(BARES::VALID::PARENTESABRE) || isdigit(tokens[i+1])) {
              throw(i+1, Error(Error::Errors::LostOperator));
            }
            else if(tokens[i-1] != char(BARES::VALID::PARENTESFECHA) || isdigit(tokens[i-1])) {
              throw(i+1, Error(Error::Errors::LostOperator));
            }
          }
          this->expression[count] = line[i];
          count++;
          break;
        case char(BARES::VALID::MULTIPLICACAO) :
          if(!lastIsNumber) { // operand of multply without the fisrt operator
            throw(i+1, Error(Error::Errors::LostOperator));
          }
          if(i == tokens.size()-1) {
            throw(i+1, Error(Error::Errors::LostOperator));
          }
          else {
            if(tokens[i+1] != char(BARES::VALID::PARENTESABRE) || isdigit(tokens[i+1])) {
              throw(i+1, Error(Error::Errors::LostOperator));
            }
            else if(tokens[i-1] != char(BARES::VALID::PARENTESFECHA) || isdigit(tokens[i-1])) {
              throw(i+1, Error(Error::Errors::LostOperator));
            }
          }
          this->expression[count] = line[i];
          count++;
          break;
        case char(BARES::VALID::DIVISAO) :
          if(!lastIsNumber) { // operand of multply without the fisrt operator
            throw(i+1, Error(Error::Errors::LostOperator));
          }
          if(i == tokens.size()-1) {
            throw(i+1, Error(Error::Errors::LostOperator));
          }
          else {
            if(tokens[i+1] != char(BARES::VALID::PARENTESABRE) || isdigit(tokens[i+1])) {
              throw(i+1, Error(Error::Errors::LostOperator));
            }
            else if(tokens[i-1] != char(BARES::VALID::PARENTESFECHA) || isdigit(tokens[i-1])) {
              throw(i+1, Error(Error::Errors::LostOperator));
            }
          }
          this->expression[count] = line[i];
          count++;
          break;
        case char(BARES::VALID::POTENCIA) :
          if(!lastIsNumber) { // operand of multply without the fisrt operator
            throw(i+1, Error(Error::Errors::LostOperator));
          }
          if(i == tokens.size()-1) {
            throw(i+1, Error(Error::Errors::LostOperator));
          }
          else {
            if(tokens[i+1] != char(BARES::VALID::PARENTESABRE) || isdigit(tokens[i+1])) {
              throw(i+1, Error(Error::Errors::LostOperator));
            }
            else if(tokens[i-1] != char(BARES::VALID::PARENTESFECHA) || isdigit(tokens[i-1])) {
              throw(i+1, Error(Error::Errors::LostOperator));
            }
          }
          this->expression[count] = line[i];
          count++;
          break;
        case char(BARES::VALID::MODULO) :
          if(!lastIsNumber) { // operand of multply without the fisrt operator
            throw(i+1, Error(Error::Errors::LostOperator));
          }
          if(i == tokens.size()-1) {
            throw(i+1, Error(Error::Errors::LostOperator));
          }
          else {
            if(tokens[i+1] != char(BARES::VALID::PARENTESABRE) || isdigit(tokens[i+1])) {
              throw(i+1, Error(Error::Errors::LostOperator));
            }
            else if(tokens[i-1] != char(BARES::VALID::PARENTESFECHA) || isdigit(tokens[i-1])) {
              throw(i+1, Error(Error::Errors::LostOperator));
            }
          }
          this->expression[count] = line[i];
          count++;
          break;
        case char(BARES::VALID::PARENTESFECHA) :
          lastClosedScope[slowClose] = i;
          continue;
          break;
        case char(BARES::VALID::PARENTESABRE) :
          lastOpenedScope[slowOpen] = i;
          continue;
          break;
        default: // invalid operand
          throw(Error( i+1,Error::Errors::InvalidOperand));
      }
    }
  }
  if(lastClosedScope.size() < lastOpenedScope.size()) {
    throw(Error(Error::ERRORS::MissingClosing));
  }
  if(lastClosedScope.size() > lastOpenedScope.size()) {
    throw(Error(Error::ERRORS::Mismatch));
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
  lastOpenedScopeSize = lastOpenedScope.size();
  lastClosedScopeSize = lastClosedScope.size();
  for(int i = 0; i < lastOpenedScopeSize; i++) {
    for(int e = lastOpenedScope[lastOpenedScopeSize-1-i]; e < lastClosedScope[i]; i++) {
      this->queueInfx->enqueue(tokens[e+1]);
    }
    InfxToPosfx();
    result = avaliaPosfx();

    delete queueInfx;
    delete queuePosfx;
    delete queueAux;

    queuePosfx = nullptr;
    queueInfx = nullptr;
    queueAux = nullptr;

    // if(lastOpenedScope[i] == -1) {
    //   break;
    // }

    int aux = lastOpenedScope[lastOpenedScopeSize-1-i];
    tokens[aux] = result;
    for(int r = lastClosedScope[i]+1; r < lastOpenedScope[lastOpenedScopeSize-1-i]+1 && lastClosedScope[i]+1 != tokens.size(); i--) {
      tokens[aux] = tokens[r];
      aux++;
    }
  }

  for(int e = 0; e < tokens.size(); i++) {
    this->queueInfx->enqueue(tokens[e]);
  }
  InfxToPosfx();
  result = avaliaPosfx();

  this->queueAux->makeEmpty();
  this->queueInfx->makeEmpty();
  this->queuePosfx->makeEmpty();

  this->expression.clear();
  this->lastOpenedScope.clear();
  this->lastClosedScope.clear();

  return result;
}

bool prcd(std::string top, std::string symb) {
  switch(top) {
    case Bares::VALID::_ADICAO:
      top = Bares::PRECEDENCE::_ADICAO;
      break;
    case Bares::VALID::_SUBTRACAO:
      top = Bares::PRECEDENCE::_SUBTRACAO;
      break;
    case Bares::VALID::_MULTIPLICACAO:
      top = Bares::PRECEDENCE::_MULTIPLICACAO;
      break;
    case Bares::VALID::_DIVISAO:
      top = Bares::PRECEDENCE::_DIVISAO;
      break;
    case Bares::VALID::_MODULO:
      top = Bares::PRECEDENCE::_MODULO;
      break;
    case Bares::VALID::_POTENCIA:
      top = Bares::PRECEDENCE::_POTENCIA;
      break;
  }
  switch(symb) {
    case Bares::VALID::_ADICAO:
      symb = Bares::PRECEDENCE::_ADICAO;
      break;
    case Bares::VALID::_SUBTRACAO:
      symb = Bares::PRECEDENCE::_SUBTRACAO;
      break;
    case Bares::VALID::_MULTIPLICACAO:
      symb = Bares::PRECEDENCE::_MULTIPLICACAO;
      break;
    case Bares::VALID::_DIVISAO:
      symb = Bares::PRECEDENCE::_DIVISAO;
      break;
    case Bares::VALID::_MODULO:
      symb = Bares::PRECEDENCE::_MODULO;
      break;
    case Bares::VALID::_POTENCIA:
      symb = Bares::PRECEDENCE::_POTENCIA;
      break;
  }
  return (top >= symb);
}

void BARES::InfxToPosfx() {
  std::string symb;
  StackAr<int> stack;
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
  StackAr<std::string> stack;
  long int symb, opnd1 = 0, opnd2 = 0, result = 0, i = 0;

  while( !(queuePosfx->isEmpty()) ) {
    symb = queuePosfx->dequeue();
    if(!(isOperator(symb)) ) {
      stack.push(symb);
    }
    else {
      opnd2 = stoi(stack.pop());
      opnd1 = stoi(stack.pop());
      switch(symb) {
        case char(Bares::VALID::_ADICAO):
          result = opnd1 + opnd2;
          break;
        case char(Bares::VALID::_SUBTRACAO):
          result = opnd1 - opnd2;
          break;
        case char(Bares::VALID::_MULTIPLICACAO):
          result = opnd1 * opnd2;
          break;
        case char(Bares::VALID::_DIVISAO):
          if(opnd2 == 0) {
            stack.makeEmpty();
            throw(i+1, Error(Error::Errors::DivisionByZero));
          } else {
            result = opnd1 / opnd2;
          }
          break;
        case char(Bares::VALID::_MODULO):
          result = opnd1 % opnd2;
          break;
        case char(Bares::VALID::_POTENCIA):
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
