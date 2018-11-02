#include <stdio.h>

int 
solve(int A, int B, int C, int D);

int
main ()
{
  int A = 1
    , B = -16
    , C = 79
    , D = -120;
  printf
  (
    "solve(A=%d, B=%d, C=%d, D=%d) = 0 at point(%d)\n"
    , A, B, C, D
    , solve(A, B, C, D)
  );
  return 0;
}
