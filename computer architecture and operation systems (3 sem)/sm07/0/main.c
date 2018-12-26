#include <stdio.h>
#include <stdlib.h>

extern void 
vector_sum(int N, const float *A, const float *B, float *R);

extern double
calc(double A, double B, double C, int D);

int 
main()
{
	puts("\n\tVector sum:");
	int N;
    scanf("%d", &N);
	float* A_=(float*)malloc(N * sizeof(float));
	float* B_=(float*)malloc(N * sizeof(float));
	float* R_=(float*)malloc(N * sizeof(float));
    for (int i = 0; i < N; ++i)
    {
        scanf("%f", &A_[i]);
    }
    for (int i = 0; i < N; ++i)
    {
        scanf("%f", &B_[i]);
    }
    puts("");
    vector_sum(N, A_, B_, R_);
    for (int i = 0; i < N; ++i)
    {
        printf("%f ", R_[i]);
    }
    free(A_);
    free(B_);
    free(R_);
    puts("\n\tCalculate:");
    double A, B, C;
    int D;
    scanf("%lf %lf %lf %d", &A, &B, &C, &D);
    printf("calc(%lf,%lf,%lf,%d)=%lf\n", A, B, C, D, calc(A, B, C, D));
	return 0;
}
