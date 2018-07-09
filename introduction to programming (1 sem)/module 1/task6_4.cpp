//
//  main.cpp
//  winmax
//
//  Created by Michael on 11.09.17.
//  Copyright (c) 2017 mvcion. All rights reserved.
//


/*
 * Дан массив натуральных чисел A[0..n), n не превосходит 100 000 000.
 * Так же задан размер некотрого окна (последовательно расположенных элементов массива)
 * в этом массиве k, k<=n. Требуется для каждого положения окна (от 0 и до n-k)
 * вывести значение максимума в окне. Скорость работы O(n log n), память O(k).
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


namespace collections {
        
    template<class A, class B> class Pair {
    public:
        A first;
        B second;
        Pair<A, B>() {
            this->first = (A)NULL;
            this->second = (B)NULL;
        }
        Pair<A, B>(A a, B b) {
            this->first = a;
            this->second = b;
        }
    };
    template<class A, class B> Pair<A, B> make_pair(A a, B b) {
        return Pair<A, B>(a, b);
    }
    
};


class WinMaxHeap {
private:
    int size;
    collections::Pair<long long, int>* data_list;
    int* ptrs;
    bool empty;
public:
    WinMaxHeap(void) {
        this->empty = true;
        this->size = 0;
        this->data_list = (collections::Pair<long long, int>*)malloc(1 * sizeof(collections::Pair<long long, int>));
        this->ptrs = (int*)malloc(1 * sizeof(int));
    }
    WinMaxHeap(unsigned int buffer_size) {
        this->empty = true;
        this->size = 0;
        this->data_list = (collections::Pair<long long, int>*)malloc(buffer_size * sizeof(collections::Pair<long long, int>));
        this->ptrs = (int*)malloc(buffer_size * sizeof(int));
    }
    void clear(void) {
        if (!this->empty) {
            this->empty = true;
            free(this->data_list);
            free(this->ptrs);
        }
    }
    unsigned int get_size(void) { 
        return this->size + 1;
    }
    bool add(long long element, int index) {
        this->data_list[++this->size] = collections::Pair<long long, int>(element, index);
        this->ptrs[index] = this->size;
        this->check_up(this->size);
        if (this->empty) {
            this->empty = false;
        }
        return true;
    }
    long long get_min() {
        return this->data_list[1].first;
    }
    long long extract_min() {
        long long value = this->data_list[1].first;
        this->data_list[1] = this->data_list[this->size--];
        this->ptrs[this->data_list[1].second] = 1;
        this->check_down(1);
        return value;
    }
    void link(int index) {
        this->data_list[this->ptrs[index]].first = -1 * 1LL << 60;
        this->check_up(this->ptrs[index]);
        this->extract_min();
    }
    ~WinMaxHeap() {
        if (!this->empty) {
            this->clear();
        }
    }
private: 
    void check_up(int current_position) {
        int previous_position = current_position / 2;
        if (previous_position == 0) {
            return;
        }
        if (this->data_list[previous_position].first > this->data_list[current_position].first) {
            functools::swap(this->data_list[previous_position], this->data_list[current_position]);
            functools::swap(this->ptrs[this->data_list[current_position].second], 
                            this->ptrs[this->data_list[previous_position].second]);
            this->check_up(previous_position);
            return;
        }
    }
    void check_down(int previous_position) {
        int current_position = 2 * previous_position;
        if (current_position > this->size) {
            return;
        }
        if (current_position + 1 <= this->size && this->data_list[current_position + 1].first < this->data_list[current_position].first) {
            current_position++;
        }
        if (this->data_list[current_position].first < this->data_list[previous_position].first) {
            functools::swap(this->data_list[current_position], this->data_list[previous_position]);
            functools::swap(this->ptrs[this->data_list[current_position].second], 
                            this->ptrs[this->data_list[previous_position].second]);
            this->check_down(current_position);
            return;
        }
    }
};


template<typename T> void get_sliding_maxs(int n, int k, T* array, T* total_sliding_maxs);
int main() {
    int n = 0, k = 0;
    assert(scanf("%d", &n));
    if (n == 0) {
        return 0;
    }
    long long* array = (long long*)malloc(n * sizeof(long long));
    for (int index = 0; index < n; index++) {
        assert(scanf("%lld", &array[index]));
    }
    assert(scanf("%d", &k));
    if (k == 0) {
        free(array);
        return 0;
    }
    long long* total_sliding_maxs = (long long*)malloc(n * sizeof(long long));
    get_sliding_maxs(n, k, array, total_sliding_maxs);
    for (int index = k - 1; index < n; index++) {
        printf("%lld ", total_sliding_maxs[index]);
    }
    printf("\n");
    free(array);
    free(total_sliding_maxs);
    return 0;
}
template<typename T> void get_sliding_maxs(int n, int k, T* array, T* total_sliding_maxs) {
    if (k == 1) {
        functools::arrays_copy(total_sliding_maxs, array, n);
        return;
    }
    else {
        WinMaxHeap heap(3 * k);
        for (int index = 0; index < n; index++) {
            if (index >= k) {
                heap.link(index - k);
            }
            heap.add(-array[index], index);
            total_sliding_maxs[index] = -heap.get_min();
        }
        return;
    }
}

