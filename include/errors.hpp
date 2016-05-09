#ifndef _ERROR_HPP_
#define _ERROR_HPP__HPP_

#include <iostream>



class Error {
  public:
    Error(coluna, err) : coluna(coluna), err(error) {}
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
  private:
    int coluna, error;
};
#endif
