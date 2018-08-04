/*
 * Дан массив целых чисел А[0..n-1].
 * Известно, что на интервале [0, m] значения массива
 * строго возрастают, а на интервале [m, n-1] строго убывают.
 * Найти m за O(log m). 2 ≤ n ≤ 10000.
 */


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


namespace functools 
{
    template<typename T> 
    T min (const T &arg0, const T &arg1)
    {
        if (arg0 > arg1)
        {
            return arg1;
        }
        else
        {
            return arg0;
        }
    }
    
    template<typename T> 
    T max (const T &arg0, const T &arg1) 
    {
        if (arg0 > arg1) 
        {
            return arg0;
        } 
        else 
        {
            return arg1;
        }
    }
};


using functools::min;
using functools::max;


template<typename T> 
int FindMaxElement (T* list, int n);


int main() 
{
    int n = 0;
    assert(scanf("%d", &n));
    int* list = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        assert(scanf("%d", &list[i]));
    }
    int m = FindMaxElement(list, n);
    printf("%d\n", m);
    free(list);
    return 0;
}



template<typename T> 
int ComputePosition (int from, int to, T* list, int n) 
{
    if (from == to)
    {
        // if max element is found
        return from;
    }
    else 
    {
        while (from != to) 
        {
            int middle = (from + to) / 2;
            if ((list[middle - 1] < list[middle]) && (list[middle] > list[middle + 1])) 
            {
                // if max element is found
                return middle;
            }
            else if ((list[middle - 1] < list[middle]) && (list[middle] < list[middle + 1]))
            {
                // if middle is in the part where subsequence increases
                from = middle;
            }
            else 
            {
                // if middle is in the part where subsequence decreases
                to = middle;
            }
        }
        return from;
    }
}


template<typename T> 
int FindMaxElement (T* list, int n)
{
    assert(n >= 2);
    if (list[0] > list[1]) 
    {
        // if the biggest element is on the head of the list
        return 0;
    }
    else if (list[n - 1] > list[n - 2]) 
    {
        // if the biggest element is on the tail of the list
        return n - 1;
    }
    else 
    {
        // if the biggest element is not on the begin or end of the list
        int index = 1;
        int from = -1, to = -1;
        while (index < n - 1)
        {
            if (list[index - 1] < list[index] && list[index] > list[index + 1]) 
            {
                // if max element is found
                return index;
            }
            if (list[index - 1] > list[index] && list[index] > list[index + 1]) 
            {
                // if the current subsequence decreases
                break;
            }
            index *= 2;
        }
        
        from = index / 2;
        to = min(index, n);
        return ComputePosition(from, to, list, n);
    }
}
