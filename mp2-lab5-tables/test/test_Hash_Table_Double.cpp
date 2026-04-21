#include <gtest.h>
#include <string>
#include <cmath>
#include "Polinoms.h"
#include "Hash_Table_Double.h"

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

TEST(HashTableDoubleTest, EmptyTable) {
    HashTableDouble<Polynomial> table;
    EXPECT_EQ(table.getSize(), 0);
    EXPECT_EQ(table.search("Missing"), nullptr);
}

TEST(HashTableDoubleTest, InsertAndSearch) {
    HashTableDouble<Polynomial> table;
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

TEST(HashTableDoubleTest, UpdateExistingKey) {
    HashTableDouble<Polynomial> table;
    Polynomial p1 = stringToPolynomial("x");
    Polynomial p2 = stringToPolynomial("10x");

    table.insert("Key", p1);
    table.insert("Key", p2);

    EXPECT_EQ(table.getSize(), 1);
    Polynomial* res = table.search("Key");
    ASSERT_NE(res, nullptr);
    EXPECT_TRUE(*res == p2);
}

TEST(HashTableDoubleTest, CollisionAndResizeHandling) {
    HashTableDouble<Polynomial> table(4);

    for (int i = 0; i < 50; ++i) {
        std::string key = "key_" + std::to_string(i);
        Polynomial p = stringToPolynomial(std::to_string(i) + "x");
        table.insert(key, p);
    }

    EXPECT_EQ(table.getSize(), 50);

    for (int i = 0; i < 50; ++i) {
        std::string key = "key_" + std::to_string(i);
        Polynomial expected = stringToPolynomial(std::to_string(i) + "x");

        Polynomial* res = table.search(key);
        ASSERT_NE(res, nullptr);
        EXPECT_TRUE(*res == expected);
    }
}

TEST(HashTableDoubleTest, RemoveElementAndTombstones) {
    HashTableDouble<Polynomial> table;
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

    Polynomial pNew = stringToPolynomial("2z");
    table.insert("Delta", pNew);

    EXPECT_EQ(table.getSize(), 3);
    EXPECT_NE(table.search("Delta"), nullptr);

    table.remove("MissingKey");
    EXPECT_EQ(table.getSize(), 3);
}

TEST(HashTableDoubleTest, ClearTable) {
    HashTableDouble<Polynomial> table;
    table.insert("A", stringToPolynomial("x"));
    table.insert("B", stringToPolynomial("y"));

    table.clear();
    EXPECT_EQ(table.getSize(), 0);
    EXPECT_EQ(table.search("A"), nullptr);
    EXPECT_EQ(table.search("B"), nullptr);
}

TEST(HashTableDoubleTest, CopySemantics) {
    HashTableDouble<Polynomial> table1;
    Polynomial p1 = stringToPolynomial("x^2");
    Polynomial p2 = stringToPolynomial("y^2");

    table1.insert("One", p1);
    table1.insert("Two", p2);

    HashTableDouble<Polynomial> table2 = table1;
    EXPECT_EQ(table2.getSize(), 2);

    table2.remove("One");
    Polynomial p3 = stringToPolynomial("z^2");
    table2.insert("Three", p3);

    EXPECT_EQ(table1.getSize(), 2);
    EXPECT_NE(table1.search("One"), nullptr);
    EXPECT_EQ(table1.search("Three"), nullptr);

    EXPECT_EQ(table2.getSize(), 2);
    EXPECT_EQ(table2.search("One"), nullptr);
    EXPECT_NE(table2.search("Three"), nullptr);

    HashTableDouble<Polynomial> table3;
    table3 = table1;
    EXPECT_EQ(table3.getSize(), 2);
    EXPECT_NE(table3.search("Two"), nullptr);
}