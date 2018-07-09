//
//  main.cpp
//  queue
//
//  Created by Michael on 17.09.17.
//  Copyright (c) 2017 mvcion. All rights reserved.
//


/*
 * Реализовать очередь с динамическим зацикленным буфером.
 * Обрабатывать команды push back и pop front.
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
    
    struct Query {
        int command;
        int value;
    };
    
    template<class T> class Queue {
    private:
        
        int size;
        int head;
        int tail;
        bool empty;
        T* circular_buffer;
    
    	void resize(void);
        
    public:
        
        Queue<T>(int new_size = 32);
        ~Queue();

        T pop_front(void);
        void push_back(T value);
    
        bool is_empty(void);
    };
    
    // Constructor
    template<class T> Queue<T>::Queue(int new_size) {
        assert(new_size > 1);
        empty = true;
        size = new_size;
        circular_buffer = new T[size];
        head = 0;
        tail = 0;
    }
    
    // Destructor
    template<class T> Queue<T>::~Queue() {
        delete circular_buffer;
        head = -1;
        tail = -1;
        size = 0;
        empty = true;
    }
    
    // Resize function
    template<class T> void Queue<T>::resize(void) {
 		int* temp_buffer = new int[size * 2];
		for (int i = head; i != (size + tail + 1); i++) {
			temp_buffer[i - head] = circular_buffer[i % size];
		}
		delete[] circular_buffer;
		circular_buffer = temp_buffer;
        head = 0;
        tail = size - 1;
        size = size * 2;
    }
    
    // Pop front function
    template<class T> T Queue<T>::pop_front(void) {
        assert(!empty);
        T temp = circular_buffer[head];
        if (head == tail) {
            head = 0;
            tail = 0;
            empty = true;
        }
        else {
            head++;
            head %= size;
        }
        return temp;
    }
    
    // Push back function
    template<class T> void Queue<T>::push_back(T value) {
        if (empty) {
            empty = false;
            circular_buffer[0] = value;
            head = tail = 0;
        }
        else { 
            if (((tail + 1) % size) == head) {
                resize();
            	circular_buffer[++tail] = value;
            }
       	    else {
            	tail = (tail + 1) % size;
            	circular_buffer[tail] = value;
        	}
        }
    }
    
    // Check if queue is empty
    template<class T> bool Queue<T>::is_empty(void) {
        return empty;
    }
    
};


using collections::Query;
using collections::Queue;

void unit_test(Queue<int>* queue, Query &query, bool& is_valid);



int main() {
    int n = 0;
    assert(scanf("%d", &n));
    Queue<int>* queue = new Queue<int>(8);
    bool is_valid = true;
    Query query;
    for (int iteration = 0; iteration < n; iteration++) {
        assert(scanf("%d %d", &query.command, &query.value));
        assert((query.command == 2) || (query.command == 3));
        unit_test(queue, query, is_valid);
    }
    puts(is_valid ? "YES" : "NO");
    delete queue;
    return 0;
}



void unit_test(Queue<int>* queue, Query &query, bool& is_valid) {
    switch(query.command) {
        case 2:
            if (queue->is_empty()) {
                is_valid &= (query.value == -1);
            }
            else {
                is_valid &= (query.value == queue->pop_front());
            }
            break;
        case 3:
            queue->push_back(query.value);
            break;
	}
}


