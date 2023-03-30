#include "gtest/gtest.h"

extern "C" {
#include "quilting.h"
#include "load_image.h"
}


TEST(min_cut, first) {

    Image *src_image = make_solid_image(8, 8, {255, 0, 0});
    Image *output_image = make_solid_image(8, 8, {0, 0, 0});

    ImageCoordinates block_coords = {0, 0};
    ImageCoordinates output_coords = {0, 0};

    min_cut(
            src_image,
            output_image,
            block_coords,
            output_coords,
            8,
            2,
            FIRST
    );

    ASSERT_TRUE(image_equal(src_image, output_image));
}


TEST(min_cut, overlap_error) {

    Image *src_image = make_solid_image(8, 8, {1, 1, 1});
    Image *output_image = make_solid_image(8, 8, {0, 0, 0});

    ImageCoordinates block_coords = {4, 4};
    ImageCoordinates output_coords = {0, 2};

    int *overlap_error = calc_overlap_error(
            src_image,
            output_image,
            block_coords,
            output_coords,
            4,
            2,
            ABOVE
    );

    for (int i = 0; i < 2; ++i) {
        ASSERT_EQ(overlap_error[i], 3);
    }

    free(overlap_error);
}