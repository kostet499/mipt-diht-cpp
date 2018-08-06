/*
 *   Вывести квадраты натуральных чисел от 1 до n,
 *   используя только O(n) операций сложения и
 *   вычитания (умножением пользоваться нельзя).
 *   n ≤ 1000.
 */


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


void ComputeSquares (int* squares_list, int n);

int main ()
{
    int n = 0;
    assert(scanf("%d", &n));
    assert(n > 0);
    int* squares_list = (int*)malloc(n * sizeof(int));
    ComputeSquares(squares_list, n);
    for (int i = 0; i < n; i++)
    {
        printf("%d ", squares_list[i]);
    }
    free(squares_list);
    return 0;
}

void ComputeSquares (int* squares_list, int n) 
{
    squares_list[0] = 1;
    for (int i = 1; i < n; i++) 
    {
        squares_list[i] = squares_list[i - 1] + (i << 1) + 1;
    }
}

