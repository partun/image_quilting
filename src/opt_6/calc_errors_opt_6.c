#include "src/matrix.h"
#include "src/image.h"
#include <stdlib.h>
#include <limits.h>

int min_err_opt_6;

//int min_err_opt_6;
/*
 * computes the overlap error for all possible src image blocks
 */
void calc_errors_opt_6(
        int *errors,
        ImageRGB *src, ImageRGB *out, ImageCoordinates out_coord, int block_size,
        int overlap, Direction direction
) {

    int src_idx;
    int out_idx;
    int err_idx = 0;

    int max_src_x = src->width - block_size;
    int max_src_y = src->height - block_size;

    int r_error, g_error, b_error;

    min_err_opt_6 = INT_MAX;


    for (int candidate_y = 0; candidate_y < max_src_y; ++candidate_y) {
        for (int candidate_x = 0; candidate_x < max_src_x; ++candidate_x) {


            int error = 0;

            if (direction == LEFT) {
                // error on left overlap
                for (int block_y = 0; block_y < block_size; ++block_y) {
                    for (int block_x = 0; block_x < overlap; ++block_x) {
                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;

                        r_error = src->r_data[src_idx] - out->r_data[out_idx];
                        g_error = src->g_data[src_idx] - out->g_data[out_idx];
                        b_error = src->b_data[src_idx] - out->b_data[out_idx];

                        r_error = r_error * r_error;
                        g_error = g_error * g_error;
                        b_error = b_error * b_error;


                        error += r_error + g_error + b_error;
                    }
                }
            }
            if (direction == ABOVE || direction == CORNER) {
                // error on above overlap
                for (int block_y = 0; block_y < overlap; ++block_y) {
                    for (int block_x = 0; block_x < block_size; ++block_x) {
                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;

                        r_error = src->r_data[src_idx] - out->r_data[out_idx];
                        g_error = src->g_data[src_idx] - out->g_data[out_idx];
                        b_error = src->b_data[src_idx] - out->b_data[out_idx];

                        r_error = r_error * r_error;
                        g_error = g_error * g_error;
                        b_error = b_error * b_error;


                        error += r_error + g_error + b_error;
                    }
                }
            }
            if (direction == CORNER) {
                // error left overlap without the section overlapping with the
                for (int block_y = overlap; block_y < block_size; ++block_y) {
                    for (int block_x = 0; block_x < overlap; ++block_x) {
                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;

                        r_error = src->r_data[src_idx] - out->r_data[out_idx];
                        g_error = src->g_data[src_idx] - out->g_data[out_idx];
                        b_error = src->b_data[src_idx] - out->b_data[out_idx];

                        r_error = r_error * r_error;
                        g_error = g_error * g_error;
                        b_error = b_error * b_error;


                        error += r_error + g_error + b_error;
                    }
                }
            }

            if (error < min_err_opt_6) {
                min_err_opt_6 = error;
            }

            errors[err_idx] = error;
            err_idx++;
        }
    }
}

/*
 * chooses a random block from the source image with an error within the tolerance
 */
ImageCoordinates find_best_block_opt_6(
        const int *errors, ImageRGB *src, int block_size,
        ImageCoordinates *candidate_coords
) {
    double tolerance = 1.1;

    int max_src_x = src->width - block_size;
    int max_src_y = src->height - block_size;
/*
    ImageCoordinates *candidate_coords = (ImageCoordinates *) malloc(
            max_src_x * max_src_y * sizeof(ImageCoordinates));
*/    int candidate_idx = 0;

    int threshold = (int) (min_err_opt_6 * tolerance);

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
    // free(candidate_coords);
    return best_block_coord;
}


