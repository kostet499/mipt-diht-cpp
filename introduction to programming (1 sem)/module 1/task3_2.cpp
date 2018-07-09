//
//  main.cpp
//  unimodal
//
//  Created by Michael on 17.09.17.
//  Copyright (c) 2017 mvcion. All rights reserved.
//


/*
 * Дан массив целых чисел А[0..n-1].
 * Известно, что на интервале [0, m] значения массива
 * строго возрастают, а на интервале [m, n-1] строго убывают.
 * Найти m за O(log m). 2 ≤ n ≤ 10000.
 */


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


namespace functools {
    template<typename T> T min(const T &arg0, const T &arg1) {
        if (arg0 > arg1) {
            return arg1;
        }
        else {
            return arg0;
        }
    }
    template<typename T> T max(const T &arg0, const T &arg1) {
        if (arg0 > arg1) {
            return arg0;
        } else {
            return arg1;
        }
    }
    template<typename T> void swap(T &arg0, T &arg1) {
        T temp = arg0;
        arg0 = arg1;
        arg1 = temp;
        return;
    }
    template<typename T> void array_fill(T* array, int array_size, T value) {
        for (int i = 0; i < array_size; i++) {
            array[i] = value;
        }
        return;
    }
    template<typename T> void array_reverse(T* array, int array_size) {
        for (int i = 0; i < (array_size) / 2; i++) {
            functools::swap(array[i], array[array_size - i - 1]);
        }
        return;
    }
    template<typename T> void arrays_copy(T* dist, T* source, int array_size) {
        for (int i = 0; i < array_size; i++) {
            dist[i] = source[i];
        }
        return;
    }
};


using functools::min;
using functools::max;

template<typename T> int find_max_element(T* list, int n);


int main() {
    int n = 0;
    assert(scanf("%d", &n));
    int* list = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        assert(scanf("%d", &list[i]));
    }
    int m = find_max_element(list, n);
    printf("%d\n", m);
    free(list);
    return 0;
}



template<typename T> int get_position(int from, int to, T* list, int n) {
    if (from == to) {
        // if max element is found
        return from;
    }
    else {
        while (from != to) {
            int middle = (from + to) / 2;
            if ((list[middle - 1] < list[middle]) && (list[middle] > list[middle + 1])) {
                // if max element is found
                return middle;
            }
            else if ((list[middle - 1] < list[middle]) && (list[middle] < list[middle + 1])) {
                // if middle is in the part where subsequence increases
                from = middle;
            }
            else {
                // if middle is in the part where subsequence decreases
                to = middle;
            }
        }
        return from;
    }
}


template<typename T> int find_max_element(T* list, int n) {
    assert(n >= 2);
    if (list[0] > list[1]) {
        // if the biggest element is on the head of the list
        return 0;
    }
    else if (list[n - 1] > list[n - 2]) {
        // if the biggest element is on the tail of the list
        return n - 1;
    }
    else {
        // if the biggest element is not on the begin or end of the list
        int index = 1;
        int from = -1, to = -1;
        
        while (index < n - 1) {
            if (list[index - 1] < list[index] && list[index] > list[index + 1]) {
                // if max element is found
                return index;
            }
            if (list[index - 1] > list[index] && list[index] > list[index + 1]) {
                // if the current subsequence decreases
                break;
            }
            index *= 2;
        }
        
        from = index / 2;
        to = min(index, n);
        return get_position(from, to, list, n);
    }
}
