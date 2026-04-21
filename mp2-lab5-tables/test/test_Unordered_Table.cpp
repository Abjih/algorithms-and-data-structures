#include <gtest.h>
#include <string>
#include <cmath>
#include "Polinoms.h"
#include "Unordered_Table.h"

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

TEST(UnorderedArrayTableTest, EmptyTable) {
    UnorderedArrayTable<Polynomial> table;
    EXPECT_EQ(table.getSize(), 0);
    EXPECT_EQ(table.search("key1"), nullptr);
}

TEST(UnorderedArrayTableTest, InsertAndSearch) {
    UnorderedArrayTable<Polynomial> table;
    Polynomial p1 = stringToPolynomial("2x^2 + 3y");
    Polynomial p2 = stringToPolynomial("5z^3");

    table.insert("poly1", p1);
    table.insert("poly2", p2);

    EXPECT_EQ(table.getSize(), 2);

    Polynomial* res1 = table.search("poly1");
    ASSERT_NE(res1, nullptr);
    EXPECT_TRUE(*res1 == p1);

    Polynomial* res2 = table.search("poly2");
    ASSERT_NE(res2, nullptr);
    EXPECT_TRUE(*res2 == p2);

    EXPECT_EQ(table.search("missing"), nullptr);
}

TEST(UnorderedArrayTableTest, UpdateExistingKey) {
    UnorderedArrayTable<Polynomial> table;
    Polynomial p1 = stringToPolynomial("x + y");
    Polynomial p2 = stringToPolynomial("2x + 2y");

    table.insert("A", p1);
    table.insert("A", p2);

    EXPECT_EQ(table.getSize(), 1);
    Polynomial* res = table.search("A");
    ASSERT_NE(res, nullptr);
    EXPECT_TRUE(*res == p2);
}

TEST(UnorderedArrayTableTest, RemoveElement) {
    UnorderedArrayTable<Polynomial> table;
    Polynomial px = stringToPolynomial("x");
    Polynomial py = stringToPolynomial("y");
    Polynomial pz = stringToPolynomial("z");

    table.insert("A", px);
    table.insert("B", py);
    table.insert("C", pz);

    table.remove("B");
    EXPECT_EQ(table.getSize(), 2);
    EXPECT_EQ(table.search("B"), nullptr);
    EXPECT_NE(table.search("A"), nullptr);
    EXPECT_NE(table.search("C"), nullptr);

    table.remove("missing");
    EXPECT_EQ(table.getSize(), 2);
}

TEST(UnorderedArrayTableTest, ResizeCapacity) {
    UnorderedArrayTable<Polynomial> table(2);

    for (int i = 0; i < 20; ++i) {
        Polynomial p = stringToPolynomial(std::to_string(i) + "x");
        table.insert("key" + std::to_string(i), p);
    }

    EXPECT_EQ(table.getSize(), 20);
    for (int i = 0; i < 20; ++i) {
        EXPECT_NE(table.search("key" + std::to_string(i)), nullptr);
    }
}

TEST(UnorderedArrayTableTest, CopySemantics) {
    UnorderedArrayTable<Polynomial> table1;
    Polynomial p1 = stringToPolynomial("x^2");
    Polynomial p2 = stringToPolynomial("y^2");

    table1.insert("A", p1);
    table1.insert("B", p2);

    UnorderedArrayTable<Polynomial> table2 = table1;
    EXPECT_EQ(table2.getSize(), 2);

    Polynomial p3 = stringToPolynomial("z^2");
    table2.insert("C", p3);
    table2.remove("A");

    EXPECT_EQ(table1.getSize(), 2);
    EXPECT_NE(table1.search("A"), nullptr);

    EXPECT_EQ(table2.getSize(), 2);
    EXPECT_EQ(table2.search("A"), nullptr);
    EXPECT_NE(table2.search("C"), nullptr);
}