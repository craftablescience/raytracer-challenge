#include <gtest/gtest.h>

#include <ray.hpp>

using namespace rt;

TEST(ray, dot) {
    ray r1{vec::make_point(0), vec::make_vector(1, 2, 3)};
    auto v = vec::make_vector(2, 3, 4);
    ray r2{vec::make_point(0), v};
    EXPECT_FLOAT_EQ(r1.dot(r2), 20.f);
    EXPECT_FLOAT_EQ(r1 * r2, 20.f);
    EXPECT_FLOAT_EQ(r1.dot(v), 20.f);
    EXPECT_FLOAT_EQ(r1 * v, 20.f);
}

TEST(ray, cross) {
    auto v1 = vec::make_vector(1, 2, 3);
    auto v2 = vec::make_vector(2, 3, 4);
    EXPECT_EQ(v1.cross(v2), vec::make_vector(-1, 2, -1));
    EXPECT_EQ(v2.cross(v1), vec::make_vector(1, -2, 1));
}

TEST(ray, point_along) {
    ray r{vec::make_point(2, 3, 4), vec::make_vector(1, 0, 0)};
    EXPECT_EQ(r.point_along(0), vec::make_point(2, 3, 4));
    EXPECT_EQ(r.point_along(1), vec::make_point(3, 3, 4));
    EXPECT_EQ(r.point_along(-1), vec::make_point(1, 3, 4));
    EXPECT_EQ(r.point_along(2.5f), vec::make_point(4.5, 3, 4));
}

TEST(ray, transform) {
    ray r1{vec::make_point(1, 2, 3), vec::make_vector(0, 1, 0)};
    auto m1 = mat<4,4>::make_translation(3, 4, 5);
    auto r2 = r1.transform(m1);
    EXPECT_EQ(r2, r1 * m1);
    EXPECT_EQ(r2.origin, vec::make_point(4, 6, 8));
    EXPECT_EQ(r2.direction, vec::make_vector(0, 1, 0));

    ray r3{vec::make_point(1, 2, 3), vec::make_vector(0, 1, 0)};
    auto m2 = mat<4,4>::make_scaled(2, 3, 4);
    auto r4 = r3.transform(m2);
    EXPECT_EQ(r4.origin, vec::make_point(2, 6, 12));
    EXPECT_EQ(r4.direction, vec::make_vector(0, 3, 0));
}
