#include <gtest.h>
#include <string>
#include <cmath>
#include "Polinoms.h"
#include "Hash_Table_Chains.h"

inline bool operator==(const Polynomial& lhs, const Polynomial& rhs) {
    auto it1 = lhs.list.begin();
    auto it2 = rhs.list.begin();
    while (it1 != lhs.list.end() && it2 != rhs.list.end()) {
        if ((*it1).getPower() != (*it2).getPower() || std::abs((*it1).coef - (*it2).coef) > 1e-9) {
            return false;
        }
        ++it1;
        ++it2;
    }
    return it1 == lhs.list.end() && it2 == rhs.list.end();
}

inline bool operator!=(const Polynomial& lhs, const Polynomial& rhs) {
    return !(lhs == rhs);
}

TEST(HashTableChainsTest, EmptyTable) {
    HashTableChains<Polynomial> table;
    EXPECT_EQ(table.getSize(), 0);
    EXPECT_EQ(table.search("Missing"), nullptr);
}

TEST(HashTableChainsTest, InsertAndSearch) {
    HashTableChains<Polynomial> table;
    Polynomial p1 = stringToPolynomial("2x^2 + 3y");
    Polynomial p2 = stringToPolynomial("5z^3");

    table.insert("A", p1);
    table.insert("B", p2);

    EXPECT_EQ(table.getSize(), 2);

    Polynomial* resA = table.search("A");
    ASSERT_NE(resA, nullptr);
    EXPECT_TRUE(*resA == p1);

    Polynomial* resB = table.search("B");
    ASSERT_NE(resB, nullptr);
    EXPECT_TRUE(*resB == p2);
}

TEST(HashTableChainsTest, UpdateExistingKey) {
    HashTableChains<Polynomial> table;
    Polynomial p1 = stringToPolynomial("x");
    Polynomial p2 = stringToPolynomial("10x");

    table.insert("Key", p1);
    table.insert("Key", p2);

    EXPECT_EQ(table.getSize(), 1);
    Polynomial* res = table.search("Key");
    ASSERT_NE(res, nullptr);
    EXPECT_TRUE(*res == p2);
}

TEST(HashTableChainsTest, CollisionHandling) {
    HashTableChains<Polynomial> table(3);

    for (int i = 0; i < 20; ++i) {
        std::string key = "key_" + std::to_string(i);
        Polynomial p = stringToPolynomial(std::to_string(i) + "x");
        table.insert(key, p);
    }

    EXPECT_EQ(table.getSize(), 20);
    for (int i = 0; i < 20; ++i) {
        std::string key = "key_" + std::to_string(i);
        Polynomial expected = stringToPolynomial(std::to_string(i) + "x");

        Polynomial* res = table.search(key);
        ASSERT_NE(res, nullptr);
        EXPECT_TRUE(*res == expected);
    }
}

TEST(HashTableChainsTest, RemoveElement) {
    HashTableChains<Polynomial> table;
    Polynomial px = stringToPolynomial("x");
    Polynomial py = stringToPolynomial("y");
    Polynomial pz = stringToPolynomial("z");

    table.insert("Alpha", px);
    table.insert("Beta", py);
    table.insert("Gamma", pz);

    table.remove("Beta");
    EXPECT_EQ(table.getSize(), 2);
    EXPECT_EQ(table.search("Beta"), nullptr);
    EXPECT_NE(table.search("Alpha"), nullptr);
    EXPECT_NE(table.search("Gamma"), nullptr);

    table.remove("MissingKey");
    EXPECT_EQ(table.getSize(), 2);
}

TEST(HashTableChainsTest, ClearTable) {
    HashTableChains<Polynomial> table;
    Polynomial px = stringToPolynomial("x");
    Polynomial py = stringToPolynomial("y");

    table.insert("A", px);
    table.insert("B", py);

    table.clear();
    EXPECT_EQ(table.getSize(), 0);
    EXPECT_EQ(table.search("A"), nullptr);
}

TEST(HashTableChainsTest, CopySemantics) {
    HashTableChains<Polynomial> table1;
    Polynomial p1 = stringToPolynomial("x^2");
    Polynomial p2 = stringToPolynomial("y^2");

    table1.insert("One", p1);
    table1.insert("Two", p2);

    HashTableChains<Polynomial> table2 = table1;
    EXPECT_EQ(table2.getSize(), 2);

    table2.remove("One");
    Polynomial p3 = stringToPolynomial("z^2");
    table2.insert("Three", p3);

    EXPECT_EQ(table1.getSize(), 2);
    EXPECT_NE(table1.search("One"), nullptr);

    EXPECT_EQ(table2.getSize(), 2);
    EXPECT_EQ(table2.search("One"), nullptr);
    EXPECT_NE(table2.search("Three"), nullptr);
}