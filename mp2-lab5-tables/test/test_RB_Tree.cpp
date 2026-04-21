#include <gtest.h>
#include <vector>
#include <string>
#include <cmath>
#include "Polinoms.h"
#include "RB_Tree.h"

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

inline bool operator<(const Polynomial& lhs, const Polynomial& rhs) {
    auto it1 = lhs.list.begin();
    auto it2 = rhs.list.begin();
    while (it1 != lhs.list.end() && it2 != rhs.list.end()) {
        if ((*it1).getPower() != (*it2).getPower()) {
            return (*it1).getPower() < (*it2).getPower();
        }
        if (std::abs((*it1).coef - (*it2).coef) > 1e-9) {
            return (*it1).coef < (*it2).coef;
        }
        ++it1;
        ++it2;
    }
    return it1 == lhs.list.end() && it2 != rhs.list.end();
}

TEST(PolynomialTest, EqualityOperator) {
    Polynomial p1 = stringToPolynomial("2x^2 + 3y^2 - 4z^3");
    Polynomial p2 = stringToPolynomial("-4z^3 + 2x^2 + 3y^2");
    Polynomial p3 = stringToPolynomial("2x^2 + 3y^2");

    EXPECT_TRUE(p1 == p2);
    EXPECT_FALSE(p1 == p3);
}

TEST(PolynomialTest, LessThanOperator) {
    EXPECT_TRUE(stringToPolynomial("x") < stringToPolynomial("x^2"));
    EXPECT_TRUE(stringToPolynomial("x^2") < stringToPolynomial("2x^2"));
    EXPECT_FALSE(stringToPolynomial("2x^2") < stringToPolynomial("x^2"));
}

TEST(PolynomialTest, ZeroAndEmpty) {
    EXPECT_TRUE(stringToPolynomial("") == stringToPolynomial("0"));
    EXPECT_TRUE(stringToPolynomial("0x") == stringToPolynomial("0"));
    EXPECT_TRUE(stringToPolynomial("x - x") == stringToPolynomial("0"));
}

TEST(PolynomialTest, ComplexSorting) {
    Polynomial p1 = stringToPolynomial("x^2 + y");
    Polynomial p2 = stringToPolynomial("x^2 + 2y");
    Polynomial p3 = stringToPolynomial("x^3");

    EXPECT_TRUE(p1 < p2);
    EXPECT_TRUE(p2 < p3);
    EXPECT_TRUE(p1 < p3);
}

TEST(PolynomialTest, ArithmeticInteractions) {
    Polynomial p1 = stringToPolynomial("x^2");
    Polynomial p2 = stringToPolynomial("y^2");
    Polynomial p3 = stringToPolynomial("x^2 + y^2");
    Polynomial p4 = stringToPolynomial("x^2y^2");
    Polynomial p5 = stringToPolynomial("x^2 - x^2");

    EXPECT_TRUE((p1 + p2) == p3);
    EXPECT_TRUE((p1 * p2) == p4);
    EXPECT_TRUE(p5 == stringToPolynomial("0"));
}

TEST(RBTreeTest, EmptyTree) {
    RBTree<Polynomial> tree;

    EXPECT_EQ(tree.size(), 0);
    EXPECT_FALSE(tree.contains(stringToPolynomial("x")));
    EXPECT_TRUE(tree.toVector().empty());
}

TEST(RBTreeTest, InsertAndContains) {
    RBTree<Polynomial> tree;

    tree.insert(stringToPolynomial("5x^3 + y"));
    tree.insert(stringToPolynomial("10z^4"));

    EXPECT_TRUE(tree.contains(stringToPolynomial("5x^3 + y")));
    EXPECT_TRUE(tree.contains(stringToPolynomial("10z^4")));
    EXPECT_FALSE(tree.contains(stringToPolynomial("x")));
}

TEST(RBTreeTest, InOrderTraversalIsSorted) {
    RBTree<Polynomial> tree;

    tree.insert(stringToPolynomial("5x^3"));
    tree.insert(stringToPolynomial("x"));
    tree.insert(stringToPolynomial("3x^2"));
    tree.insert(stringToPolynomial("x^2"));
    tree.insert(stringToPolynomial("10x^4"));

    std::vector<Polynomial> sorted = tree.toVector();

    ASSERT_EQ(sorted.size(), 5);
    for (size_t i = 0; i < sorted.size() - 1; ++i) {
        EXPECT_TRUE(sorted[i] < sorted[i + 1]);
    }
}

TEST(RBTreeTest, InsertDescendingOrder) {
    RBTree<Polynomial> tree;

    for (int i = 50; i > 0; --i) {
        tree.insert(stringToPolynomial(std::to_string(i) + "x"));
    }

    std::vector<Polynomial> result = tree.toVector();

    ASSERT_EQ(result.size(), 50);
    for (size_t i = 0; i < result.size() - 1; ++i) {
        EXPECT_TRUE(result[i] < result[i + 1]);
    }
}

TEST(RBTreeTest, InsertZigZagOrder) {
    RBTree<Polynomial> tree;
    std::vector<int> values = { 10, 30, 20, 50, 40, 70, 60 };

    for (int v : values) {
        tree.insert(stringToPolynomial(std::to_string(v) + "x"));
    }

    std::vector<Polynomial> result = tree.toVector();

    ASSERT_EQ(result.size(), 7);
    for (size_t i = 0; i < result.size() - 1; ++i) {
        EXPECT_TRUE(result[i] < result[i + 1]);
    }
}

TEST(RBTreeTest, NegativeCoefficients) {
    RBTree<Polynomial> tree;

    tree.insert(stringToPolynomial("-5x"));
    tree.insert(stringToPolynomial("5x"));
    tree.insert(stringToPolynomial("-10x"));
    tree.insert(stringToPolynomial("10x"));

    std::vector<Polynomial> result = tree.toVector();

    ASSERT_EQ(result.size(), 4);
    EXPECT_TRUE(result[0] == stringToPolynomial("-10x"));
    EXPECT_TRUE(result[1] == stringToPolynomial("-5x"));
    EXPECT_TRUE(result[2] == stringToPolynomial("5x"));
    EXPECT_TRUE(result[3] == stringToPolynomial("10x"));
}

TEST(RBTreeTest, StressTestRotations) {
    RBTree<Polynomial> tree;

    for (int i = 1; i <= 200; ++i) {
        tree.insert(stringToPolynomial(std::to_string(i) + "x"));
    }

    std::vector<Polynomial> result = tree.toVector();

    ASSERT_EQ(result.size(), 200);
    for (size_t i = 0; i < result.size() - 1; ++i) {
        EXPECT_TRUE(result[i] < result[i + 1]);
    }
}

TEST(RBTreeTest, HandlesDuplicatesGracefully) {
    RBTree<Polynomial> tree;
    Polynomial p = stringToPolynomial("2x^2 + 3y");

    tree.insert(p);
    tree.insert(p);
    tree.insert(p);

    EXPECT_EQ(tree.size(), 1);
    EXPECT_TRUE(tree.contains(p));
}