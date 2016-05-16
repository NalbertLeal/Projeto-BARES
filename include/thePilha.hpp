#ifndef _THEPILHA_HPP_
#define _THEPILHA_HPP_

template <class Data>
class StackAr {
  public:
    StackAr(int capacidade = 50) {
      capacidade = capacidade;
      stack = new Data[capacidade];
      topo = 0;
      proximo = 1;
    }
    ~StackAr() {
      delete [] stack;
    }

    void push(Data x) {
      // if(proximo == capacidade) {
      //   capacidade = capacidade * 2;
      //   Data *stackAux = new Data[capacidade];
      //
      //   for(int i = 0; i <= topo; i++) {
      //     stackAux[i] = stack[i];
      //   }
      //
      //   delete [] stack;
      //   stack = stackAux;
      // }
      stack[proximo] = x;
      proximo++;
      topo++;
    }

    Data pop() {
      if(!isEmpty()) {
        topo--;
        proximo--;
        return stack[proximo];
      }
    }

    bool isEmpty() {
      return topo == 0;
    }

    void makeEmpty() {
      topo = 0;
      proximo = 1;
    }

    Data top() {
      return stack[topo+1];
    }

    void printStack() {
      std::cout << ">>> The elements that still in the stack are: \n [ ";
      for(int i = 0; i < topo; i++) {
        std::cout << stack[i] << " " ;
      }
      std::cout << "] \n";
    }

  private:
    int capacidade;
    int topo;
    int proximo;
    Data * stack;
};



#endif
