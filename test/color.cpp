#include <gtest/gtest.h>

#include <color.hpp>

using namespace rt;

TEST(color, comparison_operators) {
    color c1{1, 2, 3};
    color c2{1, 2, 3};
    color c3{3, 4, 5};
    EXPECT_TRUE(c1 == c2);
    EXPECT_FALSE(c1 == c3);
    EXPECT_FALSE(c1 != c2);
    EXPECT_TRUE(c1 != c3);
}

TEST(color, color_operators) {
    color c1{0.1, 0.2, 0.3};
    color c2{0.2, 0.3, 0.4};
    EXPECT_EQ(c1 + c2, color(0.3, 0.5, 0.7));
    EXPECT_EQ(c1 - c2, color(-0.1, -0.1, -0.1));
    EXPECT_EQ(c1 * c2, color(0.1 * 0.2, 0.2 * 0.3, 0.3 * 0.4));
}

TEST(color, scalar_operators) {
    color c{1, 2, 3};
    EXPECT_EQ(c * 2, color(2, 4, 6));
    EXPECT_EQ(c * 0.5f, color(0.5f, 1.f, 1.5f));
    EXPECT_EQ(c / 2, color(0.5f, 1.f, 1.5f));
    EXPECT_EQ(c / 0.5f, color(2, 4, 6));
}
