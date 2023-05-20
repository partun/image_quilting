#include "src/opt_1/calc_errors_opt.h"
#include "src/opt_1/min_cut_opt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RSEED 8


/*
 * copies block from source image to out image
 */
void copy_block_opt(
        Image *src, ImageCoordinates src_coord, Image *out,
        ImageCoordinates out_coord, int block_size
) {
    unsigned int src_idx;
    unsigned int out_idx;

    for (int y = 0; y < block_size; ++y) {
        for (int x = 0; x < block_size; ++x) {
            src_idx = (y + src_coord.y) * src->width + x + src_coord.x;
            out_idx = (y + out_coord.y) * out->width + x + out_coord.x;

            out->data[out_idx] = src->data[src_idx];
        }
    }
}

void mask_copy_block_opt(
        Image *source_image, Image *output_image, Matrix *mask,
        ImageCoordinates block_coords,
        ImageCoordinates output_coords, int block_size
) {

    unsigned int source_idx;
    unsigned int output_idx;
    unsigned int mask_idx;

    for (int y = 0; y < block_size; y++) {
        for (int x = 0; x < block_size; x++) {
            mask_idx = y * mask->width + x;
            source_idx =
                    (block_coords.y + y) * source_image->width + (block_coords.x + x);
            output_idx =
                    (output_coords.y + y) * output_image->width + (output_coords.x + x);

            if (mask->data[mask_idx] == 1) {
                output_image->data[output_idx] = source_image->data[source_idx];
            } else if (mask->data[mask_idx] == 0) {
                output_image->data[output_idx].b =
                        output_image->data[output_idx].b / 2 +
                        source_image->data[source_idx].b / 2;
                output_image->data[output_idx].g =
                        output_image->data[output_idx].g / 2 +
                        source_image->data[source_idx].g / 2;
                output_image->data[output_idx].r =
                        output_image->data[output_idx].r / 2 +
                        source_image->data[source_idx].r / 2;
            }
        }
    }
}

Image *quilting_opt(
        Image *image, int block_size, int out_blocks,
        int overlap_size
) {

    srand(time(NULL));
    if (block_size < overlap_size) {
        fprintf(stderr,
                "error: quilting block size (%d) is smaller than the overlap size "
                "(%d)\n",
                block_size, overlap_size);
        exit(1);
    }

    if (image->height < block_size || image->width < block_size) {
        fprintf(stderr,
                "error: quilting block size (%dx%d) exceeds one of the image "
                "dimensions (%dx%d)\n",
                block_size, block_size, image->height, image->width);
        exit(1);
    }

    int out_size = out_blocks * (block_size - overlap_size) + overlap_size;
    Image *out_im = (Image *) malloc(sizeof(Image));
    out_im->width = out_size;
    out_im->height = out_size;
    out_im->data = (RGB *) malloc(out_size * out_size * sizeof(RGB));

    for (int y = 0; y < out_blocks; y++) {
        for (int x = 0; x < out_blocks; x++) {

            // coordinates of the next block in the output image
            ImageCoordinates out_coord = {x * (block_size - overlap_size),
                                          y * (block_size - overlap_size)};

            if (x == 0 && y == 0) {
                // case: corner
                // start out image with a random block of the src image

                ImageCoordinates src_coord = {rand() % (image->width - block_size),
                                              rand() % (image->height - block_size)};
                copy_block_opt(image, src_coord, out_im, out_coord, block_size);
            } else if (y == 0) {
                // case left overlap
                int *errors = calc_errors_opt(image, out_im, out_coord, block_size,
                                              overlap_size, LEFT);
                ImageCoordinates src_coord = find_best_block_opt(errors, image, block_size);
                free(errors);
                Matrix *cut = min_cut_opt(
                        image,
                        out_im,
                        src_coord,
                        out_coord,
                        block_size,
                        overlap_size,
                        LEFT
                );
                mask_copy_block_opt(
                        image,
                        out_im,
                        cut,
                        src_coord,
                        out_coord,
                        block_size
                );
                free_matrix(cut);
            } else if (x == 0) {
                // case left overlap
                int *errors = calc_errors_opt(image, out_im, out_coord, block_size,
                                              overlap_size, ABOVE);
                ImageCoordinates src_coord = find_best_block_opt(errors, image, block_size);
                free(errors);
                Matrix *cut = min_cut_opt(
                        image,
                        out_im,
                        src_coord,
                        out_coord,
                        block_size,
                        overlap_size,
                        ABOVE
                );
                mask_copy_block_opt(
                        image,
                        out_im,
                        cut,
                        src_coord,
                        out_coord,
                        block_size
                );
                free_matrix(cut);
            } else {
                // case left overlap
                int *errors = calc_errors_opt(image, out_im, out_coord, block_size,
                                              overlap_size, CORNER);
                ImageCoordinates src_coord = find_best_block_opt(errors, image, block_size);
                free(errors);
                Matrix *cut = min_cut_opt(
                        image,
                        out_im,
                        src_coord,
                        out_coord,
                        block_size,
                        overlap_size,
                        CORNER
                );
                mask_copy_block_opt(
                        image,
                        out_im,
                        cut,
                        src_coord,
                        out_coord,
                        block_size
                );
                free_matrix(cut);
            }
        }
    }

    return out_im;
}
