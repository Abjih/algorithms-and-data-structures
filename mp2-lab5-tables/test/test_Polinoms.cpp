#include <gtest.h>
#include "polinoms.h" 

TEST(MonomTest, check_power_hash_calculation) {
    Monom m(1.0, 1, 2, 3);
    EXPECT_EQ(123, m.getPower());
}

TEST(MonomTest, check_comparison_logic) {
    Monom m1(1.0, 2, 0, 0);
    Monom m2(1.0, 1, 9, 9);

    EXPECT_TRUE(m1.isHigherThan(m2));
    EXPECT_FALSE(m2.isHigherThan(m1));
}

TEST(MonomTest, check_same_powers) {
    Monom m1(5.0, 1, 2, 3);
    Monom m2(10.0, 1, 2, 3);
    Monom m3(5.0, 1, 2, 4);

    EXPECT_TRUE(m1.samePowers(m2));
    EXPECT_FALSE(m1.samePowers(m3));
}

TEST(ListTest, generic_insert_and_order) {
    List<int> list;
    list.insert(0, 10);
    list.insert(1, 20);
    list.insert(0, 5);

    auto it = list.begin();
    EXPECT_EQ(5, *it);
    ++it;
    EXPECT_EQ(10, *it);
    ++it;
    EXPECT_EQ(20, *it);
}

TEST(ListTest, generic_remove_at) {
    List<int> list;
    list.insert(0, 10);
    list.insert(1, 20);
    list.insert(2, 30);

    list.removeAt(1);

    auto it = list.begin();
    EXPECT_EQ(10, *it);
    ++it;
    EXPECT_EQ(30, *it);
}

TEST(ListTest, generic_search) {
    List<int> list;
    list.insert(0, 10);
    list.insert(1, 20);

    auto it = list.search(20);
    EXPECT_EQ(20, *it);

    auto itEnd = list.search(99);
    EXPECT_EQ(itEnd, list.end());
}

TEST(ListTest, copy_constructor_deep_copy) {
    List<int> list1;
    list1.insert(0, 10);

    List<int> list2(list1);
    list1.insert(1, 20);

    auto it1 = list1.begin();
    ++it1;
    EXPECT_EQ(20, *it1);

    auto it2 = list2.begin();
    ++it2;
    EXPECT_EQ(it2, list2.end());
}

TEST(PolynomialTest, can_add_sorted_elements) {
    Polynomial p;
    p.addSorted(Monom(1, 1, 0, 0));
    p.addSorted(Monom(1, 3, 0, 0));
    p.addSorted(Monom(1, 2, 0, 0));

    auto it = p.list.begin();

    EXPECT_EQ(300, (*it).getPower());
    ++it;
    EXPECT_EQ(200, (*it).getPower());
    ++it;
    EXPECT_EQ(100, (*it).getPower());
}

TEST(PolynomialTest, merges_coefficients_with_same_powers) {
    Polynomial p;
    p.addSorted(Monom(2.0, 1, 1, 1));
    p.addSorted(Monom(3.0, 1, 1, 1));

    auto it = p.list.begin();
    EXPECT_DOUBLE_EQ(5.0, (*it).coef);

    ++it;
    EXPECT_EQ(it, p.list.end());
}

TEST(PolynomialTest, removes_node_when_sum_is_zero) {
    Polynomial p;
    p.addSorted(Monom(5.0, 2, 2, 2));
    p.addSorted(Monom(-5.0, 2, 2, 2));

    EXPECT_EQ(p.list.begin(), p.list.end());
    EXPECT_EQ(nullptr, p.list.head);
}

TEST(PolynomialTest, ignores_invalid_degrees) {
    Polynomial p;
    p.add(1.0, 10, 0, 0);
    p.add(1.0, 0, 0, -1);

    EXPECT_EQ(p.list.head, nullptr);

    p.add(1.0, 9, 9, 9);
    EXPECT_NE(p.list.head, nullptr);
}

TEST(PolynomialTest, addition_works_correctly) {
    Polynomial p1, p2;
    p1.add(3.0, 2, 1, 0);
    p2.add(1.0, 2, 1, 0);
    p2.add(5.0, 0, 0, 1);

    Polynomial sum = p1 + p2;

    auto it = sum.list.begin();
    EXPECT_DOUBLE_EQ(4.0, (*it).coef);
    EXPECT_EQ(210, (*it).getPower());
    ++it;
    EXPECT_DOUBLE_EQ(5.0, (*it).coef);
    EXPECT_EQ(1, (*it).getPower());
}

TEST(PolynomialTest, subtraction_results_in_zero) {
    Polynomial p1;
    p1.add(2.5, 1, 1, 1);
    Polynomial p2;
    p2.add(2.5, 1, 1, 1);

    Polynomial res = p1 - p2;

    EXPECT_EQ(res.list.head, nullptr);
}

TEST(PolynomialTest, multiplication_simple) {
    Polynomial p1;
    p1.add(1.0, 1, 0, 0);
    Polynomial p2;
    p2.add(1.0, 1, 0, 0);

    Polynomial res = p1 * p2;

    auto it = res.list.begin();
    EXPECT_DOUBLE_EQ(1.0, (*it).coef);
    EXPECT_EQ(200, (*it).getPower());
}

TEST(PolynomialTest, multiplication_ignores_overflow) {
    Polynomial p1; p1.add(1.0, 9, 0, 0);
    Polynomial p2; p2.add(1.0, 1, 0, 0);

    Polynomial res = p1 * p2;
    EXPECT_EQ(res.list.head, nullptr);
}

TEST(PolynomialTest, parser_simple) {
    string input = "3.5x^2y^1 + 2z";
    Polynomial p = stringToPolynomial(input);

    auto it = p.list.begin();
    EXPECT_DOUBLE_EQ(3.5, (*it).coef);
    EXPECT_EQ(210, (*it).getPower());

    ++it;
    EXPECT_DOUBLE_EQ(2.0, (*it).coef);
    EXPECT_EQ(1, (*it).getPower());
}

TEST(PolynomialTest, parser_with_group_power) {
    std::string input = "1(xyz)^2";
    Polynomial p = stringToPolynomial(input);

    auto it = p.list.begin();
    EXPECT_EQ(222, (*it).getPower());
}

TEST(PolynomialTest, parser_mixed_operations) {
    string input = "5x - 2x";
    Polynomial p = stringToPolynomial(input);

    auto it = p.list.begin();
    EXPECT_DOUBLE_EQ(3.0, (*it).coef);
    EXPECT_EQ(100, (*it).getPower());
}
TEST(PolynomialTest, parser_group_full) {
    string input = "(xyz)^2";
    Polynomial p = stringToPolynomial(input);
    auto it = p.list.begin();

    EXPECT_EQ(222, (*it).getPower());
}

TEST(PolynomialTest, parser_group_partial) {
    string input = "(xz)^3";
    Polynomial p = stringToPolynomial(input);
    auto it = p.list.begin();

    EXPECT_EQ(303, (*it).getPower());
}

TEST(PolynomialTest, parser_group_repeated_vars) {
    string input = "x^2 + (y)^3";
    Polynomial p = stringToPolynomial(input);
    auto it = p.list.begin();
    EXPECT_EQ(200, (*it).getPower());
    ++it;
    EXPECT_EQ(30, (*it).getPower());
}