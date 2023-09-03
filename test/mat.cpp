#include <gtest/gtest.h>

#include <mat.hpp>
#include <math.hpp>

using namespace rt;

TEST(mat, make_identity) {
    auto m1 = mat<2,2>::make_identity();
    EXPECT_FLOAT_EQ(m1(0, 0), 1);
    EXPECT_FLOAT_EQ(m1(0, 1), 0);
    EXPECT_FLOAT_EQ(m1(1, 0), 0);
    EXPECT_FLOAT_EQ(m1(1, 1), 1);

    auto m2 = mat<3,3>::make_identity();
    EXPECT_FLOAT_EQ(m2(0, 0), 1);
    EXPECT_FLOAT_EQ(m2(0, 1), 0);
    EXPECT_FLOAT_EQ(m2(0, 2), 0);
    EXPECT_FLOAT_EQ(m2(1, 0), 0);
    EXPECT_FLOAT_EQ(m2(1, 1), 1);
    EXPECT_FLOAT_EQ(m2(1, 2), 0);
    EXPECT_FLOAT_EQ(m2(2, 0), 0);
    EXPECT_FLOAT_EQ(m2(2, 1), 0);
    EXPECT_FLOAT_EQ(m2(2, 2), 1);

    auto m3 = mat<4,4>::make_identity();
    EXPECT_FLOAT_EQ(m3(0, 0), 1);
    EXPECT_FLOAT_EQ(m3(0, 1), 0);
    EXPECT_FLOAT_EQ(m3(0, 2), 0);
    EXPECT_FLOAT_EQ(m3(0, 3), 0);
    EXPECT_FLOAT_EQ(m3(1, 0), 0);
    EXPECT_FLOAT_EQ(m3(1, 1), 1);
    EXPECT_FLOAT_EQ(m3(1, 2), 0);
    EXPECT_FLOAT_EQ(m3(1, 3), 0);
    EXPECT_FLOAT_EQ(m3(2, 0), 0);
    EXPECT_FLOAT_EQ(m3(2, 1), 0);
    EXPECT_FLOAT_EQ(m3(2, 2), 1);
    EXPECT_FLOAT_EQ(m3(2, 3), 0);
    EXPECT_FLOAT_EQ(m3(3, 0), 0);
    EXPECT_FLOAT_EQ(m3(3, 1), 0);
    EXPECT_FLOAT_EQ(m3(3, 2), 0);
    EXPECT_FLOAT_EQ(m3(3, 3), 1);
}

TEST(mat, make_translation) {
    auto m = mat<4,4>::make_translation(5, -3, 2);
    auto p = vec::make_point(-3, 4, 5);
    EXPECT_EQ(m * p, vec::make_point(2, 1, 7));

    ASSERT_TRUE(m.invertible());
    EXPECT_EQ(m.inverse() * p, vec::make_point(-8, 7, 3));

    auto v = vec::make_vector(-3, 4, 5);
    EXPECT_EQ(m * v, v);
}

TEST(mat, make_scaled) {
    auto m = mat<4,4>::make_scaled(2, 3, 4);
    auto p = vec::make_point(-4, 6, 8);
    EXPECT_EQ(m * p, vec::make_point(-8, 18, 32));

    auto v = vec::make_vector(-4, 6, 8);
    EXPECT_EQ(m * v, vec::make_vector(-8, 18, 32));

    ASSERT_TRUE(m.invertible());
    EXPECT_EQ(m.inverse() * v, vec::make_vector(-2, 2, 2));
}

TEST(mat, make_rotated_x) {
    auto p = vec::make_point(0, 1, 0);
    auto m1 = mat<4,4>::make_rotated_x(PI_4);
    auto m2 = mat<4,4>::make_rotated_x(PI_2);
    EXPECT_EQ(m1 * p, vec::make_point(0, std::sqrt(2) / 2, std::sqrt(2) / 2));
    EXPECT_EQ(m2 * p, vec::make_point(0, 0, 1));
}

TEST(mat, make_rotated_y) {
    auto p = vec::make_point(0, 0, 1);
    auto m1 = mat<4,4>::make_rotated_y(PI_4);
    auto m2 = mat<4,4>::make_rotated_y(PI_2);
    EXPECT_EQ(m1 * p, vec::make_point(std::sqrt(2) / 2, 0, std::sqrt(2) / 2));
    EXPECT_EQ(m2 * p, vec::make_point(1, 0, 0));
}

TEST(mat, make_rotated_z) {
    auto p = vec::make_point(0, 1, 0);
    auto m1 = mat<4,4>::make_rotated_z(PI_4);
    auto m2 = mat<4,4>::make_rotated_z(PI_2);
    EXPECT_EQ(m1 * p, vec::make_point(-std::sqrt(2) / 2, std::sqrt(2) / 2, 0));
    EXPECT_EQ(m2 * p, vec::make_point(-1, 0, 0));
}

TEST(mat, make_sheared) {
    auto p = vec::make_point(2, 3, 4);

    auto m1 = mat<4,4>::make_sheared(1, 0, 0, 0, 0, 0);
    EXPECT_EQ(m1 * p, vec::make_point(5, 3, 4));

    auto m2 = mat<4,4>::make_sheared(0, 1, 0, 0, 0, 0);
    EXPECT_EQ(m2 * p, vec::make_point(6, 3, 4));

    auto m3 = mat<4,4>::make_sheared(0, 0, 1, 0, 0, 0);
    EXPECT_EQ(m3 * p, vec::make_point(2, 5, 4));

    auto m4 = mat<4,4>::make_sheared(0, 0, 0, 1, 0, 0);
    EXPECT_EQ(m4 * p, vec::make_point(2, 7, 4));

    auto m5 = mat<4,4>::make_sheared(0, 0, 0, 0, 1, 0);
    EXPECT_EQ(m5 * p, vec::make_point(2, 3, 6));

    auto m6 = mat<4,4>::make_sheared(0, 0, 0, 0, 0, 1);
    EXPECT_EQ(m6 * p, vec::make_point(2, 3, 7));
}

TEST(mat, chained_transformations) {
    auto p1 = vec::make_point(1, 0, 1);
    auto p2 = mat<4,4>::make_translation(10, 5, 7) * mat<4,4>::make_scaled(5) * mat<4,4>::make_rotated_x(PI_2) * p1;
    EXPECT_EQ(p2, vec::make_point(15, 0, 7));
}

TEST(mat, transpose) {
    mat<2,2> m1{
        1, 2,
        3, 4
    };
    mat<2,2> m2{
        1, 3,
        2, 4
    };
    EXPECT_EQ(m1.transpose(), m2);
}

TEST(mat, sub) {
    mat<2,3> m1{
        1, 2, 3,
        4, 5, 6
    };
    mat<1,2> m2{
        1, 3
    };
    EXPECT_EQ(m1.sub(1, 1), m2);
}

TEST(mat, minor) {
    mat<3,3> m{
        3, 5, 0,
        2, -1, -7,
        6, -1, 5
    };
    EXPECT_FLOAT_EQ(m.minor(1, 0), 25);
}

TEST(mat, cofactor) {
    mat<3,3> m{
        3, 5, 0,
        2, -1, -7,
        6, -1, 5
    };
    EXPECT_FLOAT_EQ(m.cofactor(0, 0), -12);
    EXPECT_FLOAT_EQ(m.cofactor(1, 0), -25);
}

TEST(mat, determinant) {
    mat<3,3> m1{
        1, 2, 6,
        -5, 8, -4,
        2, 6, 4
    };
    EXPECT_FLOAT_EQ(m1.determinant(), -196);

    mat<4,4> m2{
        -2, -8, 3, 5,
        -3, 1, 7, 3,
        1, 2, -9, 6,
        -6, 7, 7, -9
    };
    EXPECT_FLOAT_EQ(m2.determinant(), -4071);
}

TEST(mat, invertible) {
    mat<4,4> m1{
        6, 4, 4, 4,
        5, 5, 7, 6,
        4, -9, 3, -7,
        9, 1, 7, -6
    };
    EXPECT_TRUE(m1.invertible());

    mat<4,4> m2{
        -4, 2, -2, -3,
        9, 6, 2, 6,
        0, -5, 1, -5,
        0, 0, 0, 0
    };
    EXPECT_FALSE(m2.invertible());
}

TEST(mat, inverse) {
    mat<4,4> m1{
        8, -5, 9, 2,
        7, 5, 6, 1,
        -6, 0, 9, 6,
        -3, 0, -9, -4
    };
    mat<4,4> m2{
        -0.15385f, -0.15385f, -0.28205f, -0.53846f,
        -0.07692f, 0.12308f, 0.02564f, 0.03077f,
        0.35897f, 0.35897f, 0.43590f, 0.92308f,
        -0.69231f, -0.69231f, -0.76923f, -1.92308f
    };
    ASSERT_TRUE(m1.invertible());
    EXPECT_EQ(m1.inverse(), m2);

    mat<4,4> m3{
        9, 3, 0, 9,
        -5, -2, -6, -3,
        -4, 9, 6, 4,
        -7, 6, 6, 2
    };
    mat<4,4> m4{
        -0.04074f, -0.07778f, 0.14444f, -0.22222f,
        -0.07778f, 0.03333f, 0.36667f, -0.33333f,
        -0.02901f, -0.1463f, -0.10926f, 0.12963f,
        0.17778f, 0.06667f, -0.26667f, 0.33333f
    };
    ASSERT_TRUE(m3.invertible());
    EXPECT_EQ(m3.inverse(), m4);

    mat<4,4> m5{
        3, -9, 7, 3,
        3, -8, 2, -9,
        -4, 4, 4, 1,
        -6, 5, -1, 1
    };
    mat<4,4> m6{
        8, 2, 2, 2,
        3, -1, 7, 0,
        7, 0, 5, 4,
        6, -2, 0, 5
    };
    mat<4,4> m7 = m5 * m6;
    ASSERT_TRUE(m6.invertible());
    EXPECT_EQ(m7 * m6.inverse(), m5);
}

TEST(mat, set_get_values) {
    mat<2,2> m{
        1, 2,
        3, 4
    };
    EXPECT_FLOAT_EQ(m.get(0, 0), 1);
    EXPECT_FLOAT_EQ(m.get(0, 1), 2);
    EXPECT_FLOAT_EQ(m.get(1, 0), 3);
    EXPECT_FLOAT_EQ(m.get(1, 1), 4);

    m.set(5, 0, 0);
    EXPECT_FLOAT_EQ(m.get(0, 0), 5);
    EXPECT_FLOAT_EQ(m.get(0, 1), 2);
    EXPECT_FLOAT_EQ(m.get(1, 0), 3);
    EXPECT_FLOAT_EQ(m.get(1, 1), 4);

    m(0, 0) = -2;
    EXPECT_FLOAT_EQ(m.get(0, 0), -2);
    EXPECT_FLOAT_EQ(m(0, 0), -2);
}

TEST(mat, mat_multiplication) {
    mat<2,2> m1{
        1, 2,
        3, 4
    };
    mat<2,2> m2{
        5, 6,
        7, 8
    };
    mat<2,2> m3{
        19, 22,
        43, 50
    };
    EXPECT_EQ(m1 * m2, m3);

    mat<2,2> m4{
        10, 11,
        12, -3
    };
    mat<2,2> m5{
        6, 1,
        7, 8
    };
    mat<2,2> m6{
        137, 98,
        51, -12
    };
    EXPECT_EQ(m4 * m5, m6);
}

TEST(mat, vec_multiplication) {
    mat<4,4> m{
        1, 2, 3, 4,
        2, 4, 4, 2,
        8, 6, 4, 1,
        0, 0, 0, 1
    };
    vec v = vec::make_point(1, 2, 3);
    EXPECT_EQ(m * v, vec::make_point(18, 24, 33));
}

TEST(mat, equality) {
    mat<2,2> m1{0, 0, 1, 1};
    mat<2,2> m2{0, 0, 1, 1};
    mat<2,2> m3{1, 1, 2, 3};
    EXPECT_TRUE(m1 == m2);
    EXPECT_FALSE(m1 == m3);
    EXPECT_FALSE(m1 != m2);
    EXPECT_TRUE(m1 != m3);
}
