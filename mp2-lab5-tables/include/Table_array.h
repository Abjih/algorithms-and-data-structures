#pragma once
#include <iostream>
#include <string>
#include "Polinoms.h"

#ifndef TABLE_ENTRY_DEFINED
#define TABLE_ENTRY_DEFINED
template <typename T>
struct TableEntry {
    std::string key;
    T value;

    TableEntry() {}
    TableEntry(const std::string& k, const T& v) : key(k), value(v) {}
};
#endif

template <typename T>
class OrderedArrayTable {
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

    int binarySearch(const std::string& key, int& pos) const {
        int left = 0, right = currentSize - 1;
        pos = 0;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (data[mid].key == key) {
                pos = mid;
                return mid;
            }
            if (data[mid].key < key) {
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }
        pos = left;
        return -1;
    }

public:
    OrderedArrayTable(int startCapacity = 10) {
        capacity = startCapacity;
        currentSize = 0;
        data = new TableEntry<T>[capacity];
    }

    ~OrderedArrayTable() {
        delete[] data;
    }

    OrderedArrayTable(const OrderedArrayTable& other) {
        capacity = other.capacity;
        currentSize = other.currentSize;
        data = new TableEntry<T>[capacity];
        for (int i = 0; i < currentSize; i++) {
            data[i] = other.data[i];
        }
    }

    OrderedArrayTable& operator=(const OrderedArrayTable& other) {
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

    void insert(const std::string& key, const T& poly) {
        int pos = 0;
        int found = binarySearch(key, pos);
        if (found != -1) {
            data[found].value = poly;
            return;
        }
        if (currentSize == capacity) {
            resize();
        }
        for (int i = currentSize; i > pos; i--) {
            data[i] = data[i - 1];
        }
        data[pos] = TableEntry<T>(key, poly);
        currentSize++;
    }

    T* search(const std::string& key) const {
        int pos = 0;
        int found = binarySearch(key, pos);
        if (found != -1) {
            return &data[found].value;
        }
        return nullptr;
    }

    void remove(const std::string& key) {
        int pos = 0;
        int found = binarySearch(key, pos);
        if (found == -1)
            return;
        for (int i = found; i < currentSize - 1; i++) {
            data[i] = data[i + 1];
        }
        currentSize--;
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

    const TableEntry<T>& getEntry(int index) const {
        return data[index];
    }
};