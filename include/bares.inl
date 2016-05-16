#ifndef _BARES_INL_
#define _BARES_INL_

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <math.h>

#include "QueueAr.h"
// #include "stackar.h"
#include "thePilha.hpp"
#include "bares.hpp"
#include "erros.hpp"

void BARES::runFile(std::string fileName, std::string fileNameOut) {

  std::ifstream fileIn;
  fileIn.open(fileName);

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

      // std::vector<std::string> expressionAux(1000);
      bool lastIsParentesFechado = false;
      bool lastIsNumber = false;
      bool lastIsDivision = false;
      int ehUnario = -1; // -1 significa que não é menos unário e 1 significa que é menos unário
      int ehNegativo = -1;
      int escopo = 0;
      unsigned long int count = 0;
      for( count = 0; count < expression.size() /* && expression[count] != "" */; count++) {
        if((0 < count) && (expression[count-1] == "")) {
          break;
        }
        std::string symb = expression[count];
        if(symb[0] == ' ') { continue; } // nos testes espaços não serão contabilizados
        if((symb[0] == '-' && (!lastIsNumber /* && !lastIsParentesFechado */)) && (ehUnario == -1)) {
          // verifica se o elemento atual é um menos, e se for verifica se o anterior é uma valor numerico ou parenteses
          ehUnario = 1;
          expression[count][0] = ' ';
          continue;
        }
        else if((ehUnario == 1) && symb[0] == '-') {
          // std::cout << "oeeeeeeeeeeeeeeeeeeeee" << std::endl;
          // para o caso de existir um menos depois do outro no unário. ex.: 4 + -----1
          while(symb[0] == '-') {
            if(count+1 == expression.size() || expression[count+1] == "" ) {
              // ill-formed
              throw(Erros(count+1, Erros::erros::IllformedExpressionOrMissingTermDetected));
            }
            expression[count][0] = ' ';
            ehUnario = ehUnario * (-1);
            count = count + 1;
            symb = expression[count];
          }
          count--;
        }
        else if(isNumber(symb)) {
          if(lastIsNumber) {
            // deu erro extraneus pois o anterior é um numero
            throw(Erros(count+1, Erros::erros::ExtraneousSymbol));
          }
          long int number = std::stoi(symb);
          if(32676 < number) {
            // numero fora do range aceitavel
            throw(Erros(count+1, Erros::erros::NumericConstantOutOfRange));
          }
          if((number == 0) && lastIsDivision) {
            // o numero atual é um zero e o elemento anterior é uma divsão
            throw(Erros(count+1, Erros::erros::DivisionByZero));
          }
          if(ehUnario == 1) {
            number = number * (-1);
            expression[count] = std::to_string(number);
            ehUnario = -1;
          }
          // expressionAux[count] = std::to_string(number);
          lastIsNumber = true;
          lastIsDivision = false;
          // lastIsParentesFechado = false;
        }
        else {
          if((symb[0] == '+') || (symb[0] == '*') || (symb[0] == '/') || (symb[0] == '^') || (symb[0] == '%')) {
            if((!lastIsNumber && !lastIsParentesFechado)) {
              // deu erro lost operator
              throw(Erros(count+1, Erros::erros::LostOperator));
            }
            if(symb[0] == '/') { lastIsDivision = true; }
            else { lastIsDivision = false; }
            lastIsParentesFechado = false;
            // expressionAux[count] = symb;
          }
          else if(symb[0] == '-') {
            // o elemento atual é um simbolo de menos
            if(expression[count+1] == "") {
              // deu erro lost operator pois o proximo elemento na subtração não existe
              throw(Erros(count+1, Erros::erros::LostOperator));
            }
            else {
              // está tudo certo com essa subtração
              // expressionAux[count] = symb;
              lastIsDivision = false;
              lastIsParentesFechado = false;
            }
          }
          else if(symb[0] == '(') {
            if(lastIsParentesFechado || lastIsNumber) {
              throw(Erros(count+1, Erros::erros::ExtraneousSymbol));
            }
            //acrecentando em expressionAux
            // expressionAux[auxSlow] = expression[count];
            // auxSlow++;
            lastIsParentesFechado = false;
            escopo++;
          }
          else if(symb[0] == ')') {
            if(escopo == 0) {
              throw(Erros(count+1, Erros::erros::Mismatch));
            }
            else if(!lastIsNumber) {
              throw(Erros(count+1, Erros::erros::IllformedExpressionOrMissingTermDetected));
            }
            //acrecentando em expressionAux
            // expressionAux[auxSlow] = expression[count];
            // auxSlow++;
            lastIsParentesFechado = true;
            escopo--;
          }
          else if((symb[0] == '.') || (symb[0] == '=')) {
            throw(Erros(count+1, Erros::erros::InvalidOperand));
          }
          else if(expression[count+1] == "") {
            // esta no fim da linha e não tem mais numeros, sendo assim deu erro
            if(!lastIsNumber && !lastIsParentesFechado ) {
              throw(Erros(count+1, Erros::erros::IllformedExpressionOrMissingTermDetected));
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
        throw(Erros(count+1, Erros::erros::MissingClosing));
      }
      else if(escopo < 0) {
        // existe mais parentes fechados doque abertos
        unsigned long int count = 0;
        int a = 0;
        int p = -1;
        for(count = expression.size(); 0 <= count; count++) {
          if(expression[count][0] == '(') { a++; }
          if(expression[count][0] == ')') { a--; p = count; }
          if(expression[count][0] == ')' && a < 0) { throw(Erros(count+1, Erros::erros::Mismatch)); }
          if(expression[count][0] == '\n' && a < 0) { throw(Erros(p+1, Erros::erros::Mismatch)); }
        }
        throw(Erros(count+1, Erros::erros::Mismatch));
      }

      /* erros que podem ocorrer na expressão foram verificados, agora é
        necessário fazer o tratamento de escopos (ordem de precedencia de cada parenteses) da expressão */

      std::vector<BARES::Opened> auxParentAbertos(1000);
      int slowVectorOpened = 1;
      std::vector<BARES::Closed> auxParentFechados(1000);
      int slowVectorClosed = 1;
      StackAr<int> posicaoEscopos;
      int positionCounter = 2;

      int re = 0;
      for(re = 0; expression[re] != "" && re < expression.size(); re++) {}

      auxParentAbertos[0]  = BARES::Opened{-1, 1}; // caso isso não seja feito na linha 277 vai dar segmentation fault
      auxParentFechados[0] = BARES::Closed{re, 1}; // caso isso não seja feito na linha 278 vai dar segmentation fault
      for(long int t = 0; t < expression.size() && expression[t] != ""; t++) {
        // descobrindo aonde estão os escopos
        std::string symb = expression[t];
        if(symb[0] == ' ') { continue; } // nos testes espaços não serão contabilizados

        BARES::Opened auxOpened;
        BARES::Closed auxClosed;

        if(symb[0] == '(') {
          auxOpened = BARES::Opened{t, positionCounter}; // cusando um struct Opened cria uma identificação para esse parenteses
          posicaoEscopos.push(positionCounter); //adiciona a posição do escopo a pilha
          positionCounter++; // incrementa em 1 a posição para que o proximo parenteses aberto fique ja com a posição correta
          auxParentAbertos[slowVectorOpened] = auxOpened; // adiciona o a "assinatura" do perenteses ao vector
          slowVectorOpened++; // incrementa em 1 a posição do vector para que o proximo elemento já entre ana posição correta
        }
        else if(symb[0] == ')') {
          std::cout << "PARENTES FECHADO = " << t << std::endl;
          int auxPositionClosed = posicaoEscopos.pop();
          auxClosed = BARES::Closed{t, auxPositionClosed};
          auxParentFechados[slowVectorClosed] = auxClosed;
          slowVectorClosed++;
        }
      }

      for( re = 0; expression[re] != "" && expression.size() != re; re++) {}
      std::vector<std::string> auxExpression(re);
      for(int we = 0; we < re; we++) {
        auxExpression[we] = expression[we];
      }

      // coloca os perenteses em seu vector, assim é possivel ter um controle maior dos parenteses
      std::vector<BARES::Opened> parentAbertos(positionCounter-1);
      std::vector<BARES::Closed> parentFechados(positionCounter-1);
      for(unsigned long int t = 0; t < positionCounter-1; t++) {
        int auxIdiceC;
        int auxParentesPosicaoA = auxParentAbertos[t].position;
        for(unsigned long int k = 0; k < positionCounter-1; k++) {
          if(auxParentesPosicaoA == auxParentFechados[k].position) {
            auxIdiceC = k;
            break;
          }
        }
        parentAbertos[t] = auxParentAbertos[t];
        parentFechados[t] = auxParentFechados[auxIdiceC];
      }

      std::string auxStringResultado = "";
      for(int t = parentAbertos.size()-1; 0 <= t; t--) {
        int auxIndiceAbre = parentAbertos[t].index;
        int auxIndiceFechados = parentFechados[t].index;

        std::cout << "auxIndiceAbre = " << auxIndiceAbre << " , auxIndiceFechados = " << auxIndiceFechados << std::endl;

        for(unsigned long int k = auxIndiceAbre+1; k < auxIndiceFechados; k++) {
          if(auxExpression[k] == " ") { continue; }
          std::cout << "COLOCANDO O SEGUINTE ELEMENTO   " << auxExpression[k] << std::endl;
          queueInfx->enqueue(auxExpression[k]);
        }

        // std::cout << "        [" << std::endl;
        // for(int qe = 0; qe < auxExpression.size(); qe++) {
        //   std::cout << "  " << auxExpression[qe];
        // }
        // std::cout << "] " << std::endl;

        // std::vector<std::string> vt(auxIndiceFechados - auxIndiceAbre);
        // for(int qe = 0; qe < vt.size(); qe++) {
        //   vt[qe] = queueInfx->enqueue;
        // }
        //
        // std::cout << "        [" << std::endl;
        // for(int qe = 0; qe < vt.size(); qe++) {
        //   std::cout << "  " << vt[qe];
        // }
        // std::cout << "] " << std::endl;

        InfxToPosfx();
        //auxExpression ok ate aki
        long int auxIntResutado = avaliaPosfx();

        delete queueInfx;
        delete queuePosfx;

        queueInfx = new QueueAr<std::string>;
        queuePosfx = new QueueAr<std::string>;

        if(auxIntResutado < -32767 || 32767 < auxIntResutado) {
          throw(Erros(0, Erros::erros::NumericOverflowError));
        }

        if(auxIndiceAbre != -1) {
          std::vector<std::string> auxAux(auxExpression.size()-(auxIndiceFechados-auxIndiceAbre)+1);
          for(int er = 0; er < auxIndiceAbre; er++) {
            auxAux[er] = auxExpression[er];
          }
          auxAux[auxIndiceAbre] = std::to_string(auxIntResutado);
          if(auxIndiceFechados < auxExpression.size()) {
            int fast = auxIndiceFechados+1;
            for(int er = auxIndiceAbre; er < auxAux.size() && auxExpression.size() != fast; er++) {
              auxAux[er] = auxExpression[fast];
              fast++;
            }
          }
          auxExpression.resize(auxAux.size(), "");
          for(int er = 0; er < auxAux.size(); er++) {
            auxExpression[er] = auxAux[er];
          }
        }
        else {
          auxExpression.resize(1);
        }

        auxStringResultado = auxStringResultado + std::to_string(auxIntResutado);
        auxStringResultado = auxStringResultado + '\n';
      }

      finalString = finalString + auxStringResultado;

    }
    catch (Erros err) {
      std::string msg = err.oErro();
      finalString = finalString + msg;
      msg = "";

      delete queueInfx;
      delete queuePosfx;

      queueInfx = new QueueAr<std::string>;
      queuePosfx = new QueueAr<std::string>;
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

bool prcd(std::string top, std::string symb) {
  switch(top[0]) {
    case '+':
      top = BARES::PRECEDENCE::_ADICAO;
      break;
    case '-':
      top = BARES::PRECEDENCE::_SUBTRACAO;
      break;
    case '*':
      top = BARES::PRECEDENCE::_MULTIPLICACAO;
      break;
    case '/':
      top = BARES::PRECEDENCE::_DIVISAO;
      break;
    case '%':
      top = BARES::PRECEDENCE::_MODULO;
      break;
    case '^':
      top = BARES::PRECEDENCE::_POTENCIA;
      break;
  }
  switch(symb[0]) {
    case '+':
      symb = BARES::PRECEDENCE::_ADICAO;
      break;
    case '-':
      symb = BARES::PRECEDENCE::_SUBTRACAO;
      break;
    case '*':
      symb = BARES::PRECEDENCE::_MULTIPLICACAO;
      break;
    case '/':
      symb = BARES::PRECEDENCE::_DIVISAO;
      break;
    case '%':
      symb = BARES::PRECEDENCE::_MODULO;
      break;
    case '^':
      symb = BARES::PRECEDENCE::_POTENCIA;
      break;
  }
  return (top <= symb);
}

void BARES::InfxToPosfx() {
  std::cout << "TRATAMENTO INFIX" << std::endl;
  std::string symb;
  StackAr<std::string> stack;
  while(!queueInfx->isEmpty()) {
    symb = queueInfx->dequeue();
    std::cout << "O SYMB =  " << symb << std::endl;
    if(isNumber(symb)) {
      std::cout << "COLOCANDO NA FILA POSFX = " << symb << std::endl;
      queuePosfx->enqueue(symb);
    }
    else {
      while(!(stack.isEmpty()) && prcd(stack.top(), symb)) {
        if(prcd(stack.top(), symb)) {
          std::cout << "COLOCANDO NA FILA POSFX = " << symb << std::endl;
          this->queuePosfx->enqueue(stack.pop());
        }
      }
      stack.push(symb);
    }
  }
  while( !stack.isEmpty()) {
    symb = stack.pop();
    std::cout << "COLOCANDO NA FILA POSFX = " << symb << std::endl;
    this->queuePosfx->enqueue(symb);
  }
  stack.makeEmpty();
}

int BARES::avaliaPosfx(){
  std::cout << "AVALIAÇÃO POSFIXA" << std::endl;
  StackAr<int> stack;
  int opnd1 = 0, opnd2 = 0, result = 0, i = 0;
  std::string symb;

  while( !(queuePosfx->isEmpty()) ) {
    stack.printStack();
    symb = queuePosfx->dequeue();
    if(isNumber(symb)) {
      stack.push(std::stoi(symb));
    }
    else {
      opnd2 = stack.pop();
      opnd1 = stack.pop();
      std::cout << "OPERAÇÃO SENDO FEITA    " << opnd1 << symb << opnd2 << std::endl;
      switch(symb[0]) {
        case '+':
          result = opnd1 + opnd2;
          break;
        case '-':
          result = opnd1 - opnd2;
          break;
        case '*':
          result = opnd1 * opnd2;
          break;
        case '/':
          if(opnd2 == 0) {
            throw(Erros(i+1, Erros::erros::DivisionByZero));
          }
            result = opnd1 / opnd2;
          break;
        case '%':
          if(opnd2 == 0) {
            throw(Erros(i+1, Erros::erros::Mismatch));
          }
          result = opnd1 % opnd2;
          break;
        case '^':
          result = pow(opnd1, opnd2);
          break;
      }
    }
    stack.push(result);
    i++;
  }
  result = stack.pop();
  stack.makeEmpty();
  return result;
}

#endif
