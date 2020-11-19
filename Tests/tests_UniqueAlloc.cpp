/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Tests of the single consumer concurrent queue
 */
#include <gtest/gtest.h>

#include <Core/UniqueAlloc.hpp>

TEST(UniqueAlloc, Basics)
{
    Core::UniqueAlloc<int> alloc(42);

    ASSERT_EQ(*alloc, 42);
    ++*alloc;
    ASSERT_EQ(*alloc, 43);
}
