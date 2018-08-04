/*
 *  В круг выстроено N человек, пронумерованных числами от 1 до N.
 *  Будем исключать каждого k-ого до тех пор, пока не уцелеет только один человек.
 *  Например, если N=10, k=3, то сначала умрет 3-й, потом 6-й, затем 9-й, затем 2-й, затем 7-й,
 *  потом 1-й, потом 8-й, за ним - 5-й, и потом 10-й. Таким образом, уцелеет 4-й.
 *  Необходимо определить номер уцелевшего.
 *  N, k ≤ 10000.
 */


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


int ComputeWinner (int n, int k);

int main () 
{
    int n = 0, k = 0;
    assert(scanf("%d %d", &n, &k));
    assert(n > 0);
    assert(k > 0);
    int winner = ComputeWinner(n, k);
    printf("%d", winner);
    return 0;
}

int ComputeWinner (int n, int k)
{
    int winner = 0;
    /*
     *  There is a regularity from Joseph's problem:
     *  J{n, k} = (J{(n - 1), k} + k - 1)% (n + 1)
     *  J{1, k} = 1
     *
     */
    for (int player = 1; player <= n; player++) 
    {
        winner = (winner + k) % (player);
    }
    return winner + 1;
}
