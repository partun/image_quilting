#include "load_image.h"
#include "calc_errors.h"
#include "quilting.h"
#include "min_cut.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <float.h>
#include <time.h>

#define RSEED 8

double tolerance = 0.3;
/**
 * Find a block with a suitable error
 * @param errors
 * @param inds index of the found block (not the pixel)
 * @param num_blocks_h
 * @param num_blocks_w
 */
void find_indexes(double *errors, int *inds,
                  int num_blocks_h, int num_blocks_w) {

    double min_err = DBL_MAX;
    inds[0] = -1;
    inds[1] = -1;
    //printf("errors: %d, %d\n",num_blocks_h, num_blocks_w);
    for (int y = 0; y < num_blocks_h; y++) {
        for (int x = 0; x < num_blocks_w; x++) {
            double err = errors[y * num_blocks_w + x];
            if (err < min_err) {
//                printf("%.1f ", err);
                min_err = err;
//                inds[0] = y;
//                inds[1] = x;
            }
        }
        //   printf("\n");
    }

    int index_h = rand() % num_blocks_h;
    int index_w = rand() % num_blocks_w;
    double threshold = min_err * (1 + tolerance);
    for (int y = 0; y < num_blocks_h; y++) {
        for (int x = 0; x < num_blocks_w; x++) {
            if (errors[((y+index_h) % num_blocks_h) * num_blocks_w + ((x + index_w) % num_blocks_w)] < threshold) {
                inds[0] = (y + index_h) % num_blocks_h;
                inds[1] = (x + index_w) % num_blocks_w;
                return;
            }
        }
        //   printf("\n");
    }

//    for (int y = index_h; y < 2 * num_blocks_h; y++) {
//        for (int x = index_w; x < 2 * num_blocks_w; x++) {
//            if (errors[(y % num_blocks_h) * num_blocks_w + (x % num_blocks_w)] < threshold) {
//                inds[0] = y;
//                inds[1] = x;
//                return;
//            }
//        }
//        //   printf("\n");
//    }

    //printf("index - y:%d x:%d\n", inds[0], inds[1]);
    if (inds[0] < 0 || inds[1] < 0) {
        fprintf(stderr, "error: find index negative: x=%d y=%d", inds[1], inds[0]);
        exit(1);
    }
    return;
}

/**
 * Copy a block to the output image
 * @param im_data image copied from
 * @param out_data image copied to
 * @param im_width width of the input image
 * @param im_start_x starting point of the block in the input image on the x axis
 * @param im_start_y starting point of the block in the input image on the y axis
 * @param out_width width of the output image
 * @param out_start_x starting point of the block in the output image on the x axis
 * @param out_start_y starting point of the block in the output image on the y axis
 * @param pixels_to_copy_x number of pixels to copy along the x axis
 * @param pixels_to_copy_y number of pixels to copy along the x axis
 */
void copy_blocks_cut(RGB *im_data, RGB *out_data,
                     int im_width, int im_start_x, int im_start_y,
                     int out_width, int out_start_x, int out_start_y,
                     int block_size, Matrix *cut) {
//    printf("Copy: %d:%d\n", cut->width, cut->height);
    for (int i = 0; i < block_size; i++) {
        for (int j = 0; j < block_size; j++) {
            if (cut->data[i * block_size + j] == 1) {
                out_data[(out_start_x + i) * out_width + out_start_y + j] =
                    im_data[(im_start_x + i) * im_width + im_start_y + j];
            } else if (cut->data[i * block_size + j] == 0) {
                out_data[(out_start_x + i) * out_width + out_start_y + j].b =
                    out_data[(out_start_x + i) * out_width + out_start_y + j].b / 2
                        + im_data[(im_start_x + i) * im_width + im_start_y + j].b / 2;
                out_data[(out_start_x + i) * out_width + out_start_y + j].g =
                    out_data[(out_start_x + i) * out_width + out_start_y + j].g / 2
                        + im_data[(im_start_x + i) * im_width + im_start_y + j].g / 2;
                out_data[(out_start_x + i) * out_width + out_start_y + j].r =
                    out_data[(out_start_x + i) * out_width + out_start_y + j].r / 2
                        + im_data[(im_start_x + i) * im_width + im_start_y + j].r / 2;
            }
        }
    }
    return;
}

/**
 * Copy a block to the output image
 * @param im_data image copied from
 * @param out_data image copied to
 * @param im_width width of the input image
 * @param im_start_x starting point of the block in the input image on the x axis
 * @param im_start_y starting point of the block in the input image on the y axis
 * @param out_width width of the output image
 * @param out_start_x starting point of the block in the output image on the x axis
 * @param out_start_y starting point of the block in the output image on the y axis
 * @param pixels_to_copy_x number of pixels to copy along the x axis
 * @param pixels_to_copy_y number of pixels to copy along the x axis
 */
void copy_blocks(RGB *im_data, RGB *out_data,
                 int im_width, int im_start_x, int im_start_y,
                 int out_width, int out_start_x, int out_start_y,
                 int pixels_to_copy_x, int pixels_to_copy_y) {
//    printf("Copy: %d:%d\n", pixels_to_copy_x, pixels_to_copy_y);
    for (int i = 0; i < pixels_to_copy_x; i++) {
        for (int j = 0; j < pixels_to_copy_y; j++) {
            out_data[(out_start_x + i) * out_width + out_start_y + j] =
                im_data[(im_start_x + i) * im_width + im_start_y + j];
        }
    }
    return;
}

/**
 * Compute the quilted image.
 * Note that both blocks and output image are square
 * @param image input image
 * @param block_size number of pixels for one block (blocks are square)
 * @param out_num_blocks number of blocks in the output image (defines the output's size)
 * @param overlap_size number of pixels overlapping between two blocks
 * @return the output image
 */
Image *quilting(Image *image, int block_size, int out_num_blocks, int overlap_size) {

    if (block_size < overlap_size) {
        fprintf(stderr, "error: quilting block size (%d) is smaller than the overlap size (%d)\n",
                block_size, overlap_size);
        exit(1);
    }

    int im_height = image->height;
    int im_width = image->width;

    // number of blocks used from the input image (they do overlap)
    int num_blocks_h = im_height / block_size;
    int num_blocks_w = im_width / block_size;

    if (num_blocks_h < 1 || num_blocks_w < 1) {
        fprintf(stderr, "error: quilting block size (%dx%d) exceeds one of the image dimensions (%dx%d)\n",
                block_size, block_size, im_height, im_width);
        exit(1);
    }

    double tolerance = 0.3;
    int out_size = out_num_blocks * (block_size - overlap_size);

    RGB *out_data = (RGB *) malloc(out_size * out_size * sizeof(RGB));
    RGB *slice_data = (RGB *) malloc(block_size * block_size * sizeof(RGB));
    Image *out_slice = malloc(sizeof(Image));

    Image *output_image = malloc(sizeof(Image));
    output_image->data = out_data;
    output_image->width = out_size;
    output_image->height = out_size;

    int seed = RSEED;
    //srand(seed);
    srand(time(NULL));

    int *inds = (int *) malloc(2 * sizeof(int));
    Matrix *mat;

    for (int i = 0; i < out_num_blocks; i++) {
        for (int j = 0; j < out_num_blocks; j++) {
//            printf("quilt: start, i=%d, j=%d\n", i, j);

            double *errors;

            // top left corner
            if (i == 0 && j == 0) {
                int rand_block_row = rand() % num_blocks_h;
                int rand_block_col = rand() % num_blocks_w;

                copy_blocks(image->data, out_data, im_width, rand_block_row * block_size,
                            rand_block_col * block_size, out_size, 0, 0,
                            block_size, block_size);
            } else if (j == 0) {
                copy_blocks(out_data, slice_data, out_size, 0,
                            (i) * (block_size - overlap_size), block_size, 0, 0,
                            block_size, overlap_size);
                out_slice->data = slice_data;
                out_slice->width = block_size;
                out_slice->height = block_size;
                errors = calc_errors(image, out_slice, block_size, overlap_size, "above");
                find_indexes(errors, inds, num_blocks_h, num_blocks_w);
                ImageCoordinates block_coords = {inds[0], inds[1]};
                ImageCoordinates out_coords = {i * (block_size - overlap_size), j * (block_size - overlap_size)};
                mat = min_cut(image, output_image, block_coords, out_coords, block_size, overlap_size, ABOVE);
//                print_matrix(mat);
//                printf("in: %d:%d, out: %d:%d\n", image->height, image->width, block_size-overlap_size, block_size);
                copy_blocks_cut(image->data,out_data,im_width,inds[0] * (block_size - overlap_size) + overlap_size,
                                inds[1] * (block_size - overlap_size) + overlap_size, out_size,
                                (block_size - overlap_size) * i, 0, block_size,mat);
//                store_image(output_image, "output/first.jpeg");
//                exit(0);
                free(errors);
                free(mat);
            } else if (i == 0) {
                copy_blocks(out_data, slice_data, out_size, (j + 1) * (block_size - overlap_size),
                            0, block_size, 0, 0,
                            overlap_size, block_size);
                out_slice->data = slice_data;
                out_slice->width = block_size;
                out_slice->height = block_size;
                errors = calc_errors(image, out_slice, block_size, overlap_size, "left");
                find_indexes(errors, inds, num_blocks_h, num_blocks_w);

//                inds[0] = rand() % num_blocks_h;
//                inds[1] = rand() % num_blocks_w;
                ImageCoordinates block_coords = {inds[0], inds[1]};
                ImageCoordinates out_coords = {i * (block_size - overlap_size), j * (block_size - overlap_size)};
                mat = min_cut(image, output_image, block_coords, out_coords, block_size, overlap_size, LEFT);

                copy_blocks_cut(image->data, out_data,
                            im_width,inds[0] * (block_size - overlap_size) + overlap_size,
                            inds[1] * (block_size - overlap_size) + overlap_size,
                            out_size,0,j * (block_size - overlap_size),block_size, mat);
                free(errors);
                free(mat);
            } else {

                copy_blocks(out_data, slice_data, out_size, (j + 1) * (block_size - overlap_size),
                            (i) * (block_size - overlap_size), block_size, 0, 0,
                            overlap_size, block_size);
                copy_blocks(out_data, slice_data, out_size, (j + 1) * (block_size - overlap_size),
                            (i) * (block_size - overlap_size), block_size, overlap_size, 0,
                            block_size - overlap_size, overlap_size);
                out_slice->data = slice_data;
                out_slice->width = block_size;
                out_slice->height = block_size;
//                find_indexes(errors_above, inds, num_blocks_h, num_blocks_w);

                ImageCoordinates block_coords = {inds[0], inds[1]};
                ImageCoordinates out_coords = {i * (block_size - overlap_size), j * (block_size - overlap_size)};
//                mat = min_cut(image, output_image, block_coords, out_coords, block_size, overlap_size, ABOVE);
//                inds[0] = rand() % num_blocks_h;
//                inds[1] = rand() % num_blocks_w;
//                copy_blocks_cut(image->data,
//                            out_data,
//                            im_width,
//                            inds[0] * (block_size - overlap_size) + overlap_size,
//                            inds[1] * (block_size - overlap_size) + overlap_size,
//                            out_size,
//                            (block_size - overlap_size) * i,
//                            (block_size - overlap_size) * j,
//                            block_size,mat);
//                out_slice->data = slice_data;
//                out_slice->width = block_size;
//                out_slice->height = block_size;
                double *errors_above = calc_errors(image, out_slice, block_size, overlap_size, "above");
                double *errors_left = calc_errors(image, out_slice, block_size, overlap_size, "left");
                double *errors_corner = calc_errors(image, out_slice, block_size, overlap_size, "corner");
                for (int y = 0; y < num_blocks_h; y++) {
                    for (int x = 0; x < num_blocks_w; x++) {
                        int idx = y * num_blocks_w + x;
                        errors_above[idx] += (errors_left[idx] + errors_above[idx] - errors_corner[idx]);
                    }
                }
                find_indexes(errors_above, inds, num_blocks_h, num_blocks_w);

                block_coords = (ImageCoordinates) {inds[0], inds[1]};
                out_coords = (ImageCoordinates) {i * (block_size - overlap_size), j * (block_size - overlap_size)};
                mat = min_cut(image, output_image, block_coords, out_coords, block_size, overlap_size, CORNER);

                copy_blocks_cut(image->data,
                            out_data,
                            im_width,
                            inds[0] * (block_size - overlap_size) + overlap_size,
                            inds[1] * (block_size - overlap_size) + overlap_size,
                            out_size,
                            i * (block_size - overlap_size),
                            j * (block_size - overlap_size),
                            block_size, mat);

                free(errors_left);
                free(errors_corner);
//                free(errors);
                free(mat);
//                printf(" %d\n", i);
//                store_image(output_image, "output/first.jpeg");
            }
        }
    }

    free(slice_data);
    free(inds);

    return output_image;

}

