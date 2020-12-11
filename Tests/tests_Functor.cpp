/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Trivial functor unit tests
 */

#include <gtest/gtest.h>

#include <Core/Functor.hpp>

struct Foo
{
    int y { 2 };

    int memberFunction(const int x)
        { return x * y; }

    static int FreeFunction(const int x, const int y)
        { return x * y; }
};

TEST(Functor, FreeBasics)
{
    Core::Functor<int(int, int)> func;
    func.prepare<&Foo::FreeFunction>();
    ASSERT_TRUE(func);
    ASSERT_EQ(func(4, 2), 8);
    ASSERT_EQ(func(8, 2), 16);
    auto func2(std::move(func));
    ASSERT_FALSE(func);
    ASSERT_TRUE(func2);
    ASSERT_EQ(func2(4, 2), 8);
    ASSERT_EQ(func2(8, 2), 16);
    func2 = std::move(func);
    ASSERT_FALSE(func2);
}

TEST(Functor, MemberBasics)
{
    Foo foo;
    Core::Functor<int(int)> func;
    func.prepare<&Foo::memberFunction>(&foo);
    ASSERT_TRUE(func);
    ASSERT_EQ(func(4), 8);
    ASSERT_EQ(func(8), 16);
    auto func2(std::move(func));
    ASSERT_FALSE(func);
    ASSERT_TRUE(func2);
    ASSERT_EQ(func2(4), 8);
    ASSERT_EQ(func2(8), 16);
    func2 = std::move(func);
    ASSERT_FALSE(func2);
}

TEST(Functor, TrivialFunctorBasics)
{
    int y = 1;
    Core::Functor<int(int)> func([&y](const int x) {
        return x * y;
    });
    ++y;
    ASSERT_TRUE(func);
    ASSERT_EQ(func(4), 8);
    ASSERT_EQ(func(8), 16);
    auto func2(std::move(func));
    ASSERT_FALSE(func);
    ASSERT_TRUE(func2);
    ASSERT_EQ(func2(4), 8);
    ASSERT_EQ(func2(8), 16);
    func2 = std::move(func);
    ASSERT_FALSE(func2);
}

TEST(Functor, NonTrivialClassFunctorBasics)
{
    Core::Functor<int(int)> func([y = std::make_unique<int>(2)](const int x) {
        return x * *y;
    });
    ASSERT_TRUE(func);
    ASSERT_EQ(func(4), 8);
    ASSERT_EQ(func(8), 16);
    auto func2(std::move(func));
    ASSERT_FALSE(func);
    ASSERT_TRUE(func2);
    ASSERT_EQ(func2(4), 8);
    ASSERT_EQ(func2(8), 16);
    func2 = std::move(func);
    ASSERT_FALSE(func2);
}

TEST(Functor, SneakyErrors)
{
    bool trigger = false;

    Core::Functor<void(void)> func([&trigger] { trigger = true; });

    ASSERT_FALSE(trigger);
    func();
    ASSERT_TRUE(trigger);
}