#pragma once
#include <iostream>
#include <vector>

enum Color { RED, BLACK };

template <typename T>
struct RBNode {
    T data;
    Color color;
    RBNode* left;
    RBNode* right;
    RBNode* parent;

    RBNode(T val) : data(val), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

template <typename T>
class RBTree {
private:
    RBNode<T>* root;
    int nodeCount;

    void rotateLeft(RBNode<T>* pt) {
        RBNode<T>* pt_right = pt->right;
        pt->right = pt_right->left;

        if (pt->right != nullptr)
            pt->right->parent = pt;

        pt_right->parent = pt->parent;

        if (pt->parent == nullptr)
            root = pt_right;
        else if (pt == pt->parent->left)
            pt->parent->left = pt_right;
        else
            pt->parent->right = pt_right;

        pt_right->left = pt;
        pt->parent = pt_right;
    }

    void rotateRight(RBNode<T>* pt) {
        RBNode<T>* pt_left = pt->left;
        pt->left = pt_left->right;

        if (pt->left != nullptr)
            pt->left->parent = pt;

        pt_left->parent = pt->parent;

        if (pt->parent == nullptr)
            root = pt_left;
        else if (pt == pt->parent->left)
            pt->parent->left = pt_left;
        else
            pt->parent->right = pt_left;

        pt_left->right = pt;
        pt->parent = pt_left;
    }

    void fixViolation(RBNode<T>* pt) {
        RBNode<T>* parent_pt = nullptr;
        RBNode<T>* grand_parent_pt = nullptr;

        while ((pt != root) && (pt->color != BLACK) && (pt->parent->color == RED)) {
            parent_pt = pt->parent;
            grand_parent_pt = pt->parent->parent;

            if (parent_pt == grand_parent_pt->left) {
                RBNode<T>* uncle_pt = grand_parent_pt->right;

                if (uncle_pt != nullptr && uncle_pt->color == RED) {
                    grand_parent_pt->color = RED;
                    parent_pt->color = BLACK;
                    uncle_pt->color = BLACK;
                    pt = grand_parent_pt;
                }
                else {
                    if (pt == parent_pt->right) {
                        rotateLeft(parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }
                    rotateRight(grand_parent_pt);
                    std::swap(parent_pt->color, grand_parent_pt->color);
                    pt = parent_pt;
                }
            }
            else {
                RBNode<T>* uncle_pt = grand_parent_pt->left;

                if (uncle_pt != nullptr && uncle_pt->color == RED) {
                    grand_parent_pt->color = RED;
                    parent_pt->color = BLACK;
                    uncle_pt->color = BLACK;
                    pt = grand_parent_pt;
                }
                else {
                    if (pt == parent_pt->left) {
                        rotateRight(parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }
                    rotateLeft(grand_parent_pt);
                    std::swap(parent_pt->color, grand_parent_pt->color);
                    pt = parent_pt;
                }
            }
        }
        root->color = BLACK;
    }

    void inorderHelper(RBNode<T>* node) const {
        if (node == nullptr) return;
        inorderHelper(node->left);
        node->data.print();
        inorderHelper(node->right);
    }

    void toVectorHelper(RBNode<T>* node, std::vector<T>& vec) const {
        if (node == nullptr) return;
        toVectorHelper(node->left, vec);
        vec.push_back(node->data);
        toVectorHelper(node->right, vec);
    }

    void clearHelper(RBNode<T>* node) {
        if (node != nullptr) {
            clearHelper(node->left);
            clearHelper(node->right);
            delete node;
        }
    }

public:
    RBTree() : root(nullptr), nodeCount(0) {}

    ~RBTree() {
        clearHelper(root);
    }

    void insert(const T& data) {
        RBNode<T>* pt = new RBNode<T>(data);
        RBNode<T>* parent = nullptr;
        RBNode<T>* current = root;

        while (current != nullptr) {
            parent = current;
            if (pt->data == current->data) {
                delete pt;
                return;
            }
            else if (pt->data < current->data) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        pt->parent = parent;

        if (parent == nullptr) {
            root = pt;
        }
        else if (pt->data < parent->data) {
            parent->left = pt;
        }
        else {
            parent->right = pt;
        }

        nodeCount++;
        fixViolation(pt);
    }

    bool contains(const T& data) const {
        RBNode<T>* current = root;
        while (current != nullptr) {
            if (data == current->data) {
                return true;
            }
            else if (data < current->data) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }
        return false;
    }

    int size() const {
        return nodeCount;
    }

    std::vector<T> toVector() const {
        std::vector<T> vec;
        toVectorHelper(root, vec);
        return vec;
    }

    void printInOrder() const {
        std::cout << "--- §±§à§Ý§Ú§ß§à§Þ§í §Ó §Õ§Ö§â§Ö§Ó§Ö (§Ó§à§Ù§â§Ñ§ã§ä§Ñ§ß§Ú§Ö) ---" << std::endl;
        inorderHelper(root);
        std::cout << "---------------------------------------" << std::endl;
    }
};