#ifndef _BARES_H_
#define _BARES_H_

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
      if ( (line == "") || (line == " ") ) {
        break;
      }
      // now errors can happen
      try{
        long int result = 0;

        this->avaliaLine(std::string line);

        this->infxToPosfx();

        result = this->avaliaPosfx();
      }

  }
  catch(Error err) {
    err.printError();
    this->queueAux->makeEmpty();
  }
}

void BARES::avaliaErrorsLine(std::string line) {
  vector<std::string> tokens;
  std::string symb = "";
  int slow = 1;
  bool lastIsNumber false;
  for(int i = 0; i < line.size(); i++) {
    if((line[i] == char(BARES::VALID::SUBTRACAO)) && !lastISNumber) {
      symb = symb + line[i];
    }
    else if(isdigit(line[i])) {
      symb = symb + line[i];
      lastISNumber = true;
    }
    else {
      tokens[slow] = symb;
      symb = "";
      slow++;
      tokens[slow] = line[i];
      slow++;
      lastISNumber = false;
    }
  }

  scopes(tokens);

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

  if(lastClosedScope.size() != lastOpenedScope.size()) {
    // error expresão mal formada
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

// void avaliaErrorsLine(std::string line) {
//   std::string symb = "";
//   for(int i = 0; i < line.size(); i++) {
//     if(line[i] == " ") {
//       continue;
//     }
//     if(isdigit(line[i]) ) {
//       for(int e = i; e < line.size() or isdigit(line[e])) {
//         symb = symb + line[e];
//       }
//       if(lastSymbIsNumber) {
//         throw(Errors(i+1, Errors::Error::IllFormedExpressionOrMissingTermDetected));
//       }
//       if(32767 < symb) {
//         throw(Errors(i+1, Errors::Error::NumericConstantOutOfRange));
//       }
//       this->queueAux->enqueue(symb);
//       lastSymbIsNumber = true;
//     }
//     else {
//       // valid or invalid operand
//
//       lastSymbIsNumber = false;
//     }
//
//     symb = "";
//   }
// }

void BARES::InfxToPosfx() {}

int BARES::avaliaPosfx(){}

#endif
