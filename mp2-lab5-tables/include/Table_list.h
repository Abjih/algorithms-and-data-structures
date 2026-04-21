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
struct ListTableNode {
    TableEntry<T> entry;
    ListTableNode* next;

    ListTableNode() : next(nullptr) {}
    ListTableNode(const std::string& k, const T& v, ListTableNode* nxt = nullptr)
        : entry(k, v), next(nxt) {
    }
};

template <typename T>
class OrderedListTable {
private:
    ListTableNode<T>* dummy;
    int currentSize;

public:
    OrderedListTable() {
        dummy = new ListTableNode<T>();
        currentSize = 0;
    }

    ~OrderedListTable() {
        clear();
        delete dummy;
    }

    OrderedListTable(const OrderedListTable& other) {
        dummy = new ListTableNode<T>();
        currentSize = 0;

        ListTableNode<T>* tail = dummy;
        ListTableNode<T>* curr = other.dummy->next;
        while (curr) {
            tail->next = new ListTableNode<T>(curr->entry.key, curr->entry.value);
            tail = tail->next;
            currentSize++;
            curr = curr->next;
        }
    }

    OrderedListTable& operator=(const OrderedListTable& other) {
        if (this == &other) return *this;
        clear();

        ListTableNode<T>* tail = dummy;
        ListTableNode<T>* curr = other.dummy->next;
        while (curr) {
            tail->next = new ListTableNode<T>(curr->entry.key, curr->entry.value);
            tail = tail->next;
            currentSize++;
            curr = curr->next;
        }
        return *this;
    }

    void clear() {
        ListTableNode<T>* curr = dummy->next;
        while (curr) {
            ListTableNode<T>* temp = curr;
            curr = curr->next;
            delete temp;
        }
        dummy->next = nullptr;
        currentSize = 0;
    }

    void insert(const std::string& key, const T& poly) {
        ListTableNode<T>* prev = dummy;
        ListTableNode<T>* curr = dummy->next;
        while (curr && curr->entry.key < key) {
            prev = curr;
            curr = curr->next;
        }
        if (curr && curr->entry.key == key) {
            curr->entry.value = poly;
            return;
        }
        ListTableNode<T>* newNode = new ListTableNode<T>(key, poly, curr);
        prev->next = newNode;
        currentSize++;
    }

    T* search(const std::string& key) const {
        ListTableNode<T>* curr = dummy->next;
        while (curr) {
            if (curr->entry.key == key) {
                return &curr->entry.value;
            }
            if (curr->entry.key > key) {
                return nullptr;
            }
            curr = curr->next;
        }
        return nullptr;
    }

    void remove(const std::string& key) {
        ListTableNode<T>* prev = dummy;
        ListTableNode<T>* curr = dummy->next;

        while (curr) {
            if (curr->entry.key == key) {
                prev->next = curr->next;
                delete curr;
                currentSize--;
                return;
            }
            if (curr->entry.key > key) {
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }

    void printAll() const {
        ListTableNode<T>* curr = dummy->next;
        while (curr) {
            std::cout << curr->entry.key << " = ";
            curr->entry.value.print();
            curr = curr->next;
        }
    }

    int getSize() const {
        return currentSize;
    }
};