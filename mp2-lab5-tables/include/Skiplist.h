#pragma once
#include <vector>
#include <cstdlib>

template <typename T>
struct SkipListNode {
    T value;
    std::vector<SkipListNode*> forward;

    SkipListNode(T val, int level) : value(val), forward(level + 1, nullptr) {}
};

template <typename T>
class SkipList {
private:
    int max_level;
    int current_level;
    SkipListNode<T>* header;

    int randomLevel() {
        int lvl = 0;
        while ((rand() % 2 == 0) && lvl < max_level) {
            lvl++;
        }
        return lvl;
    }

public:
    SkipList(int max_lvl = 16)
        : max_level(max_lvl), current_level(0) {
        header = new SkipListNode<T>(T(), max_level);
    }

    ~SkipList() {
        SkipListNode<T>* current = header;
        while (current != nullptr) {
            SkipListNode<T>* next = current->forward[0];
            delete current;
            current = next;
        }
    }

    void insert(T value) {
        std::vector<SkipListNode<T>*> update(max_level + 1, nullptr);
        SkipListNode<T>* current = header;

        for (int i = current_level; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->value < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current == nullptr || current->value != value) {
            int rlevel = randomLevel();

            if (rlevel > current_level) {
                for (int i = current_level + 1; i <= rlevel; i++) {
                    update[i] = header;
                }
                current_level = rlevel;
            }

            SkipListNode<T>* n = new SkipListNode<T>(value, rlevel);

            for (int i = 0; i <= rlevel; i++) {
                n->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = n;
            }
        }
    }

    bool search(T value) const {
        SkipListNode<T>* current = header;

        for (int i = current_level; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->value < value) {
                current = current->forward[i];
            }
        }

        current = current->forward[0];

        return current != nullptr && current->value == value;
    }

    void erase(T value) {
        std::vector<SkipListNode<T>*> update(max_level + 1, nullptr);
        SkipListNode<T>* current = header;

        for (int i = current_level; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->value < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current != nullptr && current->value == value) {
            for (int i = 0; i <= current_level; i++) {
                if (update[i]->forward[i] != current) {
                    break;
                }
                update[i]->forward[i] = current->forward[i];
            }

            delete current;

            while (current_level > 0 && header->forward[current_level] == nullptr) {
                current_level--;
            }
        }
    }
};