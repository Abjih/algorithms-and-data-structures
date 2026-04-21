#include <gtest.h>
#include <string>
#include <cmath>
#include "Polinoms.h"
#include "Table_list.h"

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

TEST(OrderedListTableTest, EmptyTable) {
    OrderedListTable<Polynomial> table;
    EXPECT_EQ(table.getSize(), 0);
    EXPECT_EQ(table.search("key"), nullptr);
}

TEST(OrderedListTableTest, InsertAndSearch) {
    OrderedListTable<Polynomial> table;
    Polynomial p1 = stringToPolynomial("2x");
    Polynomial p2 = stringToPolynomial("x");
    Polynomial p3 = stringToPolynomial("3x");

    table.insert("B", p1);
    table.insert("A", p2);
    table.insert("C", p3);

    EXPECT_EQ(table.getSize(), 3);
    EXPECT_NE(table.search("A"), nullptr);
    EXPECT_NE(table.search("B"), nullptr);
    EXPECT_NE(table.search("C"), nullptr);
    EXPECT_EQ(table.search("D"), nullptr);
}

TEST(OrderedListTableTest, UpdateExistingKey) {
    OrderedListTable<Polynomial> table;
    Polynomial p1 = stringToPolynomial("x^3");
    Polynomial p2 = stringToPolynomial("y^3");

    table.insert("Key1", p1);
    table.insert("Key1", p2);

    EXPECT_EQ(table.getSize(), 1);
    Polynomial* p = table.search("Key1");
    ASSERT_NE(p, nullptr);
    EXPECT_TRUE(*p == p2);
}

TEST(OrderedListTableTest, RemoveElement) {
    OrderedListTable<Polynomial> table;
    Polynomial px = stringToPolynomial("x");
    Polynomial py = stringToPolynomial("y");
    Polynomial pz = stringToPolynomial("z");

    table.insert("A", px);
    table.insert("B", py);
    table.insert("C", pz);

    table.remove("A");
    EXPECT_EQ(table.getSize(), 2);
    EXPECT_EQ(table.search("A"), nullptr);
    EXPECT_NE(table.search("B"), nullptr);
    EXPECT_NE(table.search("C"), nullptr);

    table.remove("C");
    EXPECT_EQ(table.getSize(), 1);
    EXPECT_EQ(table.search("C"), nullptr);

    table.remove("Missing");
    EXPECT_EQ(table.getSize(), 1);
}

TEST(OrderedListTableTest, ClearTable) {
    OrderedListTable<Polynomial> table;
    Polynomial px = stringToPolynomial("x");
    Polynomial py = stringToPolynomial("y");

    table.insert("A", px);
    table.insert("B", py);

    EXPECT_EQ(table.getSize(), 2);
    table.clear();
    EXPECT_EQ(table.getSize(), 0);
    EXPECT_EQ(table.search("A"), nullptr);
}

TEST(OrderedListTableTest, CopySemantics) {
    OrderedListTable<Polynomial> t1;
    Polynomial px = stringToPolynomial("x");
    Polynomial py = stringToPolynomial("y");

    t1.insert("One", px);
    t1.insert("Two", py);

    OrderedListTable<Polynomial> t2;
    t2 = t1;
    EXPECT_EQ(t2.getSize(), 2);

    t2.remove("One");

    Polynomial pz = stringToPolynomial("z");
    t2.insert("Three", pz);

    EXPECT_EQ(t1.getSize(), 2);
    EXPECT_NE(t1.search("One"), nullptr);

    EXPECT_EQ(t2.getSize(), 2);
    EXPECT_EQ(t2.search("One"), nullptr);
    EXPECT_NE(t2.search("Three"), nullptr);
}