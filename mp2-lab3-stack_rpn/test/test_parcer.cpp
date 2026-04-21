#include "Parcer.h" // §£§Ñ§ê §Ù§Ñ§Ô§à§Ý§à§Ó§à§é§ß§í§Û §æ§Ñ§Û§Ý §Õ§Ý§ñ §á§Ñ§â§ã§Ö§â§Ñ
#include <gtest.h>  // §±§à§Õ§Ü§Ý§ð§é§Ö§ß§Ú§Ö Google Test

TEST(Parser, can_parse_simple_addition)
{
    Parser p;
    EXPECT_EQ("5 3 + ", p.toRPN("5 + 3"));
}

TEST(Parser, respects_operator_precedence)
{
    Parser p;
    EXPECT_EQ("5 3 2 * + ", p.toRPN("5 + 3 * 2"));
}

TEST(Parser, can_parse_expression_with_parentheses)
{
    Parser p;
    EXPECT_EQ("5 3 + 2 * ", p.toRPN("(5 + 3) * 2"));
}

TEST(Parser, can_parse_single_function)
{
    Parser p;
    EXPECT_EQ("1 s ", p.toRPN("sin(1)"));
}

TEST(Parser, can_parse_complex_expression_with_functions)
{
    Parser p;
    EXPECT_EQ("1 s 2 c + ", p.toRPN("sin(1) + cos(2)"));
}

TEST(Parser, can_parse_nested_functions)
{
    Parser p;
    EXPECT_EQ("5 e l ", p.toRPN("ln(exp(5))"));
}

TEST(Parser, can_parse_complex_expression)
{
    Parser p;
    EXPECT_EQ("3 4 2 * 1 5 - / + ", p.toRPN("3 + 4 * 2 / (1 - 5)"));
}

TEST(Parser, can_parse_floats)
{
    Parser p;
    EXPECT_EQ("10.5 2.1 * ", p.toRPN("10.5 * 2.1"));
}

TEST(Parser, throws_on_unmatched_opening_parenthesis)
{
    Parser p;
    ASSERT_ANY_THROW(p.toRPN("(5 + 3"));
}

TEST(Parser, throws_on_unmatched_closing_parenthesis)
{
    Parser p;
    ASSERT_ANY_THROW(p.toRPN("5 + 3)"));
}

TEST(Parser, throws_on_invalid_character)
{
    Parser p;
    ASSERT_ANY_THROW(p.toRPN("5 & 3"));
}

TEST(Parser, throws_on_unknown_function)
{
    Parser p;
    ASSERT_ANY_THROW(p.toRPN("log(10)"));
}