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
            //      printf("%.1f ", err);
            if (err < min_err) {
                min_err = err;
                inds[0] = y;
                inds[1] = x;
            }
        }
        //   printf("\n");
    }
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
void copy_blocks(RGB *im_data, RGB *out_data,
                 int im_width, int im_start_x, int im_start_y,
                 int out_width, int out_start_x, int out_start_y,
                 int pixels_to_copy_x, int pixels_to_copy_y){
//    printf("im_x : (%d + %d) * %d < xxx\n", im_start_x, pixels_to_copy_x-1, im_width);
//    printf("im_y : (%d + %d) < xxx\n", im_start_y, pixels_to_copy_y-1);
//    printf("out_x: (%d + %d) * %d < xxx\n", out_start_x, pixels_to_copy_x-1, out_width);
//    printf("out_y: (%d + %d) < xxx\n", out_start_y, pixels_to_copy_y-1);
    for (int i = 0; i < pixels_to_copy_x; i++) {
        for (int j = 0; j < pixels_to_copy_y; j++) {
//            printf("\n\ni = %d, j = %d\n", i, j);
//            printf("%d\n", (im_start_x + i) * im_width + im_start_y + j);
//            printf("%d\n", im_data[(im_start_x + i) * im_width + im_start_y + j].b);
//            printf("%d\n", (out_start_x + i) * out_width + out_start_y + j);
//            printf("%d\n", out_data[(out_start_x + i) * out_width + out_start_y + j].b);
            out_data[(out_start_x + i) * out_width + out_start_y + j] =
                im_data[(im_start_x + i) * im_width + im_start_y + j];
        }
    }
    return;
}


/**
 * Copy a block to the output image
 * @param im_data image copied from
 * @param out_data image copied to
 * @param im_width width of the input image
 * @param out_width width of the output image
 * @param over_x starting point of the block in the input image on the x axis
 * @param over_y starting point of the block in the input image on the y axis
 * @param bx end point of the block in the input image on the x axis
 * @param by end point of the block in the input image on the y axis
 * @param out_si starting point of the block in the output image on the x axis
 * @param out_sj starting point of the block in the output image on the y axis
 * @param im_si end point of the block in the output image on the x axis
 * @param im_sj end point of the block in the output image on the y axis
 */
void copy_blocks2(RGB *im_data, RGB *out_data,
                 int im_width, int out_width, 
                 int over_x, int over_y, int bx, int by,
                 int out_si, int out_sj, int im_si, int im_sj){

    /*
     * A better signature:
     * input image
     * output image
     * block size (#pixels to copy)
     * total width input (for the jump at the end of the line)
     * start height input
     * start width input
     * NO start height output: (?) entire picture should be copied
     * NO start height output
     */
    for (int i = over_y; i < by; i++) {
        for (int j = over_x; j < bx; j++) {
            out_data[(out_si + i) * out_width + out_sj + j] =
                    im_data[(im_si + i) * im_width + im_sj + j];
        }
    }
    return;
}


void
copy_section(Image *source_image, Image *output_image, ImageCoordinates source_coords, ImageCoordinates output_coords,
             int width, int height) {
    unsigned int source_idx;
    unsigned int output_idx;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            source_idx = (source_coords.y + y) * source_image->width + (source_coords.x + x);
            output_idx = (output_coords.y + y) * output_image->width + (output_coords.x + x);
            output_image->data[output_idx] = source_image->data[source_idx];
        }
    }
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

    for (int i = 0; i < out_num_blocks; i++) {
        for (int j = 0; j < out_num_blocks; j++) {
            printf("quilt: start, i=%d, j=%d\n", i, j);
//            printf("\npart1\nMax: %d, %d, %d, %d:::::: %d\n", out_size, out_size, block_size, block_size, out_data[0].r);

            int start_i = i * block_size - i * overlap_size;
            int start_j = j * block_size - j * overlap_size;

            double *errors;

            // top left corner
            if (i == 0 && j == 0) {
                int rand_block_row = rand() % num_blocks_h;
                int rand_block_col = rand() % num_blocks_w;
                //  printf("init index - y:%d x:%d\n", rand_block_row, rand_block_col);
//                copy_blocks(image->data, out_data, im_width, out_size,
//                            0, 0, block_size, block_size,
//                            0, 0, rand_block_row, rand_block_col);
//                printf("\npart0\nMax: %d, %d, %d, %d:::::: %d\n", im_height, im_width, out_size, out_size, out_data[19*238].r);
                copy_blocks(image->data, out_data, im_width, rand_block_row * block_size,
                            rand_block_col * block_size, out_size, 0, 0,
                            block_size, block_size);
//                store_image(output_image, "output/first.jpeg");
                continue;
            } else if (j == 0) {
//                copy_blocks(out_data, slice_data, out_size, block_size,
//                            0, 0, block_size, overlap_size,
//                            0, 0, start_i, 0);
//                printf("\nquilt: 2, i=%d, j=%d\n", i, j);
//                printf("\npart 2.1\nMax: %d, %d, %d, %d:::::: %d\n", out_size, out_size, block_size, block_size, out_data[0].r);
                copy_blocks(out_data, slice_data, out_size, 0,
                            (i) * (block_size - overlap_size), block_size, 0, 0,
                            block_size, overlap_size);
//                printf("quilt: 2, i=%d, j=%d\n", i, j);
                out_slice->data = slice_data;
                out_slice->width = block_size;
                out_slice->height = block_size;
//                printf("quilt: 2, i=%d, j=%d\n", i, j);
                errors = calc_errors(image, out_slice, block_size, overlap_size, "above");
                find_indexes(errors, inds, num_blocks_h, num_blocks_w);
//                copy_blocks(image->data, out_data, im_width, out_size,
//                            0, overlap_size, block_size, block_size,
//                            start_i, 0, inds[0], inds[1]);
//                printf("quilt: 2, i=%d, j=%d\n", i, j);
//                printf("\npart1\nMax: %d, %d, %d, %d:::::: %d\n", out_size, out_size, block_size, block_size, out_data[0].r);
                copy_blocks(image->data, out_data, im_width, inds[0] * (block_size - overlap_size) + overlap_size,
                            inds[1] * (block_size - overlap_size) + overlap_size, out_size, (block_size - overlap_size) * i, 0,
                            block_size - overlap_size, block_size);
                store_image(output_image, "output/first.jpeg");
//                printf("quilt: 2 end, i=%d, j=%d\n", i, j);
                free(errors);
                continue;
            } else if (i == 0) {
//                copy_blocks(out_data, slice_data, out_size, block_size,
//                            0, 0, overlap_size, block_size,
//                            0, 0, 0, start_j);
//                printf("\nquilt: 1, i=%d, j=%d\n", i, j);
//                printf("\npart1\nMax: %d, %d, %d, %d:::::: %d\n", out_size, out_size, block_size, block_size, out_data[0].r);
                copy_blocks(out_data, slice_data, out_size, (j+1) * (block_size - overlap_size),
                            0, block_size, 0, 0,
                            overlap_size, block_size);
//                printf("quilt: 1, i=%d, j=%d\n", i, j);
                out_slice->data = slice_data;
                out_slice->width = block_size;
                out_slice->height = block_size;
//                printf("quilt: 1, i=%d, j=%d\n", i, j);
                errors = calc_errors(image, out_slice, block_size, overlap_size, "left");
//                printf("quilt: 1, i=%d, j=%d\n", i, j);
                find_indexes(errors, inds, num_blocks_h, num_blocks_w);
//                copy_blocks(image->data, out_data, im_width, out_size,
//                            overlap_size, 0, block_size, block_size,
//                            0, start_j, inds[0], inds[1]);
//                inds[0] = rand() % num_blocks_h;
//                inds[1] = rand() % num_blocks_w;
//                printf("\npart2\nMax: %d, %d, %d, %d\n", im_width, im_height, out_size, out_size);
                copy_blocks(image->data, out_data, im_width, inds[0] * (block_size - overlap_size) + overlap_size,
                            inds[1] * (block_size - overlap_size) + overlap_size, out_size, 0, j * (block_size - overlap_size),
                            block_size - overlap_size, block_size);
//                printf("quilt: 1 end, i=%d, j=%d\n", i, j);
                free(errors);
                continue;
            } else {
//                copy_blocks(out_data, slice_data, out_size, block_size,
//                            0, 0, block_size, overlap_size,
//                            0, 0, start_i, 0);
                printf("quilt: 3, i=%d, j=%d\n", i, j);
                copy_blocks(out_data, slice_data, out_size, (j+1) * (block_size - overlap_size),
                            (i) * (block_size - overlap_size), block_size, 0, 0,
                            overlap_size, block_size);
                copy_blocks(out_data, slice_data, out_size, (j+1) * (block_size - overlap_size),
                            (i) * (block_size - overlap_size), block_size, overlap_size, 0,
                            block_size - overlap_size, overlap_size);
                printf("quilt: 3, i=%d, j=%d\n", i, j);
                out_slice->data = slice_data;
                out_slice->width = block_size;
                out_slice->height = block_size;
//                printf("quilt: 3, i=%d, j=%d\n", i, j);
                errors = calc_errors(image, out_slice, block_size, overlap_size, "above");
//                copy_blocks(out_data, slice_data, out_size, block_size,
//                            0, 0, overlap_size, block_size,
//                            0, 0, 0, start_j);
//                printf("quilt: 3, i=%d, j=%d\n", i, j);
                copy_blocks(image->data, out_data, im_width, inds[0] * (block_size - overlap_size) + overlap_size,
                            inds[1] * (block_size - overlap_size) + overlap_size, out_size, (block_size - overlap_size) * i, (block_size - overlap_size) * j,
                            block_size - overlap_size, block_size - overlap_size);
                printf("quilt: 3, i=%d, j=%d\n", i, j);
                out_slice->data = slice_data;
                out_slice->width = block_size;
                out_slice->height = block_size;
                double *errors_left = calc_errors(image, out_slice, block_size, overlap_size, "left");
                double *errors_corner = calc_errors(image, out_slice, block_size, overlap_size, "corner");
                for (int y = 0; y < num_blocks_h; y++) {
                    for (int x = 0; x < num_blocks_w; x++) {
                        int idx = y * num_blocks_w + x;
                        errors[idx] += (errors_left[idx] - errors_corner[idx]);
                    }
                }
                find_indexes(errors, inds, num_blocks_h, num_blocks_w);


//                ImageCoordinates block_coords = {inds[0], inds[1]};
//                ImageCoordinates out_coords = {start_i, start_j};
//                min_cut(image, output_image, block_coords, out_coords, block_size, overlap_size, CORNER);
//
//
//                copy_blocks(image->data, out_data, im_width, out_size,
//                            overlap_size, overlap_size, block_size, block_size,
//                            start_i, start_j, inds[0], inds[1]);
                printf("quilt: 3, i=%d, j=%d\n", i, j);

                copy_blocks(image->data, out_data, im_width, inds[0] * (block_size - overlap_size) + overlap_size,
                            inds[1] * (block_size - overlap_size) + overlap_size, out_size, 0, j * (block_size - overlap_size),
                            block_size - overlap_size, block_size);
                printf("quilt: 3 end, i=%d, j=%d\n", i, j);

                free(errors_left);
                free(errors_corner);
                free(errors);
            }
        }
        store_image(output_image, "output/first.jpeg");
    }

    free(slice_data);
    free(inds);


    return output_image;

}

