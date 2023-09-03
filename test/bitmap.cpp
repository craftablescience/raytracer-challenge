#include <gtest/gtest.h>

#include <bitmap.hpp>

using namespace rt;

TEST(bitmap, size) {
    bitmap b{4, 4};
    EXPECT_EQ(b.get_width(), 4);
    EXPECT_EQ(b.get_height(), 4);
    EXPECT_EQ(b.get_pixel(2, 2), color(0, 0, 0));
}

TEST(bitmap, set_get_pixel) {
    bitmap b{4, 4};
    b.set_pixel({1, 0, 1}, 2, 2);
    EXPECT_EQ(b.get_pixel(2, 2), color(1, 0, 1));
    EXPECT_EQ(b(2, 2), color(1, 0, 1));
}

/*
TEST(bitmap, save) {
    bitmap b{8, 8};
    for (short i = 0; i < b.get_width(); i++) {
        for (short j = 0; j < b.get_height(); j++) {
            b(i, j) = color{0, i > j ? 1.f : 0.5f, 1};
        }
    }
    b.save("test.png");
}
*/
