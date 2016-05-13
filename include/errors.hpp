#ifndef _ERROR_HPP_
#define _ERROR_HPP_

#include <iostream>
#include <string>

class Error {
  public:
    Error(int coluna, int err) : coluna(coluna), err(err) {}
    enum Errors {
      NumericConstantOutOfRange = 1,
      LostOperator = 2,
      InvalidOperand = 3,
      IllFormedExpression = 4,
      ExtraneousSymbol = 5,
      Mismatch = 6,
      MissingClosing = 7,
      DivisionByZero = 8,
      NumericOverflowError = 9
    };
    std::string printError() {
      switch(err) {
        case 1:
          return "";
          break;
        case 2:
          return "";
          break;
        case 3:
          return "";
          break;
        case 4:
          return "";
          break;
        case 5:
          return "";
          break;
        case 6:
          return "";
          break;
        case 7:
          return "";
          break;
        case 8:
          return "";
          break;
        case 9:
          return "";
          break;
      }
    }

  private:
    int coluna, err;
};
#endif
