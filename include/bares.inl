#ifndef _BARES_INL_
#define _BARES_INL_

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "QueueAr.h"
#include "StackAr.h"
#include "bares.hpp"
#include "erros.hpp"

void BARES::runFile(std::string fileName, std::string fileNameOut) {

  std::ifstream fileIn;
  fileIn.open(fileName);

  std::cout << fileName << std::endl;

  if(!(fileIn.is_open())) {
    std::cout << ">>> O arquivo não pode ser aberto por algum motivo. Tentem novamente." << std::endl;
    fileIn.close();
    exit(1);
  }

  std::string finalString; // armazena TUDO que será escrito no arquivo de saida

  std::string line;
  while(!(fileIn.eof())) {

    std::cout << "TEFINAL STRING\n\n\n\n" << std::endl;
    std::cout << finalString << std::endl;
    std::cout << "TEFINAL STRING\n\n\n\n" << std::endl;

    std::vector<std::string> expression(1000); // queue that get the expression
    unsigned int slow = 0;
    std::getline(fileIn, line); // reading the line of the file
    if(line == "") { continue; }
    for(unsigned long int i = 0; i < line.size() && line[i] != '\n'; i++) {
      std::cout << ">>> Line size:" << line.size() << std::endl;
      std::cout << "teste1:" << std::endl;
      std::cout << line[i] << std::endl;
      std::cout << "teste2:" << std::endl;
      if(line[i] == ' ') {
        expression[slow] = line[i];
        slow++;
        continue;
      }
      if(isdigit(line[i])) {
        unsigned long int r = 0;
        std::string number = "";
        for(r = i; r < line.size() && isdigit(line[r]); r++) {
          number = number + line[r];
        }
        i = r-1;
        expression[slow] = number;
        slow++;
      }
      else {
        std::string operand = "";
        operand = line[i];
        expression[slow] = operand;
        slow++;
      }
    }

    std::cout << "IMPRIMINDO" << std::endl;
    unsigned int e = 0;
    for(e = 0; e < expression.size() && expression[e] != ""; e++) {
      std::cout << expression[e] << std::endl;
    }


    // avaliação de erros da expressão

    try {

      std::cout << "TESTE 444444444444444444444444" << std::endl;

      std::vector<std::string> expressionAux(1000);
      bool lastIsParentesFechado = false;
      bool lastIsNumber = false;
      bool lastIsDivision = false;
      int ehUnario = -1; // -1 significa que não é menos unário e 1 significa que é menos unário
      int ehNegativo = -1;
      int escopo = 0;
      for( unsigned long int count = 0; count < expression.size() && expression[count] != ""; count++) {
        std::string symb = expression[count];
        if(symb[0] == ' ') { continue; } // no teste espaços não serão contabilizados
        if((symb[0] == '-' && (!lastIsNumber && !lastIsParentesFechado)) && (ehUnario == -1)) {
          // verifica se o elemento atual é um menos, e se for verifica se o anterior é uma valor numerico ou parenteses
          ehUnario == 1;
        }
        else if((ehUnario == 1) && symb[0] == '-') {
          // para o caso de existir um menos depois do outro no unário. ex.: 4 + -----1
          while(symb[0] == '-') {
            if(count+1 == expression.size() ) {
              // ill-formed
              throw(Erros(count+1, Erros::erros::IllformedExpressionOrMissingTermDetected));
            }
            ehUnario = ehUnario * (-1);
            count = count + 1;
            symb = expression[count];
          }
        }
        else if(isNumber(symb)) {
          if(lastIsNumber) {
            // deu erro extraneus pois o anterior é um numero
          }
          long int number = std::stoi(symb);
          if(32676 < number) {
            // numero fora do range aceitavel
          }
          if(number == 0 && lastIsDivision) {
            // o numero atual é um zero e o elemento anterior é uma divsão
            throw(Erros(count+1, Erros::erros::DivisionByZero));
          }
          if(ehUnario == 1) {
            number = number * (-1);
            ehUnario = -1;
          }
          expressionAux[count] = std::to_string(number);
          lastIsNumber = true;
          lastIsDivision = false;
          lastIsParentesFechado = false;
        }
        else {
          if((symb[0] == '+') || (symb[0] == '*') || (symb[0] == '/') || (symb[0] == '^') || (symb[0] == '%')) {
            if((!lastIsNumber && !lastIsParentesFechado)) {
              // deu erro lost operator
              throw(Erros(count+1, Erros::erros::LostOperator));
            }
            else {
              if(symb[0] == '/') { lastIsDivision = true; }
              else { lastIsDivision = false; }
              expressionAux[count] = symb;
            }
          }
          else if(symb[0] == '-') {
            // o elemento atual é um simbolo de menos
            if(expression[count+1][0] == '\n') {
              // deu erro lost operator pois o proximo elemento na subtração não exites
              throw(Erros(count+1, Erros::erros::LostOperator));
            }
            else {
              // está tudo certo com essa subtração
              expressionAux[count] = symb;
              lastIsDivision = true;
            }
          }
          else if(symb[0] == '(') {
            escopo++;
          }
          else if(symb[0] == ')') {
            escopo--;
          }
          else if((symb[0] == '.') || (symb[0] == '=')) {
            throw(Erros(count+1, Erros::erros::InvalidOperand));
          }
          else if(symb[0] == '\n') {
            if(!lastIsNumber && !lastIsParentesFechado) {
              throw(Erros(count+1, Erros::erros::LostOperator));
            }
          }
          else {
            throw(Erros(count+1, Erros::erros::IllformedExpressionOrMissingTermDetected));
          }
          lastIsNumber = false;
        }
      }
      if(0 < escopo) {
        // existe mais parentes abertos doque fechados
        unsigned long int count = 0;
        for(count = 0; count < expression.size(); count++) {
          if(expression[count][0] == ')') { break; }
        }
        throw(Erros(count+1, Erros::erros::Mismatch));
      }
      else if(escopo < 0) {
        // existe mais parentes fechados doque abertos
        unsigned long int count = 0;
        int a = 0;
        int p = -1;
        for(count = expression.size(); 0 <= count; count++) {
          if(expression[count][0] == '(') { a++; }
          if(expression[count][0] == ')') { a--; p = count; }
          if(expression[count][0] == ')' && a < 0) { throw(Erros(count+1, Erros::erros::MissingClosing)); }
          if(expression[count][0] == '\n' && a < 0) { throw(Erros(p+1, Erros::erros::MissingClosing)); }
        }
        throw(Erros(count+1, Erros::erros::MissingClosing));
      }

      // erros foram verificados, agora é necessário fazer o tratamento de escopos da expressão

      std::cout << "TESTE!!!!!!!" << std::endl;

      for(unsigned long int i = 0; i < expressionAux.size(); i++) {
        // descobrindo aonde estão os escopos

      }

    }
    catch (Erros err) {
      std::string msg = err.oErro();
      finalString = finalString + msg;
    }
  }

  std::ofstream fileOut;
  fileOut.open(fileNameOut);

  fileOut << finalString;

  fileOut.close();

}


bool BARES::isNumber(std::string number) {
  for(unsigned int w = 0; w < number.size(); w++) {
    if(isdigit(number[w])) {
      return true;
    }
  }
  return false;
}

#endif
