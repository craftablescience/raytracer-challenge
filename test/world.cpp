#include <gtest/gtest.h>

#include <world.hpp>

using namespace rt;

TEST(transform, ctor) {
    transform t1{};
    EXPECT_EQ(t1.get_transform(), (mat<4,4>::make_identity()));

    transform t2{vec::make_point(2)};
    EXPECT_EQ(t2.get_transform(), (mat<4,4>::make_translation(2)));

    transform t3{vec::make_point(0), vec::make_vector(2)};
    EXPECT_EQ(t3.get_transform(), (mat<4,4>::make_scaled(2)));

    transform t4{vec::make_point(2), vec::make_vector(2)};
    EXPECT_EQ(t4.get_transform(), (mat<4,4>::make_translation(2) * mat<4, 4>::make_scaled(2)));
}

TEST(intersection, discard_occluded) {
    std::vector<intersection> v;
    intersection i1{ray{}, 1, 0};
    intersection i2{ray{}, 2, 0};
    v.push_back(i1);
    v.push_back(i2);
    auto ir1 = intersection::discard_occluded(v);
    ASSERT_TRUE(ir1);
    EXPECT_EQ(*ir1, i1);

    v.clear();
    intersection i3{ray{}, -1, 0};
    intersection i4{ray{}, 1, 0};
    v.push_back(i3);
    v.push_back(i4);
    auto ir2 = intersection::discard_occluded(v);
    ASSERT_TRUE(ir2);
    EXPECT_EQ(*ir2, i4);

    v.clear();
    intersection i5{ray{}, -2, 0};
    intersection i6{ray{}, -1, 0};
    v.push_back(i5);
    v.push_back(i6);
    auto ir3 = intersection::discard_occluded(v);
    EXPECT_FALSE(ir3);

    v.clear();
    intersection i7{ray{}, 0, 0};
    intersection i8{ray{}, 0, 0};
    v.push_back(i7);
    v.push_back(i8);
    auto ir4 = intersection::discard_occluded(v);
    ASSERT_TRUE(ir4);
    EXPECT_EQ(ir4, i7);
    EXPECT_EQ(ir4, i8);
}

TEST(sphere, intersects_and_intersections) {
    sphere s{1};

    // Direct hit
    ray r1{vec::make_point(0, 0, -5), vec::make_vector(0, 0, 1)};
    EXPECT_TRUE(s.intersects(r1));
    auto i1 = s.intersections(r1);
    ASSERT_EQ(i1.size(), 2);
    EXPECT_FLOAT_EQ(i1[0].distance, 4);
    EXPECT_FLOAT_EQ(i1[1].distance, 6);

    // Tangent
    ray r2{vec::make_point(0, 1, -5), vec::make_vector(0, 0, 1)};
    EXPECT_TRUE(s.intersects(r2));
    auto i2 = s.intersections(r2);
    ASSERT_EQ(i2.size(), 2);
    EXPECT_FLOAT_EQ(i2[0].distance, 5);
    EXPECT_FLOAT_EQ(i2[1].distance, 5);

    // Miss
    ray r3{vec::make_point(0, 2, -5), vec::make_vector(0, 0, 1)};
    EXPECT_FALSE(s.intersects(r3));
    auto i3 = s.intersections(r3);
    EXPECT_TRUE(i3.empty());

    // Inside
    ray r4{vec::make_point(0, 0, 0), vec::make_vector(0, 0, 1)};
    EXPECT_TRUE(s.intersects(r4));
    auto i4 = s.intersections(r4);
    ASSERT_EQ(i4.size(), 2);
    EXPECT_FLOAT_EQ(i4[0].distance, -1);
    EXPECT_FLOAT_EQ(i4[1].distance, 1);

    // Ahead of
    ray r5{vec::make_point(0, 0, 5), vec::make_vector(0, 0, 1)};
    EXPECT_TRUE(s.intersects(r5));
    auto i5 = s.intersections(r5);
    ASSERT_EQ(i5.size(), 2);
    EXPECT_FLOAT_EQ(i5[0].distance, -6);
    EXPECT_FLOAT_EQ(i5[1].distance, -4);
}

TEST(sphere, intersects_and_intersections_transformed) {
    ray r{vec::make_point(0, 0, -5), vec::make_vector(0, 0, 1)};

    // Transformed
    sphere s1{1};
    s1.model.set_translation(vec::make_point(5, 0, 0));
    EXPECT_FALSE(s1.intersects(r));
    auto i1 = s1.intersections(r);
    EXPECT_TRUE(i1.empty());

    // Scaled
    sphere s2{1};
    s2.model.set_scale(2);
    EXPECT_TRUE(s2.intersects(r));
    auto i2 = s2.intersections(r);
    ASSERT_EQ(i2.size(), 2);
    EXPECT_FLOAT_EQ(i2[0].distance, 3);
    EXPECT_FLOAT_EQ(i2[1].distance, 7);
}

TEST(world, add_sphere) {
    world w;
    ray r{vec::make_point(0, 0, 0), vec::make_vector(0, 0, 1)};

    w.add<sphere>(vec::make_point(0, 0, 0), vec::make_vector(1));
    auto i1 = w.get_intersections(r);
    ASSERT_EQ(i1.size(), 2);
    EXPECT_FLOAT_EQ(i1[0].distance, -1);
    EXPECT_FLOAT_EQ(i1[1].distance, 1);
    auto i2 = w.get_visible_intersection(r);
    ASSERT_TRUE(i2);
    EXPECT_FLOAT_EQ(i2->distance, 1);

    w.add<sphere>(vec::make_point(0, 0, 2), vec::make_vector(2));
    auto i3 = w.get_intersections(r);
    ASSERT_EQ(i3.size(), 4);
    EXPECT_FLOAT_EQ(i3[0].distance, -1);
    EXPECT_FLOAT_EQ(i3[1].distance, 1);
    EXPECT_FLOAT_EQ(i3[2].distance, 0);
    EXPECT_FLOAT_EQ(i3[3].distance, 4);
    auto i4 = w.get_visible_intersection(r);
    ASSERT_TRUE(i4);
    EXPECT_FLOAT_EQ(i4->distance, 0);
}

/*
TEST(world, render_spheres) {
    world w;
    w.add<sphere>(vec::make_point(0, -1, 4), vec::make_vector(1));
    w.add<sphere>(vec::make_point(0, 2, 10), vec::make_vector(1));
    auto b = w.render(128, 128, vec::make_point(0, 0, 0), vec::make_vector(0, 0, 1), vec::make_vector(0, 1, 0), PI_2);
    b.save("test_world_render_spheres.png");
}
*/
