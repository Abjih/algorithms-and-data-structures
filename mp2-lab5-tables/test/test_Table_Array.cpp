#include <gtest.h>
#include <string>
#include <cmath>
#include "Polinoms.h"
#include "Table_array.h"

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

TEST(OrderedArrayTableTest, EmptyTable) {
    OrderedArrayTable<Polynomial> table;
    EXPECT_EQ(table.getSize(), 0);
    EXPECT_EQ(table.search("X"), nullptr);
}

TEST(OrderedArrayTableTest, InsertAndSearch) {
    OrderedArrayTable<Polynomial> table;
    Polynomial px = stringToPolynomial("x");
    Polynomial py = stringToPolynomial("y");
    Polynomial pz = stringToPolynomial("z");

    table.insert("M", px);
    table.insert("A", py);
    table.insert("Z", pz);

    EXPECT_EQ(table.getSize(), 3);
    EXPECT_NE(table.search("A"), nullptr);
    EXPECT_NE(table.search("M"), nullptr);
    EXPECT_NE(table.search("Z"), nullptr);
}

TEST(OrderedArrayTableTest, UpdateExistingKey) {
    OrderedArrayTable<Polynomial> table;
    Polynomial p1 = stringToPolynomial("x^2");
    Polynomial p2 = stringToPolynomial("10x^2");

    table.insert("Key", p1);
    table.insert("Key", p2);

    EXPECT_EQ(table.getSize(), 1);
    Polynomial* res = table.search("Key");
    ASSERT_NE(res, nullptr);
    EXPECT_TRUE(*res == p2);
}

TEST(OrderedArrayTableTest, MaintainsOrder) {
    OrderedArrayTable<Polynomial> table;
    Polynomial p1 = stringToPolynomial("z");
    Polynomial p2 = stringToPolynomial("x");
    Polynomial p3 = stringToPolynomial("x^2");
    Polynomial p4 = stringToPolynomial("y");

    table.insert("C", p1);
    table.insert("A", p2);
    table.insert("D", p3);
    table.insert("B", p4);

    ASSERT_EQ(table.getSize(), 4);
    EXPECT_EQ(table.getEntry(0).key, "A");
    EXPECT_EQ(table.getEntry(1).key, "B");
    EXPECT_EQ(table.getEntry(2).key, "C");
    EXPECT_EQ(table.getEntry(3).key, "D");
}

TEST(OrderedArrayTableTest, RemoveElement) {
    OrderedArrayTable<Polynomial> table;
    Polynomial p1 = stringToPolynomial("1");
    Polynomial p2 = stringToPolynomial("2");
    Polynomial p3 = stringToPolynomial("3");

    table.insert("A", p1);
    table.insert("B", p2);
    table.insert("C", p3);

    table.remove("B");
    EXPECT_EQ(table.getSize(), 2);
    EXPECT_EQ(table.search("B"), nullptr);

    EXPECT_EQ(table.getEntry(0).key, "A");
    EXPECT_EQ(table.getEntry(1).key, "C");
}

TEST(OrderedArrayTableTest, ResizeCapacity) {
    OrderedArrayTable<Polynomial> table(3);

    for (int i = 99; i >= 0; --i) {
        std::string key = (i < 10 ? "0" : "") + std::to_string(i);
        Polynomial p = stringToPolynomial(std::to_string(i) + "x");
        table.insert(key, p);
    }

    EXPECT_EQ(table.getSize(), 100);
    EXPECT_EQ(table.getEntry(0).key, "00");
    EXPECT_EQ(table.getEntry(99).key, "99");
}

TEST(OrderedArrayTableTest, CopySemantics) {
    OrderedArrayTable<Polynomial> t1;
    Polynomial px = stringToPolynomial("x");
    Polynomial py = stringToPolynomial("y");
    t1.insert("X", px);
    t1.insert("Y", py);

    OrderedArrayTable<Polynomial> t2(t1);
    t2.remove("X");

    Polynomial pz = stringToPolynomial("z");
    t2.insert("Z", pz);

    EXPECT_EQ(t1.getSize(), 2);
    EXPECT_NE(t1.search("X"), nullptr);

    EXPECT_EQ(t2.getSize(), 2);
    EXPECT_EQ(t2.search("X"), nullptr);
    EXPECT_NE(t2.search("Z"), nullptr);
}