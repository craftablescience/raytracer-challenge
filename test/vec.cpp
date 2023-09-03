#include <gtest/gtest.h>

#include <cmath>
#include <vec.hpp>

using namespace rt;

TEST(vec, make_vector) {
    auto v = vec::make_vector(1, 2, 3);
    EXPECT_FLOAT_EQ(v.x, 1.f);
    EXPECT_FLOAT_EQ(v.y, 2.f);
    EXPECT_FLOAT_EQ(v.z, 3.f);
    EXPECT_FLOAT_EQ(v.w, 0.f);
}

TEST(vec, make_point) {
    auto p = vec::make_point(-1, -2, -3);
    EXPECT_FLOAT_EQ(p.x, -1.f);
    EXPECT_FLOAT_EQ(p.y, -2.f);
    EXPECT_FLOAT_EQ(p.z, -3.f);
    EXPECT_FLOAT_EQ(p.w, 1.f);
}

TEST(vec, dot) {
    auto v1 = vec::make_vector(1, 2, 3);
    auto v2 = vec::make_vector(2, 3, 4);
    EXPECT_FLOAT_EQ(v1.dot(v2), 20.f);
    EXPECT_FLOAT_EQ(v1 * v2, 20.f);
}

TEST(vec, cross) {
    auto v1 = vec::make_vector(1, 2, 3);
    auto v2 = vec::make_vector(2, 3, 4);
    EXPECT_EQ(v1.cross(v2), vec::make_vector(-1, 2, -1));
    EXPECT_EQ(v2.cross(v1), vec::make_vector(1, -2, 1));
}

TEST(vec, magnitude) {
    auto v1 = vec::make_vector(1, 0, 0);
    auto v2 = vec::make_vector(0, 1, 0);
    auto v3 = vec::make_vector(0, 0, 1);
    EXPECT_FLOAT_EQ(v1.magnitude(), 1.f);
    EXPECT_FLOAT_EQ(v2.magnitude(), 1.f);
    EXPECT_FLOAT_EQ(v3.magnitude(), 1.f);

    auto v4 = vec::make_vector(1, 2, 3);
    auto v5 = vec::make_vector(-1, -2, -3);
    EXPECT_FLOAT_EQ(v4.magnitude(), std::sqrt(14));
    EXPECT_FLOAT_EQ(v5.magnitude(), std::sqrt(14));
}

TEST(vec, normalize) {
    auto v1 = vec::make_vector(5, 0, 0);
    auto v2 = vec::make_vector(0, 5, 0);
    auto v3 = vec::make_vector(0, 0, 5);
    EXPECT_EQ(v1.normalize(), vec::make_vector(1, 0, 0));
    EXPECT_EQ(v2.normalize(), vec::make_vector(0, 1, 0));
    EXPECT_EQ(v3.normalize(), vec::make_vector(0, 0, 1));

    auto v4 = vec::make_vector(-5, 0, 0);
    auto v5 = vec::make_vector(0, -5, 0);
    auto v6 = vec::make_vector(0, 0, -5);
    EXPECT_EQ(v4.normalize(), vec::make_vector(-1, 0, 0));
    EXPECT_EQ(v5.normalize(), vec::make_vector(0, -1, 0));
    EXPECT_EQ(v6.normalize(), vec::make_vector(0, 0, -1));

    auto v7 = vec::make_vector(1, 2, 3);
    EXPECT_EQ(v7.normalize(), vec::make_vector(1 / std::sqrt(14), 2 / std::sqrt(14), 3 / std::sqrt(14)));
}

TEST(vec, is_unit_vector) {
    auto v1 = vec::make_vector(1, 0, 0);
    auto v2 = vec::make_vector(0, 1, 0);
    auto v3 = vec::make_vector(0, 0, 1);
    auto v4 = vec::make_vector(-1, 0, 0);
    auto v5 = vec::make_vector(0, -1, 0);
    auto v6 = vec::make_vector(0, 0, -1);
    EXPECT_TRUE(v1.is_unit_vector());
    EXPECT_TRUE(v2.is_unit_vector());
    EXPECT_TRUE(v3.is_unit_vector());
    EXPECT_TRUE(v4.is_unit_vector());
    EXPECT_TRUE(v5.is_unit_vector());
    EXPECT_TRUE(v6.is_unit_vector());

    auto v7 = vec::make_vector(2, 0, 0);
    EXPECT_FALSE(v7.is_unit_vector());

    auto v8 = vec::make_vector(1 / std::sqrtf(2), 1 / std::sqrtf(2), 0);
    EXPECT_TRUE(v8.is_unit_vector());
}

TEST(vec, comparison_operators) {
    auto v1 = vec::make_vector(1, 2, 3);
    auto v2 = vec::make_vector(1, 2, 3);
    auto v3 = vec::make_vector(3, 4, 5);
    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
    EXPECT_FALSE(v1 != v2);
    EXPECT_TRUE(v1 != v3);
}

TEST(vec, unary_operators) {
    auto v = vec::make_vector(-1, 0, 3);
    EXPECT_EQ(+v, vec::make_vector(-1, 0, 3));
    EXPECT_EQ(-v, vec::make_vector(1, 0, -3));
}

TEST(vec, vector_operators) {
    auto v1 = vec::make_vector(1, 2, 3);
    auto v2 = vec::make_vector(2, 3, 4);
    EXPECT_EQ(v1 + v2, vec::make_vector(3, 5, 7));
    EXPECT_EQ(v1 - v2, vec::make_vector(-1, -1, -1));
}

TEST(vec, scalar_operators) {
    auto v = vec::make_vector(1, 2, 3);
    EXPECT_EQ(v * 2, vec::make_vector(2, 4, 6));
    EXPECT_EQ(v * 0.5f, vec::make_vector(0.5f, 1.f, 1.5f));
    EXPECT_EQ(v / 2, vec::make_vector(0.5f, 1.f, 1.5f));
    EXPECT_EQ(v / 0.5f, vec::make_vector(2, 4, 6));
}
