#include "gtest/gtest.h"

extern "C" {
#include "src/baseline/quilting.h"
#include "load_image.h"
#include "image.h"
#include "matrix.h"
#include "src/baseline/min_cut.h"
#include "src/opt_6/min_cut_opt_6.h"
}

#define R {255, 0, 0}
#define G {255, 0, 0}
#define B {255, 0, 0}
#define W {0, 0, 0}

TEST(min_cut, matrix_transpose) {

    Matrix *a_matrix = (Matrix *) malloc(sizeof(Matrix));
    a_matrix->width = 4;
    a_matrix->height = 3;
    a_matrix->data = (int *) malloc(16 * sizeof(int));;
    for (int i = 0; i < 16; ++i) {
        a_matrix->data[i] = i;
    }


    int expected[16] = {
            0, 4, 8,
            1, 5, 9,
            2, 6, 10,
            3, 7, 11
    };
    Matrix expected_matrix = {3, 4, expected};

    Matrix *a_transposed = transpose(a_matrix);


    ASSERT_TRUE(matrix_equal(a_transposed, &expected_matrix));

    free_matrix(a_transposed);
}

TEST(min_cut, first) {

    Image *src_image = make_solid_image(8, 8, {255, 0, 0});
    Image *output_image = make_solid_image(8, 8, {0, 0, 0});

    ImageCoordinates block_coords = {0, 0};
    ImageCoordinates output_coords = {0, 0};

    int expected[64] = {
            1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1,

            1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1,
    };
    Matrix expected_cut = {8, 8, expected};

    Matrix *cut = min_cut(
            src_image,
            output_image,
            block_coords,
            output_coords,
            8,
            2,
            FIRST
    );

    print_matrix(cut);
    ASSERT_TRUE(matrix_equal(cut, &expected_cut));

    free_matrix(cut);
    free_image(src_image);
    free_image(output_image);
}


TEST(min_cut, overlap_error) {
    Image *src_image = make_solid_image(32, 32, R);
    Image *output_image = make_solid_image(32, 32, W);

    ImageRGB *src_image_rgb = convert_image_to_image_rgb(src_image);
    ImageRGB *output_image_rgb = convert_image_to_image_rgb(output_image);

    ImageCoordinates block_coords = {0, 0};
    ImageCoordinates output_coords = {0, 0};


    Matrix *overlap_error = calc_overlap_error(
            src_image,
            output_image,
            block_coords,
            output_coords,
            32,
            32
    );

    Matrix *overlap_error_opt_6 = calc_overlap_error_opt_6(
            src_image_rgb,
            output_image_rgb,
            block_coords,
            output_coords,
            32,
            32
    );


    print_matrix(overlap_error);
    print_matrix(overlap_error_opt_6);

    ASSERT_TRUE(matrix_equal(overlap_error, overlap_error_opt_6));

    free_image_rgb(src_image_rgb);
    free_image_rgb(output_image_rgb);
    free_image(src_image);
    free_image(output_image);
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
            4,
            8
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

    free_matrix(cut);
    free_matrix(overlap_error);
}


TEST(min_cut, above) {

    Image *src_image = make_solid_image(8, 8, W);

    RGB output_data[96] = {
            R, R, R, R, R, R, R, R,
            R, R, R, R, R, R, R, R,
            R, R, R, R, R, R, R, R,
            R, R, R, R, R, R, R, R,

            R, R, R, R, W, W, R, R,
            W, R, R, W, R, R, W, R,
            R, W, W, R, R, R, R, W,
            R, R, R, R, R, R, R, W,

            R, R, R, R, R, R, R, R,
            R, R, R, R, R, R, R, R,
            R, R, R, R, R, R, R, R,
            R, R, R, R, R, R, R, R,
    };
    Image output_image = {8, 12, output_data};


    ImageCoordinates block_coords = {0, 0};
    ImageCoordinates output_coords = {0, 4};


    Matrix *overlap_error = calc_overlap_error(
            src_image,
            &output_image,
            block_coords,
            output_coords,
            8,
            4
    );

    print_matrix(overlap_error);

    Matrix *cut = min_cut(
            src_image,
            &output_image,
            block_coords,
            output_coords,
            8,
            4,
            ABOVE
    );

    print_matrix(cut);

    free_matrix(cut);
    free_matrix(overlap_error);
}

TEST(min_cut, corner_0) {

    Image *src_image = make_solid_image(8, 8, W);

    RGB output_data[144] = {
            R, R, R, R, R, R, R, R, R, R, R, R,
            R, R, R, R, R, R, R, R, R, R, R, R,
            R, R, R, R, R, R, R, R, R, R, R, R,
            R, R, R, R, R, R, R, R, R, R, R, R,

            R, R, R, R, R, W, R, R, R, R, R, R,
            R, R, R, R, W, R, W, R, R, R, R, W,
            R, R, R, R, W, R, R, W, R, R, W, R,
            R, R, R, R, W, R, R, R, W, W, R, R,

            R, R, R, R, R, W, R, R, R, R, R, R,
            R, R, R, R, R, W, R, R, R, R, R, R,
            R, R, R, R, R, R, W, R, R, R, R, R,
            R, R, R, R, R, R, W, R, R, R, R, R,
    };
    Image output_image = {12, 12, output_data};


    ImageCoordinates block_coords = {0, 0};
    ImageCoordinates output_coords = {4, 4};


    Matrix *overlap_error_above = calc_overlap_error(
            src_image,
            &output_image,
            block_coords,
            output_coords,
            8,
            4
    );
    print_matrix(overlap_error_above);
    free_matrix(overlap_error_above);


    Matrix *overlap_error_left = calc_overlap_error(
            src_image,
            &output_image,
            block_coords,
            output_coords,
            4,
            8
    );
    print_matrix(overlap_error_left);
    free_matrix(overlap_error_left);


    Matrix *cut = min_cut(
            src_image,
            &output_image,
            block_coords,
            output_coords,
            8,
            4,
            CORNER
    );

    print_matrix(cut);

    free_matrix(cut);
}


TEST(min_cut, corner_1) {

    Image *src_image = make_solid_image(8, 8, W);

    RGB output_data[144] = {
            R, R, R, R, R, R, R, R, R, R, R, R,
            R, R, R, R, R, R, R, R, R, R, R, R,
            R, R, R, R, R, R, R, R, R, R, R, R,
            R, R, R, R, R, R, R, R, R, R, R, R,

            R, R, R, R, R, R, R, W, R, R, R, R,
            R, R, R, R, R, R, R, W, R, R, R, W,
            R, R, R, R, R, R, R, W, R, R, W, R,
            R, R, R, R, W, W, W, W, W, W, R, R,

            R, R, R, R, R, R, R, W, R, R, R, R,
            R, R, R, R, R, R, R, W, R, R, R, R,
            R, R, R, R, R, R, W, R, R, R, R, R,
            R, R, R, R, R, W, R, R, R, R, R, R,
    };
    Image output_image = {12, 12, output_data};


    ImageCoordinates block_coords = {0, 0};
    ImageCoordinates output_coords = {4, 4};


    Matrix *overlap_error_above = calc_overlap_error(
            src_image,
            &output_image,
            block_coords,
            output_coords,
            8,
            4
    );
    print_matrix(overlap_error_above);
    free_matrix(overlap_error_above);


    Matrix *overlap_error_left = calc_overlap_error(
            src_image,
            &output_image,
            block_coords,
            output_coords,
            4,
            8
    );
    print_matrix(overlap_error_left);
    free_matrix(overlap_error_left);


    Matrix *cut = min_cut(
            src_image,
            &output_image,
            block_coords,
            output_coords,
            8,
            4,
            CORNER
    );

    print_matrix(cut);

    free_matrix(cut);
}