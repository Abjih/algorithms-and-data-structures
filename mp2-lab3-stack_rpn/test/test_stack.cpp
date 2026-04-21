// test_stack.cpp
#include "stack.h"  // §£§Ñ§ê §Ù§Ñ§Ô§à§Ý§à§Ó§à§é§ß§í§Û §æ§Ñ§Û§Ý §Õ§Ý§ñ §ã§ä§Ö§Ü§Ñ
#include <gtest.h>  // §±§à§Õ§Ü§Ý§ð§é§Ö§ß§Ú§Ö Google Test

TEST(Stack, can_create_stack_with_positive_size)
{
	ASSERT_NO_THROW(Stack<int> s(5));
}

TEST(Stack, throws_when_creating_stack_with_negative_size)
{
	ASSERT_ANY_THROW(Stack<int> s(-5));
}

TEST(Stack, throws_when_creating_stack_with_zero_size)
{
	ASSERT_ANY_THROW(Stack<int> s(0));
}

TEST(Stack, stack_is_empty_after_creation)
{
	Stack<int> s(5);
	EXPECT_TRUE(s.is_empty());
}

TEST(Stack, can_push_and_get_top_element)
{
	Stack<int> s(5);
	s.push(10);
	EXPECT_EQ(10, s.top());
}

TEST(Stack, can_push_and_pop_element)
{
	Stack<int> s(5);
	s.push(10);
	int val = s.pop();
	EXPECT_EQ(10, val);
	EXPECT_TRUE(s.is_empty());
}

TEST(Stack, stack_is_full_after_pushing_to_capacity)
{
	Stack<char> s(2);
	s.push('a');
	s.push('b');
	EXPECT_TRUE(s.is_full());
}

TEST(Stack, throws_when_pushing_to_full_stack)
{
	Stack<int> s(1);
	s.push(100);
	ASSERT_ANY_THROW(s.push(200));
}

TEST(Stack, throws_when_popping_from_empty_stack)
{
	Stack<int> s(3);
	ASSERT_ANY_THROW(s.pop());
}

TEST(Stack, throws_when_getting_top_from_empty_stack)
{
	Stack<int> s(3);
	ASSERT_ANY_THROW(s.top());
}