#pragma once
#include <iostream>
#include <string>
#include "Polinoms.h"

template <typename T>
struct TableEntry {
    std::string key;
    T value;

    TableEntry() {}
    TableEntry(const std::string& k, const T& v) : key(k), value(v) {}
};

template <typename T>
class UnorderedArrayTable {
private:
    TableEntry<T>* data;
    int capacity;
    int currentSize;

    void resize() {
        capacity *= 2;
        TableEntry<T>* newData = new TableEntry<T>[capacity];
        for (int i = 0; i < currentSize; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    UnorderedArrayTable(int startCapacity = 10) {
        capacity = startCapacity;
        currentSize = 0;
        data = new TableEntry<T>[capacity];
    }

    ~UnorderedArrayTable() {
        delete[] data;
    }

    UnorderedArrayTable(const UnorderedArrayTable& other) {
        capacity = other.capacity;
        currentSize = other.currentSize;
        data = new TableEntry<T>[capacity];
        for (int i = 0; i < currentSize; i++) {
            data[i] = other.data[i];
        }
    }

    UnorderedArrayTable& operator=(const UnorderedArrayTable& other) {
        if (this == &other) return *this;
        delete[] data;
        capacity = other.capacity;
        currentSize = other.currentSize;
        data = new TableEntry<T>[capacity];
        for (int i = 0; i < currentSize; i++) {
            data[i] = other.data[i];
        }
        return *this;
    }

    void insert(const std::string& key, const T& val) {
        T* existing = search(key);
        if (existing) {
            *existing = val;
            return;
        }

        if (currentSize == capacity) {
            resize();
        }

        data[currentSize] = TableEntry<T>(key, val);
        currentSize++;
    }

    T* search(const std::string& key) const {
        for (int i = 0; i < currentSize; i++) {
            if (data[i].key == key) {
                return &data[i].value;
            }
        }
        return nullptr;
    }

    void remove(const std::string& key) {
        for (int i = 0; i < currentSize; i++) {
            if (data[i].key == key) {
                data[i] = data[currentSize - 1];
                currentSize--;
                return;
            }
        }
    }

    void printAll() const {
        for (int i = 0; i < currentSize; i++) {
            std::cout << data[i].key << " = ";
            data[i].value.print();
        }
    }

    int getSize() const {
        return currentSize;
    }
};