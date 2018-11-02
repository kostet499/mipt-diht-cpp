#include <stdio.h>

int 
f (int a, int b, int c, int x);

int
main ()
{
  int a = 0, b = 0, c = 0, x = 0;
  scanf("%d %d %d %d", &a, &b, &c, &x);
  printf("f(a=%d, b=%d, c=%d, x=%d) = %d\n", a, b, c, x, f(a, b, c, x));
  return 0;
}

/*
int
f (int a, int b, int c, int x)
{
  return a*x*x + b*x + c;
}
*/
