#pragma once
#include <iostream>
#include <string>
#include <functional>
#include "Polinoms.h"

enum EntryState { EMPTY, OCCUPIED, DELETED };

template <typename T>
struct HashEntryDouble {
    std::string key;
    T value;
    EntryState state;

    HashEntryDouble() : state(EMPTY) {}
    HashEntryDouble(const std::string& k, const T& v) : key(k), value(v), state(OCCUPIED) {}
};

template <typename T>
class HashTableDouble {
private:
    HashEntryDouble<T>* data;
    int capacity;
    int currentSize;
    int usedSize;

    size_t hash1(const std::string& key) const {
        return std::hash<std::string>{}(key) % capacity;
    }

    size_t hash2(const std::string& key) const {
        size_t h = std::hash<std::string>{}(key);
        size_t step = (h % (capacity - 1)) + 1;
        if (step % 2 == 0) {
            step++;
        }
        return step;
    }

    void resize() {
        int oldCapacity = capacity;
        capacity *= 2;
        HashEntryDouble<T>* oldData = data;

        data = new HashEntryDouble<T>[capacity];
        currentSize = 0;
        usedSize = 0;

        for (int i = 0; i < oldCapacity; i++) {
            if (oldData[i].state == OCCUPIED) {
                insert(oldData[i].key, oldData[i].value);
            }
        }
        delete[] oldData;
    }

public:
    HashTableDouble(int startCapacity = 16) {
        capacity = 1;
        while (capacity < startCapacity) {
            capacity *= 2;
        }
        currentSize = 0;
        usedSize = 0;
        data = new HashEntryDouble<T>[capacity];
    }

    ~HashTableDouble() {
        delete[] data;
    }

    HashTableDouble(const HashTableDouble& other) {
        capacity = other.capacity;
        currentSize = other.currentSize;
        usedSize = other.usedSize;
        data = new HashEntryDouble<T>[capacity];
        for (int i = 0; i < capacity; i++) {
            data[i] = other.data[i];
        }
    }

    HashTableDouble& operator=(const HashTableDouble& other) {
        if (this == &other) return *this;
        delete[] data;

        capacity = other.capacity;
        currentSize = other.currentSize;
        usedSize = other.usedSize;
        data = new HashEntryDouble<T>[capacity];
        for (int i = 0; i < capacity; i++) {
            data[i] = other.data[i];
        }
        return *this;
    }

    void insert(const std::string& key, const T& val) {
        if ((usedSize + 1) * 2 > capacity) {
            resize();
        }

        size_t idx = hash1(key);
        size_t step = hash2(key);
        int firstDeleted = -1;

        for (int i = 0; i < capacity; i++) {
            if (data[idx].state == EMPTY) {
                if (firstDeleted != -1) {
                    idx = firstDeleted;
                }
                else {
                    usedSize++;
                }
                data[idx].key = key;
                data[idx].value = val;
                data[idx].state = OCCUPIED;
                currentSize++;
                return;
            }
            else if (data[idx].state == OCCUPIED && data[idx].key == key) {
                data[idx].value = val;
                return;
            }
            else if (data[idx].state == DELETED && firstDeleted == -1) {
                firstDeleted = idx;
            }

            idx = (idx + step) % capacity;
        }
    }

    T* search(const std::string& key) const {
        size_t idx = hash1(key);
        size_t step = hash2(key);

        for (int i = 0; i < capacity; i++) {
            if (data[idx].state == EMPTY) {
                return nullptr;
            }
            else if (data[idx].state == OCCUPIED && data[idx].key == key) {
                return &data[idx].value;
            }
            idx = (idx + step) % capacity;
        }
        return nullptr;
    }

    void remove(const std::string& key) {
        size_t idx = hash1(key);
        size_t step = hash2(key);

        for (int i = 0; i < capacity; i++) {
            if (data[idx].state == EMPTY) {
                return;
            }
            else if (data[idx].state == OCCUPIED && data[idx].key == key) {
                data[idx].state = DELETED;
                currentSize--;
                return;
            }
            idx = (idx + step) % capacity;
        }
    }

    void clear() {
        for (int i = 0; i < capacity; i++) {
            data[i].state = EMPTY;
        }
        currentSize = 0;
        usedSize = 0;
    }

    void printAll() const {
        for (int i = 0; i < capacity; i++) {
            if (data[i].state == OCCUPIED) {
                std::cout << data[i].key << " = ";
                data[i].value.print();
            }
        }
    }

    int getSize() const {
        return currentSize;
    }
};