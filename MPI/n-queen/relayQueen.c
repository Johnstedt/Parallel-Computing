#include <stdlib.h>
#include "n-queens.h"


int main(int argc, char* argv[]) {

  int brdsz = strtol(argv[1], NULL, 10);

  n_queens(brdsz);

  return 0;

}