#include <stdio.h>
#include <stdlib.h>

int N;
int *A;
int *B;
int *R;

extern void summ();

extern void everyday795(int X, int Y);

int 
main()
{
	everyday795(3, 7);
	N=3;
	A=(int*)malloc(N * sizeof(int));
	B=(int*)malloc(N * sizeof(int));
	R=(int*)malloc(N * sizeof(int));
	A[0] = B[1] = A[2] = 40;
	B[0] = A[1] = B[2] = 4;
	summ();
	for (int i = 0; i < N; i++)
	{
		printf("\n->%d", R[i]);
	}
	return 0;
}
