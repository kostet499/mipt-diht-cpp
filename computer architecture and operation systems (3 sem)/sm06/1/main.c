#include <stdio.h>
#include <stdlib.h>


extern void summ(int N, const int *A, const int *B, int *R);

int 
main()
{
	int N=0;
    scanf("%d", &N);
	int* A=(int*)malloc(N * sizeof(int));
	int* B=(int*)malloc(N * sizeof(int));
	int* R=(int*)malloc(N * sizeof(int));
    for (int i = 0; i < N; ++i)
    {
        scanf("%d", &A[i]);
    }
    for (int i = 0; i < N; ++i)
    {
        scanf("%d", &B[i]);
    }
    puts("");
    summ(N, A, B, R);
    for (int i = 0; i < N; ++i)
    {
        printf("%d ", R[i]);
    }
    free(A);
    free(B);
    free(R);
    return 0;
}
