#include <assert.h>
#include <smmintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


extern void
matrix_multiply (uint32_t N, uint32_t M, const float *A, const float *B, float *R);


int
main ()
{
  uint32_t N = 0, M = 0, i = 0, j = 0;
  assert(scanf("%u %u", &N, &M));
  float *A = (float *) malloc(N * M * sizeof(float));
  float *B = (float *) malloc(M * N * sizeof(float));
  float *R = (float *) malloc(N * N * sizeof(float));
  for (i = 0; i < N; ++i)
  {
    for (j = 0; j < M; ++j)
    {
      assert(scanf("%f", &A[i * M + j]));
    }
  }
  for (j = 0; j < M; ++j)
  {
    for (i = 0; i < N; ++i)
    {
      assert(scanf("%f", &B[i * M + j]));
    }
  }
  matrix_multiply(N, M, A, B, R);
  for (i = 0; i < N; ++i)
  {
    for (j = 0; j < N; ++j)
    {
      printf("%0.4f ", R[i * N + j]);
    }
    printf("\n");
  }
  free(A);
  free(B);
  free(R);
  return 0;
}

extern void
matrix_multiply (uint32_t N, uint32_t M, const float *A, const float *B, float *R)
{
  uint32_t i, j;
  for (i = 0; i < N; ++i)
  {
    for (j = 0; j < N; ++j)
    {
      int k = 0;
      __m128 a_vec, b_vec, ans_vec = _mm_load_ss((float *) &k);
      for (k = 0; (k + 4) <= M; k += 4)
      {
        a_vec = _mm_loadu_ps(A + i * M + k);
        b_vec = _mm_loadu_ps(B + j * M + k);
        a_vec = _mm_dp_ps(a_vec, b_vec, 0xF1);
        ans_vec = _mm_add_ps(ans_vec, a_vec);
      }
      R[i * N + j] = ans_vec[0];
      for (; k < M; ++k)
      {
        R[i * N + j] += A[i * M + k] * B[j * M + k];
      }
    }
  }
}
