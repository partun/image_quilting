#include "gtest/gtest.h"

extern "C" {
#include "image.h"
}

TEST(image, rgb_error) {

    RGB a = {1, 2, 3};
    RGB b = {1, 2, 3};

    int error = rgb_sq_error(&a, &b);
    ASSERT_EQ(error, 0);

    a = {0, 0, 0};
    b = {255, 255, 255};

    error = rgb_sq_error(&a, &b);
    ASSERT_EQ(error, 195075);
}

TEST(image, rgb_compare) {

    RGB a = {1, 2, 3};
    RGB b = {1, 2, 3};
    RGB c = {255, 255, 255};

    ASSERT_TRUE(rgb_equal(&a, &a));
    ASSERT_TRUE(rgb_equal(&a, &b));
    ASSERT_FALSE(rgb_equal(&a, &c));
}

TEST(image, image_compare) {

    Image *a = make_solid_image(5, 5, {0, 255, 255});
    Image *b = make_solid_image(5, 5, {0, 255, 255});
    Image *c = make_solid_image(2, 2, {0, 255, 255});

    ASSERT_TRUE(image_equal(a, a));
    ASSERT_FALSE(image_equal(a, c));
    ASSERT_TRUE(image_equal(a, b));

    (a->data[24]).g = 0;
    ASSERT_FALSE(image_equal(a, b));

    free_image(a);
    free_image(b);
    free_image(c);
}
