#include <gtest/gtest.h>

#include <math.hpp>

using namespace rt;

TEST(math, abs_constexpr) {
    EXPECT_EQ(abs_constexpr(5), 5);
    EXPECT_EQ(abs_constexpr(0), 0);
    EXPECT_EQ(abs_constexpr(-5), 5);

    EXPECT_FLOAT_EQ(abs_constexpr(5.f), 5.f);
    EXPECT_FLOAT_EQ(abs_constexpr(0.f), 0.f);
    EXPECT_FLOAT_EQ(abs_constexpr(-5.f), 5.f);
}

TEST(math, float_eq) {
    EXPECT_TRUE(float_eq(1.f, 1.f));

    EXPECT_FALSE(float_eq(1.f, 1.1f));
    EXPECT_FALSE(float_eq(1.f, 1.0001f));

    EXPECT_TRUE(float_eq(1.f, 1.000001f));
    EXPECT_FALSE(float_eq(1.f, 1.000001f, 0.0000001f));
}
