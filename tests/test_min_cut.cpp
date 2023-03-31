#include "gtest/gtest.h"

extern "C" {
#include "quilting.h"
#include "load_image.h"
}

#define R {255, 0, 0}
#define G {255, 0, 0}
#define B {255, 0, 0}
#define W {0, 0, 0}

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
    free_image(src_image);
    free_image(output_image);
}


TEST(min_cut, overlap_error) {
    Image *src_image = make_solid_image(4, 4, R);
    RGB output_data[24] = {
            W, W, R, W, W, W,
            W, W, R, R, W, W,
            W, W, W, W, W, W,
            W, W, W, R, W, W,
    };
    Image output_image = {6, 4, output_data};


    ImageCoordinates block_coords = {0, 0};
    ImageCoordinates output_coords = {2, 0};


    Matrix *overlap_error = calc_overlap_error(
            src_image,
            &output_image,
            block_coords,
            output_coords,
            4,
            2,
            LEFT
    );

    int expected[8] = {
            0, 65025,
            0, 0,
            65025, 65025,
            65025, 0
    };
    Matrix expected_matrix = {2, 4, expected};

    print_matrix(overlap_error);

    ASSERT_TRUE(matrix_equal(overlap_error, &expected_matrix));

    free_image(src_image);
    free_matrix(overlap_error);
}


TEST(min_cut, left) {

    Image *src_image = make_solid_image(8, 8, W);

    RGB output_data[96] = {
            R, R, R, R, R, R, R, W, R, W, W, W,
            R, R, R, R, R, W, W, R, W, W, W, W,
            R, R, R, R, R, W, R, R, W, W, W, W,
            R, R, R, R, R, W, R, R, W, W, W, W,

            R, R, R, R, R, W, R, R, W, W, W, W,
            R, R, R, R, R, W, R, R, W, W, W, W,
            R, R, R, R, R, W, W, R, W, W, W, W,
            R, R, R, R, R, R, W, R, W, W, W, W,
    };
    Image output_image = {12, 8, output_data};


    ImageCoordinates block_coords = {0, 0};
    ImageCoordinates output_coords = {4, 0};


    Matrix *overlap_error = calc_overlap_error(
            src_image,
            &output_image,
            block_coords,
            output_coords,
            8,
            4,
            LEFT
    );

    print_matrix(overlap_error);

    Matrix *cut = min_cut(
            src_image,
            &output_image,
            block_coords,
            output_coords,
            8,
            4,
            LEFT
    );

    print_matrix(cut);

//    ASSERT_TRUE(image_equal(src_image, output_image));
}