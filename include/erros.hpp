#ifndef _ERROS_HPP_
#define _ERROS_HPP_

class Erros {
  public:

    Erros(int coluna, int err) : coluna(coluna), err(err) {}

    enum erros {
      NumericConstantOutOfRange = 1,
      IllformedExpressionOrMissingTermDetected = 2,
      InvalidOperand = 3,
      ExtraneousSymbol = 4,
      Mismatch = 5,
      LostOperator = 6,
      MissingClosing = 7,
      DivisionByZero = 8,
      NumericOverflowError = 9
    };

    std::string oErro() {
      std::string retorno;
      switch(err) {
        case 1:
          retorno = "Numeric constant out of range: column " + std::to_string(coluna) +  ".\n";
          break;
        case 2:
          retorno =  "Ill-formed expression or missing term detected: column " + std::to_string(coluna) +  ".\n";
          break;
        case 3:
          retorno =  "Invalid operand: column " + std::to_string(coluna) +  ".\n";
          break;
        case 4:
          retorno =  "Extraneous symbol: column " + std::to_string(coluna) +  ".\n";
          break;
        case 5:
          retorno =  "Mismatch \')\': column " + std::to_string(coluna) +  ".\n";
          break;
        case 6:
          retorno =  "Missing closing \')\' to match opening \'(\' at: column " + std::to_string(coluna) +  ".\n";
          break;
        case 7:
          retorno =  "Lost operator: column " + std::to_string(coluna) +  ".\n";
          break;
        case 8:
          retorno =  "Division by zero!\n";
          break;
        case 9:
          retorno =  "Numeric overflow error!\n";
          break;
      }
      return retorno;
    }
  private:
    int coluna;
    int err;
};

#endif
