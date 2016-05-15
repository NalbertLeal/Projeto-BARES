#include <iostream>
#include <vector>
#include <string>

#include "bares.hpp"

int main(int argc, char const *argv[]) {

  if(argc < 3) {
    std::cout << ">>> Erro. Por favor use essa notação:" << std::endl;
    std::cout << ">>> ./bares nomeDoArquivoDeEntrada nomeDoArquivoDeSaida" << std::endl;
    exit(1);
  }

  std::string nameFileIn = argv[1];
  std::string nameFileOut = argv[2];

  BARES bares;
  bares.runFile(nameFileIn, nameFileOut);

  return EXIT_SUCCESS;
}
