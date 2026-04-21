#pragma once
#include <iostream>
#include <stdexcept> 

template <class T>
class Stack {
private:
    T* mem;
    int size;
    int p;

public:
    Stack() : mem(nullptr), size(0), p(0) {}

    Stack(int sz) : size(sz), p(0) {
        if (sz <= 0) {
            throw std::invalid_argument("Размер стека должен быть положительным");
        }
        mem = new T[sz];
    }

    ~Stack() {
        delete[] mem;
    }

    bool is_full() const {
        return p == size;
    }

    bool is_empty() const {
        return p == 0;
    }

    void push(const T& el) {
        if (is_full()) {
            throw std::out_of_range("Стек полон");
        }
        mem[p] = el;
        p++;
    }

    T pop() {
        if (is_empty()) {
            throw std::out_of_range("Стек пуст");
        }
        p--;
        return mem[p];
    }

    T top() const {
        if (is_empty()) {
            throw std::out_of_range("Стек пуст для просмотра элемента");
        }
        return mem[p - 1];
    }
};