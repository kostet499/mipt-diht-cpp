//
//  main.cpp
//  squares
//
//  Created by Michael on 17.09.17.
//  Copyright (c) 2017 mvcion. All rights reserved.
//


/*
 *   Вывести квадраты натуральных чисел от 1 до n,
 *   используя только O(n) операций сложения и
 *   вычитания (умножением пользоваться нельзя).
 *   n ≤ 1000.
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
    
    template<class A, class B, class C> class Triple {
    public:
        A first;
        B second;
        C third;
        Triple<A, B, C>() {
            this->first = (A)NULL;
            this->second = (B)NULL;
            this->third = (C)NULL;
        }
        Triple<A, B, C>(A a, B b, C c) {
            this->first = a;
            this->second = b;
            this->third = c;
        }
    };
    template<class A, class B, class C> Triple<A, B, C> make_triple(A a, B b, C c) {
        return Triple<A, B, C>(a, b, c);
    }
    
    template<class T> class ListArray {
    private:
        unsigned int size;
        float resize_factor;
        T* data_list;
    public:
        ListArray<T>(){
            this->size = 1;
            this->resize_factor = 2.0;
            this->data_list = (T*)malloc(this->size * sizeof(T));
        }
        ListArray<T>(unsigned int size){
            this->size = size;
            this->resize_factor = 2.0;
            this->data_list = (T*)malloc(this->size * sizeof(T));
        }
        ListArray<T>(unsigned int size, float resize_factor){
            this->size = size;
            this->data_list = (T*)malloc(this->size * sizeof(T));
            this->resize_factor = resize_factor;
        }
        void update(unsigned int size, T* source) {
            this->resize(size);
            functools::arrays_copy(this->data_list, source, size);
        }
        void resize(unsigned int size) {
            this->clear();
            this->size = size;
            this->data_list = (T*)malloc(this->size * sizeof(T));
        }
        unsigned int length(void) {
            return this->size;
        }
        T get(unsigned int index) {
            assert(index < this->size);
            return this->data_list[index];
        }
        void set(unsigned int index, T value) {
            assert(index < this->size);
            this->data_list[index] = value;
        }
        void fill(T value) {
            functools::array_fill(this->data_list, this->size, value);
        }
        void push_back(T value) {
            T* buffer = (T*)malloc(this->size * sizeof(T));
            functools::arrays_copy(buffer, this->data_list, this->size);
            this->resize((unsigned int)(this->size * this->resize_factor));
            functools::arrays_copy(this->data_list, buffer, this->size);
            free(buffer);
        }
        void reverse(int last_index) {
            functools::array_reverse(this->data_list, last_index);
        }
        void clear(void) {
            if (this->size != 0) {
                this->size = 0;
                free(this->data_list);
            }
        }
        ~ListArray(){
            if (this->size != 0) {
                this->clear();
            }
        };
    };
    
    template<class T> class Stack {
    private:
        unsigned int size;
        float resize_factor;
        T* data_list;
        int ptr;
        bool empty;
    public:
        Stack<T>() {
            this->size = 42;
            this->resize_factor = 2.0;
            this->ptr = -1;
            this->empty = true;
            this->data_list = (T*)malloc(this->size * sizeof(T));
        }
        Stack<T>(unsigned int size) {
            this->resize_factor = 2.0;
            this->ptr = -1;
            this->empty = true;
            this->size = size;
            this->data_list = (T*)malloc(this->size * sizeof(T));
        }
        Stack<T>(unsigned int size, float resize_factor) {
            this->ptr = -1;
            this->empty = true;
            this->size = size;
            this->resize_factor = resize_factor;
            this->data_list = (T*)malloc(this->size * sizeof(T));
        }
        T top(void) {
            assert(this->ptr != -1);
            return this->data_list[this->ptr];
        }
        T bottom(void) {
            assert(this->ptr != -1);
            return this->data_list[0];
        }
        void push(T value) {
            if (this->ptr == -1) {
                this->empty = false;
            }
            else if (this->ptr == (this->size - 1)) {
                T* buffer = (T*)malloc(this->size * sizeof(T));
                functools::arrays_copy(buffer, this->data_list, this->size);
                unsigned int prev_size = this->size;
                int prev_ptr = this->ptr;
                this->resize((unsigned int)(this->size * this->resize_factor));
                functools::arrays_copy(this->data_list, buffer, prev_size);
                this->ptr = prev_ptr;
                free(buffer);
            }
            this->data_list[++this->ptr] = value;
            return;
        }
        T pop(void) {
            assert(!this->empty);
            if (this->ptr == 0) {
                this->empty = true;
            }
            T cache = this->data_list[this->ptr--];
            return cache;
        }
        bool is_empty(void) {
            return this->empty;
        }
        unsigned int length(void) {
            return this->ptr + 1;
        }
        void resize(int size) {
            if (!this->empty) {
                this->clear();
            }
            this->size = size;
            this->data_list = (T*)malloc(this->size * sizeof(T));
        }
        void clear(void) {
            this->ptr = -1;
            this->size = 0;
            free(this->data_list);
        }
        ~Stack(void) {
            if (!this->empty && this->size > 0) {
                free(this->data_list);
            }
            this->ptr = -1;
            this->size = 0;
        }
    };
    
    template<class T> class Queue {
    private:
        int size;
        int head;
        int tail;
        bool empty;
        T* circular_buffer;
    public:
        Queue<T>() {
            this->size = 0;
            this->head = -1;
            this->tail = -1;
            this->empty = true;
            this->circular_buffer = NULL;
        }
        Queue<T>(int size) {
            this->empty = true;
            this->size = size;
            this->circular_buffer = (T*)malloc(this->size * sizeof(T));
            this->head = 0;
            this->tail = 0;
        }
        void resize(int size) {
            if (!this->empty) {
                this->clear();
                this->empty = true;
            }
            this->size = size;
            this->circular_buffer = (T*)malloc(this->size * sizeof(T));
            this->head = 0;
            this->tail = 0;
        }
        T pop_front(void) {
            T temp = this->circular_buffer[this->head];
            if (this->head == this->tail) {
                this->head = 0;
                this->tail = 0;
                this->empty = true;
            }
            else {
                this->head++;
                this->head %= this->size;
            }
            return temp;
        }
        T push_back(T value) {
            if (this->empty) {
                this->empty = false;
                this->circular_buffer[0] = value;
            }
            else {
                this->tail++;
                this->tail %= this->size;
                assert(!(this->tail == this->head));
                this->circular_buffer[this->tail] = value;
            }
        }
        bool is_empty(void) {
            return this->empty;
        }
        void clear(void) {
            free(this->circular_buffer);
            this->head = -1;
            this->tail = -1;
            this->size = 0;
            this->empty = true;
        }
        ~Queue() {
            if (!this->empty && this->size != 0) {
                free(this->circular_buffer);
            }
            this->head = -1;
            this->tail = -1;
        }
    };
    
    // Extra task #4*
    template<class T> class SQueue {
    private:
        Stack<T> head, tail;
    public:
        SQueue<T>() {
            this->head = Stack<T>();
            this->tail = Stack<T>();
        }
        SQueue<T>(unsigned int size) {
            this->head = Stack<T>(size);
            this->tail = Stack<T>(size);
        }
        bool is_empty(void) {
            return this->head.is_empty() || this->tail.is_empty();
        }
        void move_data(void) {
            while (!this->tail.is_empty()) {
                this->head.push(this->tail.pop());
            }
            return;
        }
        void push(T value) {
            this->tail.push(value);
            return;
        }
        T pop() {
            assert(!(this->head.is_empty() && this->tail.is_empty()));
            if (this->head.is_empty()) {
                this->move_data();
            }
            return this->head.pop();
        }
        T top() {
            assert(!(this->head.is_empty() && this->tail.is_empty()));
            if (this->head.is_empty()) {
                this->move_data();
            }
            return this->head.top();
        }
        void clear() {
            if (!this->head.is_empty()) {
                this->head.clear();
            }
            if (!this->tail.is_empty()) {
                this->tail.clear();
            }
            return;
        }
        ~SQueue() {
            this->clear();
        }
    };
    
    template<typename T> class Heap {
    private:
        unsigned int size;
        T* data_list;
        bool empty;
    public:
        Heap<T>(void) {
            this->empty = true;
            this->size = 0;
            this->data_list = (T*)malloc(1 * sizeof(T));
        }
        Heap<T>(unsigned int buffer_size) {
            this->empty = true;
            this->size = 0;
            this->data_list = (T*)malloc(buffer_size * sizeof(T));
        }
        void clear(void) {
            if (!this->empty) {
                this->empty = true;
                free(this->data_list);
            }
        }
        unsigned int get_size(void) { 
            return this->size;
        }
        bool add(T value) {
            this->data_list[++this->size] = value;
            this->check_up(this->size);
            if (this->empty) {
                this->empty = false;
            }
            return true;
        }
        bool extract_min(T *value) {
            assert(this->size != 0);
            *value = this->data_list[1];
            this->data_list[1] = this->data_list[this->size--];
            this->check_down(1);
            return true;
        }
        ~Heap() {
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
            if (this->data_list[previous_position] > this->data_list[current_position]) {
                functools::swap(this->data_list[previous_position], this->data_list[current_position]);
                this->check_up(previous_position);
                return;
            }
        }
        void check_down(int previous_position) {
            int current_position = 2 * previous_position;
            if (current_position > this->size) {
                return;
            }
            if (current_position + 1 <= this->size && this->data_list[current_position + 1] < this->data_list[current_position]) {
                current_position++;
            }
            if (this->data_list[current_position] < this->data_list[previous_position]) {
                functools::swap(this->data_list[current_position], this->data_list[previous_position]);
                this->check_down(current_position);
                return;
            }
        }
    };
    
    
    namespace bst {
        
        template<typename T> class Node {
        private:
            T value;
            Node<T> *left;
            Node<T> *right;
        public:
            Node<T>() {
                this->value = (T)NULL;
                this->left = NULL;
                this->right = NULL;
            }
            Node<T>(T value) {
                this->value = value;
                this->left = NULL;
                this->right = NULL;
            } 
            void clear() {
                // TO-DO
            }
            ~Node() {
                this->clear();
            }
            void set_value(T value) {
                this->value = value;
            }
            T get_value() {
                return this->value;
            }
            void set_left(Node *node) {
                this->left = node;
            }
            Node<T>* get_left() {
                return this->left;
            }
            void set_right(Node *node) {
                this->right = node;
            }
            Node<T>* get_right() {
                return this->right;
            }
        };
        
        template<typename T> class Tree {
        private:
            Node<T>* head;
        public:
            Tree<T>() {
                this->head = NULL;
            }
            Tree<T>(Node<T>* node) {
                this->head = node;
            }
            void clear() {
                // TO-DO
            }
            ~Tree() {
                this->clear();
            }
            Node<T>* get_head() {
                return this->head;
            }
            bool add(Node<T>* node, T value) {
                if (this->head == NULL) {
                    Node<T>* node = new Node<T>(value);
                    this->head = node;
                    return true;
                }
                else if (value >= node->get_value()) {
                    if (node->get_right() != NULL) {
                        return this->add(node->get_right(), value);
                    }
                    else {
                        Node<T>* new_node = new Node<T>(value);
                        node->set_right(new_node);
                        return true;
                    }
                } 
                else {
                    if (node->get_left() != NULL) {
                        return this->add(node->get_left(), value);
                    }
                    else {
                        Node<T>* new_node = new Node<T>(value);
                        node->set_left(new_node);
                        return true;
                    }
                }
                return false;
            }
            bool add(T value) {
                if (this->head == NULL) {
                    Node<T>* node = new Node<T>(value);
                    this->head = node;
                    return true;
                }
                if (value >= this->head->get_value()) {
                    if (this->head->get_right() != NULL) {
                        return this->add(this->head->get_right(), value);
                    }
                    else {
                        Node<T>* new_node = new Node<T>(value);
                        this->head->set_right(new_node);
                        return true;
                    }
                } 
                else {
                    if (this->head->get_left() != NULL) {
                        return this->add(this->head->get_left(), value);
                    }
                    else {
                        Node<T>* new_node = new Node<T>(value);
                        this->head->set_left(new_node);
                        return true;
                    }
                }
            }
        private:
            bool search(T value, Node<T>* node) {
                while (node != NULL) {
                    if (value == node->get_value()) {
                        return true;
                    } 
                    else {
                        if (value <= node->get_value()) {
                            return this->search(value, node->get_left());
                        }
                        else {
                            return this->search(value, node->get_right());
                        }
                    }
                }
                return false;
            }
            Node<T>* remove_node(Node<T>* root, T value) {
                Node<T>* new_node = new Node<T>();
                if (root == NULL) {
                    return NULL;
                }
                if (value == root->get_value()) {
                    if (root->get_left() == NULL) {
                        new_node = root->get_right();
                        free(root);
                        return new_node;
                    }
                    new_node = root->get_left();
                    while (new_node->get_right()) {
                        new_node = new_node->get_right();
                    }
                    new_node->set_right(root->get_right());
                    return root->get_left();
                }
                if (value < root->get_value()) {
                    root->set_left(this->remove_node(root->get_left(), value));
                }
                else {
                    root->set_right(this->remove_node(root->get_right(), value));
                }
                return root;
            }
            T get_min(Node<T>* node) {
                assert(node != NULL);
                if (node->get_left() == NULL) {
                    return node->get_value();
                }
                else {
                    return get_min(node->get_left());
                }
            }
            T get_max(Node<T>* node) {
                assert(node != NULL);
                if (node->get_right() == NULL) {
                    return node->get_value();
                }
                else {
                    return get_max(node->get_right());
                }
            }
        public:
            bool search(T value) {
                return this->search(value, this->head);
            }
            Node<T>* remove_node(T value) {
                return this->remove_node(this->get_head(), value);
            }
            T get_min() {
                return this->get_min(this->get_head());
            }
            T get_max() {
                return this->get_max(this->get_head());
            }
        };
    };
};


void get_squares(int* squares_list, int n);
int main(){
    int n = 0;
    assert(scanf("%d", &n));
    assert(n > 0);
    int* squares_list = (int*)malloc(n * sizeof(int));
    get_squares(squares_list, n);
    for (int i = 0; i < n; i++) {
        printf("%d ", squares_list[i]);
    }
    free(squares_list);
    return 0;
}
void get_squares(int* squares_list, int n) {
    squares_list[0] = 1;
    for (int i = 1; i < n; i++) {
        squares_list[i] = squares_list[i - 1] + i + i + 1;
    }
    return;
}

