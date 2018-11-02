#include <stdio.h>
#include <stdlib.h>

extern void 
vector_sum(int N, const int *A, const int *B, int *R);

extern double
calc(double A, double B, double C, int D);

int 
main()
{
	puts("\n\tVector sum:");
	int N;
    scanf("%d", &N);
	int* A_=(int*)malloc(N * sizeof(int));
	int* B_=(int*)malloc(N * sizeof(int));
	int* R_=(int*)malloc(N * sizeof(int));
    for (int i = 0; i < N; ++i)
    {
        scanf("%d", &A_[i]);
    }
    for (int i = 0; i < N; ++i)
    {
        scanf("%d", &B_[i]);
    }
    puts("");
    vector_sum(N, A_, B_, R_);
    for (int i = 0; i < N; ++i)
    {
        printf("%d ", R_[i]);
    }
    free(A_);
    free(B_);
    free(R_);
    puts("\n\tCalculate:");
    double A, B, C;
    int D;
    scanf("%lf %lf %lf %d", &A, &B, &C, &D);
    printf("calc(%lf,%lf,%lf,%d)=%lf", calc(A, B, C, D));
	return 0;
}
