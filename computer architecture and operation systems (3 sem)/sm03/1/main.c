#include <stdio.h>

int 
summ (int x0, int N, int *X);

int
main ()
{
  int x0 = 10, N = 3;
  int X[3] = {228, 42, 23};
  printf("summ(x0=%d, N=%d, *X) = %d\n", x0, N, summ(x0, N, X));
  return 0;
}
