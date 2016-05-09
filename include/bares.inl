#ifndef _BARES_INL_
#define _BARES_INL_

#include <iostream>
#include <string>
#include <vector>

#include "bares.hpp"
#include "erros.h"

void BARES::run(int argc, const char argv[]) {
    if(argc != 2) {
      std::cout << ">>> Error. please use this notation:" << estd::endl;
      std::cout << ">>> ./bares NameOfTheFileToRead NameOfTheFileToOut" << std::endl;
    }

    std::string namefileIn = argv[1];
    std::string namefileOut = argv[2];

    std::ifstream fileIn;
    fileIn.open(namefileIn);

    if(!(fileIn.is_open()) ) {
      std::cout << ">>> Error. Sorry but Wasn't posible open the file " << namefileIn << std::endl;
    }

    while( !(fileIn.eof()) ) {
      getline(fileIn, line); // get a line of the file and push into the line string variable
      if (line == "") {
        break;
      }
      try{
        pushLine(std::string line);
      }
      catch(Error err) {
        err.printError();
        this->queueAux->makeEmpty();
      }
  }
}

void BARES::pushLine(std::string line) {
  vector<std::string> tokens;
  std::string symb = "";
  int slow = 0;
  bool lastIsNumber false;
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
  scopes(tokens);

  }
}

void lineErros(vector<std::string> tokens) {
  std::string symb;
  int number = 0;
  for(int i = 0; i < tokens.size(); i++) {
    symb = tokens[i];
    if((1 < symb.size()) && (symb[0] == char(BARES::VALID::SUBTRACAO)) ) { // number is negative
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
    }
    else if(1 < symb.size()) { // number is positive
      number = stoi(symb);
      if(32767 < number) { // if the number is out of ranger
        throw(Error( i+1, Error::Errors::NumericConstantOutOfRange));
      }
      number = 0;
    }
    else if((i == 0) && (1 == symb.size()) ) { // first element is a unexpected operand
      if(isValidOperand(symb[0])) { // is a valid operand
        throw(Error(i+1, Error::Errors::LostOperator));
      }
      else { // is a invalid operand
        throw(Error(i+1, Error::Errors::InvalidOperand));
      }
    }
    else {
      switch(symb) {
        case char(BARES::VALID::ADICAO) :

          break;
        case char(BARES::VALID::SUBTRACAO) :

          break;
        case char(BARES::VALID::MULTIPLICACAO) :

          break;
        case char(BARES::VALID::DIVISAO) :

          break;
        case char(BARES::VALID::POTENCIA) :

          break;
        case char(BARES::VALID::MODULO) :

          break;
        case char(BARES::VALID::PARENTESABRE) :

          break;
        case char(BARES::VALID::PARENTESFECHA) :

          break;
        default: // invalid operand
          throw(Error(Error::Errors::InvalidOperand, i + /*size of avery element of this line*/ 1));
      }
    }
  }
}

bool isValidOperand(char symb) {
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

void scopes(vector<std::string> tokens) {
  std::string symb;
  int i;
  int e;
  vector<int> lastOpenedScope; // stores index (of tokens) of the position of every opened parentsis
  vector<int> lastClosedScope; // stores index (of tokens) of the position of every closed parentsis

  int slow = 0;
  for(i = 0; i < tokens.size(); i++) {
    if(tokens[i] == char(BARES::VALID::PARENTESABRE)) {
      lastOpenedScope[slow] = i;
      slow++;
    }
  }

  slow = 0;
  for(e = tokens.size(); 0 <= e; e--) {
    if(tokens[e] == char(BARES::VALID::PARENTESFECHA)) {
      lastClosedScope[slow] = e;
      slow++;
    }
  }

  if(lastClosedScope.size() < lastOpenedScope.size()) {
    throw(Error(Error::ERRORS::MissingClosing));
  }

  if(lastClosedScope.size() > lastOpenedScope.size()) {
    throw(Error(Error::ERRORS::Mismatch));
  }

  if(0 < lastOpenedScope.size()) {
    int result = 0;
    for(int r = 0; r < lastClosedScope.size(); r++) {
      int indexOfClosed = lastClosedScope[r]; // the scope size
      int indexOfOpened = lastOpenedScope[lastOpenedScope.size()-r];
      for(int w = 1; w <= (indexOfClosed - indexOfOpened - 1); w++) { // push each value to queueInfx
        this->queueInfx->enqueue(tokens[w+indexOfOpened]);

        this->InfxToPosfx();

        result = result + this->avaliaPosfx();

      }
    }
  }

}

void BARES::InfxToPosfx() {}

int BARES::avaliaPosfx(){}

#endif
