//
// Created by Oleh Kuzyk on 10.03.23.
//
#include "calc_errors.h"
#include "image.h"
#include <stdlib.h>
#include <float.h>

/*
 * computes the overlap error for all possible src image blocks
 */
double *
calc_errors(Image *src, Image *out, ImageCoordinates out_coord, int block_size,
            int overlap, Direction direction) {

    unsigned int src_idx;
    unsigned int out_idx;
    int err_idx = 0;

    unsigned int max_src_x = src->width - block_size;
    unsigned int max_src_y = src->height - block_size;

    double *errors = (double *) malloc(max_src_x * max_src_y * sizeof(double));


    for (int candidate_y = 0; candidate_y < max_src_y; ++candidate_y) {
        for (int candidate_x = 0; candidate_x < max_src_x; ++candidate_x) {


            double error = 0;

            if (direction == LEFT) {
                // error on left overlap
                for (int block_y = 0; block_y < block_size; ++block_y) {
                    for (int block_x = 0; block_x < overlap; ++block_x) {
                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;

                        error += rgb_sq_error(src->data + src_idx, out->data + out_idx);
                    }
                }
            }
            if (direction == ABOVE || direction == CORNER) {
                // error on above overlap
                for (int block_y = 0; block_y < overlap; ++block_y) {
                    for (int block_x = 0; block_x < block_size; ++block_x) {
                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;

                        error += rgb_sq_error(src->data + src_idx, out->data + out_idx);
                    }
                }
            }
            if (direction == CORNER) {
                // error left overlap without the section overlapping with the
                for (int block_y = overlap; block_y < block_size; ++block_y) {
                    for (int block_x = 0; block_x < overlap; ++block_x) {
                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;

                        error += rgb_sq_error(src->data + src_idx, out->data + out_idx);
                    }
                }
            }

            errors[err_idx] = error;
            err_idx++;
        }
    }
    return errors;
}

/*
 * chooses a random block from the source image with an error within the tolerance
 */
ImageCoordinates find_best_block(const double *errors, Image *src, int block_size) {
    double tolerance = 1.3;
    double min_err = DBL_MAX;
    unsigned int max_src_x = src->width - block_size;
    unsigned int max_src_y = src->height - block_size;

    ImageCoordinates *candidate_coords = (ImageCoordinates *) malloc(max_src_x * max_src_y * sizeof(ImageCoordinates));
    int candidate_idx = 0;

    for (int i = 0; i < max_src_x * max_src_y; i++) {
        double err = errors[i];
        if (err < min_err) {
            min_err = err;
        }
    }


    double threshold = min_err * tolerance;

    for (int y = 0; y < max_src_y; y++) {
        for (int x = 0; x < max_src_x; x++) {
            if (errors[y * max_src_x + x] <= threshold) {
                ImageCoordinates coord = {x, y};
                candidate_coords[candidate_idx] = coord;
                candidate_idx++;
            }
        }
    }

    ImageCoordinates best_block_coord = candidate_coords[rand() % candidate_idx];
    free(candidate_coords);
    return best_block_coord;
}


