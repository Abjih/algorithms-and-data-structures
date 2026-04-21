#pragma once
#include <iostream>
#include <string>
#include "Polinoms.h"

template <typename T>
struct HashNode {
    std::string key;
    T value;
    HashNode* next;

    HashNode(const std::string& k, const T& v) : key(k), value(v), next(nullptr) {}
};

template <typename T>
class HashTableChains {
private:
    HashNode<T>** table;
    int bucketCount;
    int currentSize;

    int hashFunction(const std::string& key) const {
        unsigned long hash = 0;
        for (char c : key) {
            hash = hash * 31 + c;
        }
        return hash % bucketCount;
    }

public:
    HashTableChains(int buckets = 11) {
        bucketCount = buckets;
        currentSize = 0;
        table = new HashNode<T>*[bucketCount];
        for (int i = 0; i < bucketCount; i++) {
            table[i] = nullptr;
        }
    }

    ~HashTableChains() {
        clear();
        delete[] table;
    }

    void clear() {
        for (int i = 0; i < bucketCount; i++) {
            HashNode<T>* curr = table[i];
            while (curr) {
                HashNode<T>* temp = curr;
                curr = curr->next;
                delete temp;
            }
            table[i] = nullptr;
        }
        currentSize = 0;
    }

    HashTableChains(const HashTableChains& other) {
        bucketCount = other.bucketCount;
        currentSize = 0;
        table = new HashNode<T>*[bucketCount];
        for (int i = 0; i < bucketCount; i++) {
            table[i] = nullptr;
        }

        for (int i = 0; i < other.bucketCount; i++) {
            HashNode<T>* curr = other.table[i];
            while (curr) {
                insert(curr->key, curr->value);
                curr = curr->next;
            }
        }
    }

    HashTableChains& operator=(const HashTableChains& other) {
        if (this == &other) return *this;

        clear();
        delete[] table;

        bucketCount = other.bucketCount;
        table = new HashNode<T>*[bucketCount];
        for (int i = 0; i < bucketCount; i++) {
            table[i] = nullptr;
        }

        for (int i = 0; i < other.bucketCount; i++) {
            HashNode<T>* curr = other.table[i];
            while (curr) {
                insert(curr->key, curr->value);
                curr = curr->next;
            }
        }
        return *this;
    }

    void insert(const std::string& key, const T& value) {
        int index = hashFunction(key);
        HashNode<T>* curr = table[index];

        while (curr) {
            if (curr->key == key) {
                curr->value = value;
                return;
            }
            curr = curr->next;
        }

        HashNode<T>* newNode = new HashNode<T>(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        currentSize++;
    }

    T* search(const std::string& key) const {
        int index = hashFunction(key);
        HashNode<T>* curr = table[index];

        while (curr) {
            if (curr->key == key) {
                return &curr->value;
            }
            curr = curr->next;
        }
        return nullptr;
    }

    void remove(const std::string& key) {
        int index = hashFunction(key);
        HashNode<T>* curr = table[index];
        HashNode<T>* prev = nullptr;

        while (curr) {
            if (curr->key == key) {
                if (prev == nullptr) {
                    table[index] = curr->next;
                }
                else {
                    prev->next = curr->next;
                }
                delete curr;
                currentSize--;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }

    void printAll() const {
        for (int i = 0; i < bucketCount; i++) {
            if (table[i] != nullptr) {
                std::cout << "[§¬§à§â§Ù§Ú§ß§Ñ " << i << "]: ";
                HashNode<T>* curr = table[i];
                while (curr) {
                    std::cout << "{" << curr->key << " = ";
                    curr->value.print();
                    std::cout << "} ";
                    curr = curr->next;
                }
            }
        }
    }

    int getSize() const {
        return currentSize;
    }
};