#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern double
my_sin(double x);

#define x 1.111128

int
main ()
{
  printf("\tmy_sin(%lf) = %lf\n", x, my_sin(x));
  printf("\t   sin(%lf) = %lf\n", x, sin(x));
  return 0;
}

