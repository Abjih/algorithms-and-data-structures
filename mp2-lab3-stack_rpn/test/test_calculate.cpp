// test_calculate.cpp
#include "calculate.h" // §£§Ñ§ê §Ù§Ñ§Ô§à§Ý§à§Ó§à§é§ß§í§Û §æ§Ñ§Û§Ý §Õ§Ý§ñ §Ü§Ñ§Ý§î§Ü§å§Ý§ñ§ä§à§â§Ñ
#include <gtest.h>     // §±§à§Õ§Ü§Ý§ð§é§Ö§ß§Ú§Ö Google Test
#include <cmath>       // §¥§Ý§ñ §Þ§Ñ§ä§Ö§Þ§Ñ§ä§Ú§é§Ö§ã§Ü§Ú§ç §Ü§à§ß§ã§ä§Ñ§ß§ä §Ú §æ§å§ß§Ü§è§Ú§Û

TEST(Calculator, can_evaluate_simple_addition)
{
    Calculator calc;
    EXPECT_DOUBLE_EQ(8.0, calc.evaluate("5 3 + "));
}

TEST(Calculator, can_evaluate_expression_with_precedence)
{
    Calculator calc;
    // §³§à§à§ä§Ó§Ö§ä§ã§ä§Ó§å§Ö§ä "5 + 3 * 2"
    EXPECT_DOUBLE_EQ(11.0, calc.evaluate("5 3 2 * + "));
}

TEST(Calculator, can_evaluate_expression_with_parentheses)
{
    Calculator calc;
    // §³§à§à§ä§Ó§Ö§ä§ã§ä§Ó§å§Ö§ä "(5 + 3) * 2"
    EXPECT_DOUBLE_EQ(16.0, calc.evaluate("5 3 + 2 * "));
}

TEST(Calculator, can_evaluate_sin_function)
{
    Calculator calc;
    EXPECT_DOUBLE_EQ(sin(1.0), calc.evaluate("1 s "));
}

TEST(Calculator, can_evaluate_cos_function)
{
    Calculator calc;
    // cos(0) = 1
    EXPECT_DOUBLE_EQ(1.0, calc.evaluate("0 c "));
}

TEST(Calculator, can_evaluate_ln_function)
{
    Calculator calc;
    // ln(1) = 0
    EXPECT_DOUBLE_EQ(0.0, calc.evaluate("1 l "));
}

TEST(Calculator, can_evaluate_exp_function)
{
    Calculator calc;
    EXPECT_DOUBLE_EQ(exp(2.5), calc.evaluate("2.5 e "));
}

TEST(Calculator, can_evaluate_nested_functions)
{
    Calculator calc;
    // ln(exp(5)) = 5
    EXPECT_DOUBLE_EQ(5.0, calc.evaluate("5 e l "));
}

TEST(Calculator, can_evaluate_complex_expression)
{
    Calculator calc;
    // §³§à§à§ä§Ó§Ö§ä§ã§ä§Ó§å§Ö§ä "3 + 4 * 2 / (1 - 5)"
    EXPECT_DOUBLE_EQ(1.0, calc.evaluate("3 4 2 * 1 5 - / + "));
}

TEST(Calculator, throws_on_division_by_zero)
{
    Calculator calc;
    ASSERT_ANY_THROW(calc.evaluate("10 0 / "));
}

TEST(Calculator, throws_when_not_enough_operands)
{
    Calculator calc;
    // §¥§Ý§ñ '+' §ß§å§Ø§ß§à §Õ§Ó§Ñ §à§á§Ö§â§Ñ§ß§Õ§Ñ, §Ñ §Ó §ã§ä§Ö§Ü§Ö §Ò§å§Õ§Ö§ä §ä§à§Ý§î§Ü§à §à§Õ§Ú§ß
    ASSERT_ANY_THROW(calc.evaluate("5 + "));
}