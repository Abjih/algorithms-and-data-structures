#pragma once

#include <iostream>
#include <cmath>
#include <string>
#include <cctype>

using namespace std;

struct Monom {
    double coef;
    int x, y, z;

    Monom(double c = 0, int _x = 0, int _y = 0, int _z = 0) : coef(c), x(_x), y(_y), z(_z) {}

    int getPower() const {
        return x * 100 + y * 10 + z;
    }

    bool isHigherThan(const Monom& other) const {
        return getPower() > other.getPower();
    }

    bool samePowers(const Monom& other) const {
        return getPower() == other.getPower();
    }

    bool operator==(const Monom& other) const {
        return (samePowers(other) && (abs(coef - other.coef) < 1e-9));
    }

    bool operator!=(const Monom& other) const {
        return !(*this == other);
    }
};

template <typename T>
struct Node {
    T data;
    Node* next;
    Node(T m) : data(m), next(nullptr) {}
};

template <typename T>
class List {
public:
    Node<T>* head;

    List() : head(nullptr) {}

    ~List() {
        clear();
    }

    void clear() {
        while (head) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }

    List(const List& other) : head(nullptr) {
        if (!other.head) return;
        head = new Node<T>(other.head->data);
        Node<T>* curr = head;
        Node<T>* othercurr = other.head->next;
        while (othercurr) {
            curr->next = new Node<T>(othercurr->data);
            curr = curr->next;
            othercurr = othercurr->next;
        }
    }

    List& operator=(const List& other) {
        if (this == &other) return *this;
        clear();
        if (!other.head) return *this;
        head = new Node<T>(other.head->data);
        Node<T>* curr = head;
        Node<T>* othercurr = other.head->next;
        while (othercurr) {
            curr->next = new Node<T>(othercurr->data);
            curr = curr->next;
            othercurr = othercurr->next;
        }
        return *this;
    }

    void insert(int index, T data) {
        if (index < 0) return;
        Node<T>* newNode = new Node<T>(data);

        if (index == 0) {
            newNode->next = head;
            head = newNode;
            return;
        }

        Node<T>* curr = head;
        for (int i = 0; i < index - 1 && curr; i++) {
            curr = curr->next;
        }

        if (!curr) {
            delete newNode;
            return;
        }

        newNode->next = curr->next;
        curr->next = newNode;
    }

    void removeAt(int index) {
        if (index < 0 || !head) return;

        if (index == 0) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        Node<T>* curr = head;
        for (int i = 0; i < index - 1 && curr->next; i++) {
            curr = curr->next;
        }

        if (!curr->next) return;

        Node<T>* temp = curr->next;
        curr->next = temp->next;
        delete temp;
    }

    class Iterator {
        Node<T>* ptr;
    public:
        Iterator(Node<T>* p) : ptr(p) {}
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
        bool operator==(const Iterator& other) const { return ptr == other.ptr; }
        Iterator& operator++() {
            if (ptr) ptr = ptr->next;
            return *this;
        }
        T& operator*() { return ptr->data; }
        Node<T>* getNode() { return ptr; }
    };

    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }

    Iterator search(const T& val) {
        for (Iterator it = begin(); it != end(); ++it) {
            if (*it == val) return it;
        }
        return end();
    }

    void push_back(T val) {
        if (!head) {
            head = new Node<T>(val);
            return;
        }
        Node<T>* curr = head;
        while (curr->next)
            curr = curr->next;
        curr->next = new Node<T>(val);
    }
};

class Polynomial {
public:
    List<Monom> list;

    Polynomial() {}

    void addSorted(Monom m) {
        if (abs(m.coef) < 1e-9) return;
        int index = 0;
        auto it = list.begin();
        while (it != list.end()) {
            Monom& currMonom = *it;

            if (m.isHigherThan(currMonom)) {
                list.insert(index, m);
                return;
            }
            if (m.samePowers(currMonom)) {
                currMonom.coef += m.coef;
                if (abs(currMonom.coef) < 1e-9) {
                    list.removeAt(index);
                }
                return;
            }
            ++it;
            index++;
        }

        list.insert(index, m);
    }

    void add(double coef, int x, int y, int z) {
        if (x < 0 || x > 9 || y < 0 || y > 9 || z < 0 || z > 9) {
            return;
        }
        addSorted(Monom(coef, x, y, z));
    }

    Polynomial operator+(const Polynomial& other) const {
        Polynomial res;
        for (auto m : list) res.addSorted(m);
        for (auto m : other.list) res.addSorted(m);
        return res;
    }

    Polynomial operator-(const Polynomial& other) const {
        Polynomial res;
        for (auto m : list) res.addSorted(m);
        for (auto m : other.list) {
            Monom temp = m;
            temp.coef = -temp.coef;
            res.addSorted(temp);
        }
        return res;
    }

    Polynomial operator*(const Polynomial& other) const {
        Polynomial res;
        for (auto m1 : list) {
            for (auto m2 : other.list) {
                int nx = m1.x + m2.x;
                int ny = m1.y + m2.y;
                int nz = m1.z + m2.z;
                if (nx <= 9 && ny <= 9 && nz <= 9) {
                    res.addSorted(Monom(m1.coef * m2.coef, nx, ny, nz));
                }
            }
        }
        return res;
    }

    void print() {
        bool first = true;
        for (auto m : list) {
            if (!first && m.coef > 0) cout << " + ";
            if (m.coef < 0) cout << " - ";

            double c = abs(m.coef);
            if (c != 1.0 || (m.x == 0 && m.y == 0 && m.z == 0)) cout << c;

            if (m.x > 0) cout << "x" << (m.x > 1 ? "^" + to_string(m.x) : "");
            if (m.y > 0) cout << "y" << (m.y > 1 ? "^" + to_string(m.y) : "");
            if (m.z > 0) cout << "z" << (m.z > 1 ? "^" + to_string(m.z) : "");
            first = false;
        }
        if (first) cout << "0";
        cout << endl;
    }
};

inline Polynomial stringToPolynomial(string str) {
    Polynomial poly;
    int n = str.length();
    int i = 0;
    while (i < n) {
        while (i < n && isspace(str[i]))
            i++;
        if (i >= n) break;

        double sign = 1.0;
        if (str[i] == '-') {
            sign = -1.0;
            i++;
        }
        else if (str[i] == '+') {
            i++;
        }

        while (i < n && isspace(str[i]))
            i++;

        double coef = 1.0;
        if (i < n && (isdigit(str[i]) || str[i] == '.')) {
            size_t len;
            coef = stod(str.substr(i), &len);
            i += len;
        }
        coef *= sign;

        int px = 0, py = 0, pz = 0;

        while (i < n && (str[i] == 'x' || str[i] == 'y' || str[i] == 'z' || str[i] == '(' || str[i] == '*')) {
            if (str[i] == '*') {
                i++;
                continue;
            }

            if (str[i] == '(') {
                i++;
                int gx = 0, gy = 0, gz = 0;
                while (i < n && str[i] != ')') {
                    switch (str[i]) {
                    case 'x': gx = 1; break;
                    case 'y': gy = 1; break;
                    case 'z': gz = 1; break;
                    default: break;
                    }
                    i++;
                }
                if (i < n && str[i] == ')') i++;

                int groupPow = 1;
                if (i < n && str[i] == '^') {
                    i++;
                    if (i < n && isdigit(str[i])) {
                        size_t len;
                        groupPow = stoi(str.substr(i), &len);
                        i += len;
                    }
                }
                px += gx * groupPow;
                py += gy * groupPow;
                pz += gz * groupPow;
            }
            else {
                char var = str[i];
                i++;
                int pow = 1;
                if (i < n && str[i] == '^') {
                    i++;
                    if (i < n && isdigit(str[i])) {
                        size_t len;
                        pow = stoi(str.substr(i), &len);
                        i += len;
                    }
                }

                if (var == 'x') px += pow;
                else if (var == 'y') py += pow;
                else if (var == 'z') pz += pow;
            }
        }

        poly.add(coef, px, py, pz);
    }
    return poly;
}