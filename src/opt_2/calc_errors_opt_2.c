#include "src/matrix.h"
#include "src/image.h"
#include <stdlib.h>
#include <limits.h>

#define BLOCK_SIZE 8

/*
 * computes the overlap error for all possible src image blocks
 */
int *
calc_errors_opt_2(
        Image *src, Image *out, ImageCoordinates out_coord, int block_size,
        int overlap, Direction direction
) {

    int src_idx;
    int out_idx;
    int err_idx = 0;

    int max_src_x = src->width - block_size;
    int max_src_y = src->height - block_size;

    int r_error0, g_error0, b_error0;
    int r_error1, g_error1, b_error1;
    int r_error2, g_error2, b_error2;
    RGB *src_rgb0, *src_rgb1, *src_rgb2;
    RGB *out_rgb0, *out_rgb1, *out_rgb2;

    int block_size_limit = block_size - 2;
    int overlap_limit = overlap - 2;

    int x_loop_lim, x_loop, y_loop_lim;

    int *errors = (int *) malloc(max_src_x * max_src_y * sizeof(int));

    for (int candidate_y = 0; candidate_y < max_src_y; ++candidate_y) {
        for (int candidate_x = 0; candidate_x < max_src_x; ++candidate_x) {

            int error = 0;
            int error1 = 0;
            int error2 = 0;
            int error3 = 0;
            int error4 = 0;
            int error5 = 0;
            int error6 = 0;
            int error7 = 0;
            int error8 = 0;

            if (direction == LEFT) {
                x_loop_lim = overlap_limit;
                x_loop = overlap;
                y_loop_lim = block_size;
            } else {
                x_loop_lim = block_size_limit;
                x_loop = block_size;
                y_loop_lim = overlap;
            }

            // error on left or top overlap

            

            for (int block_yy = 0; block_yy < y_loop_lim; block_yy += BLOCK_SIZE) {
                for (int block_xx = 0; block_xx < x_loop_lim; block_xx += BLOCK_SIZE) {
                    src_idx = (candidate_y + block_yy) * src->width + candidate_x + block_xx;
                    out_idx = (out_coord.y + block_yy) * out->width + out_coord.x + block_xx;

                    for (int block_y = block_yy; block_y < block_yy + BLOCK_SIZE && block_y < y_loop_lim; ++block_y) {

                        src_rgb0 = src->data + src_idx;
                        src_rgb1 = src_rgb0 + 1;
                        src_rgb2 = src_rgb0 + 2;
                        out_rgb0 = out->data + out_idx;
                        out_rgb1 = out_rgb0 + 1;
                        out_rgb2 = out_rgb0 + 2;

                        int block_x = block_xx;

                        for (; block_x < block_xx + BLOCK_SIZE && block_x < x_loop_lim; block_x += 3) {

                            r_error0 = src_rgb0->r - out_rgb0->r;
                            g_error0 = src_rgb0->g - out_rgb0->g;
                            b_error0 = src_rgb0->b - out_rgb0->b;
                            r_error1 = src_rgb1->r - out_rgb1->r;
                            g_error1 = src_rgb1->g - out_rgb1->g;
                            b_error1 = src_rgb1->b - out_rgb1->b;
                            r_error2 = src_rgb2->r - out_rgb2->r;
                            g_error2 = src_rgb2->g - out_rgb2->g;
                            b_error2 = src_rgb2->b - out_rgb2->b;

                            error += r_error0 * r_error0;
                            error1 += g_error0 * g_error0;
                            error2 += b_error0 * b_error0;

                            error3 += r_error1 * r_error1;
                            error4 += g_error1 * g_error1;
                            error5 += b_error1 * b_error1;

                            error6 += r_error2 * r_error2;
                            error7 += g_error2 * g_error2;
                            error8 += b_error2 * b_error2;

                            src_rgb0 += 3;
                            src_rgb1 += 3;
                            src_rgb2 += 3;
                            out_rgb0 += 3;
                            out_rgb1 += 3;
                            out_rgb2 += 3;
                        }
                        for (; block_x < block_xx + BLOCK_SIZE && block_x < x_loop; block_x++) {

                            r_error0 = src_rgb0->r - out_rgb0->r;
                            g_error0 = src_rgb0->g - out_rgb0->g;
                            b_error0 = src_rgb0->b - out_rgb0->b;
                            error += r_error0 * r_error0;
                            error1 += g_error0 * g_error0;
                            error2 += b_error0 * b_error0;
                            src_rgb0 += 1;
                            out_rgb0 += 1;
                        }
                        src_idx += src->width;
                        out_idx += out->width;

                    }
                }
            }

            if (direction == CORNER) {
                // error on the missing left overlap section in corner case

                
                
            for (int block_yy = overlap; block_yy < y_loop_lim; block_yy += BLOCK_SIZE) {
                for (int block_xx = 0; block_xx < x_loop_lim; block_xx += BLOCK_SIZE) {
                    src_idx = (candidate_y + block_yy) * src->width + candidate_x + block_xx;
                    out_idx = (out_coord.y + block_yy) * out->width + out_coord.x + block_xx;

                    for (int block_y = block_yy; block_y < block_yy + BLOCK_SIZE && block_y < block_size; ++block_y) {

                        src_rgb0 = src->data + src_idx;
                        src_rgb1 = src_rgb0 + 1;
                        src_rgb2 = src_rgb0 + 2;
                        out_rgb0 = out->data + out_idx;
                        out_rgb1 = out_rgb0 + 1;
                        out_rgb2 = out_rgb0 + 2;

                        int block_x = block_xx;

                        for (; block_x < block_xx + BLOCK_SIZE && block_x < overlap_limit; block_x += 3) {
                            r_error0 = src_rgb0->r - out_rgb0->r;
                            g_error0 = src_rgb0->g - out_rgb0->g;
                            b_error0 = src_rgb0->b - out_rgb0->b;
                            r_error1 = src_rgb1->r - out_rgb1->r;
                            g_error1 = src_rgb1->g - out_rgb1->g;
                            b_error1 = src_rgb1->b - out_rgb1->b;
                            r_error2 = src_rgb2->r - out_rgb2->r;
                            g_error2 = src_rgb2->g - out_rgb2->g;
                            b_error2 = src_rgb2->b - out_rgb2->b;

                            error += r_error0 * r_error0;
                            error1 += g_error0 * g_error0;
                            error2 += b_error0 * b_error0;

                            error3 += r_error1 * r_error1;
                            error4 += g_error1 * g_error1;
                            error5 += b_error1 * b_error1;

                            error6 += r_error2 * r_error2;
                            error7 += g_error2 * g_error2;
                            error8 += b_error2 * b_error2;

                            src_rgb0 += 3;
                            src_rgb1 += 3;
                            src_rgb2 += 3;
                            out_rgb0 += 3;
                            out_rgb1 += 3;
                            out_rgb2 += 3;
                        }

                        for (; block_x < block_xx + BLOCK_SIZE && block_x < overlap; ++block_x) {
                            r_error0 = src_rgb0->r - out_rgb0->r;
                            g_error0 = src_rgb0->g - out_rgb0->g;
                            b_error0 = src_rgb0->b - out_rgb0->b;
                            error += r_error0 * r_error0;
                            error1 += g_error0 * g_error0;
                            error2 += b_error0 * b_error0;
                            src_rgb0 += 1;
                            out_rgb0 += 1;
                        }
                        src_idx += src->width;
                        out_idx += out->width;
                    }
                }
            }
            }
            error += error1;
            error2 += error3;
            error4 += error5;
            error6 += error7;
            error += error2;
            error4 += error6;
            error += error4;
            errors[err_idx] = error + error8;
            err_idx++;
        }
    }
    return errors;
}

/*
 * chooses a random block from the source image with an error within the tolerance
 */
ImageCoordinates find_best_block_opt_2(const int *errors, Image *src, int block_size) {
    double tolerance = 1.3;
    int min_err = INT_MAX;
    int max_src_x = src->width - block_size;
    int max_src_y = src->height - block_size;

    ImageCoordinates *candidate_coords = (ImageCoordinates *) malloc(
            max_src_x * max_src_y * sizeof(ImageCoordinates));
    int candidate_idx = 0;

    for (int i = 0; i < max_src_x * max_src_y; i++) {
        int err = errors[i];
        if (err < min_err) {
            min_err = err;
        }
    }


    int threshold = (int) (min_err * tolerance);

    for (int y = 0; y < max_src_y; y++) {
        for (int x = 0; x < max_src_x; x++) {
            if (errors[y * max_src_x + x] <= threshold) {
                ImageCoordinates coord = {x, y};
                candidate_coords[candidate_idx] = coord;
                candidate_idx++;
            }
        }
    }
    // printf("min error = %d, threshold = %d, candidate_idx = %d\n",
    //     min_err, threshold, candidate_idx);

    ImageCoordinates best_block_coord = candidate_coords[rand() % candidate_idx];
    free(candidate_coords);
    return best_block_coord;
}


