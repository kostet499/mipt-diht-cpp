#include <stdio.h>
#include <stdlib.h>


extern float
dot_product (int N, const float *A, const float *B);

int
main ()
{
  int N;
  scanf("%d", &N);
  float *A = (float *) malloc(N * sizeof(float));
  float *B = (float *) malloc(N * sizeof(float));
  for (int i = 0; i < N; i++)
  {
    scanf("%f", &A[i]);
  }
  for (int i = 0; i < N; i++)
  {
    scanf("%f", &B[i]);
  }
  printf("%f\n", dot_product(N, A, B));
  return 0;
}
