//
// Created by Oleh Kuzyk on 10.03.23.
//
#include "load_image.h"
#include "calc_errors.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


//double *calc_errors2(Image *source_image, Image *output_image, int block_size, int overlap, Direction direction) {
//    int source_image_height = image->height;
//    int source_image_width = image->width;
//
//    int possible_blocks = (source_image_height - block_size) * (source_image_width - block_size);
//    double *block_errors = (double *) malloc(possible_blocks * sizeof(double));
//
//    for (int y = 0; y < source_image_height; ++y) {
//
//    }
//
//
//}

double *calc_errors(Image *image, Image *out_slice, int block_size, int overlap_size, char *direction) {
    int image_height = image->height;
    int image_width = image->width;

    int num_blocks_h = image_height - block_size;
    int num_blocks_w = image_width - block_size;

    int errors_size = num_blocks_h * num_blocks_w;
    double *errors = malloc(errors_size * sizeof(double));

//    printf("calc: 1\n");
    for (int y = 0; y < num_blocks_h; y++) {
        for (int x = 0; x < num_blocks_w; x++) {
//            printf("calc: 2, y=%d, x=%d\n", y, x);
            int y_start = y, x_start = x;
            int y_end, x_end;
            if (strcmp(direction, "above") == 0) {
                y_end = y_start + overlap_size - 1;
                x_end = x_start + block_size - 1;
            } else if (strcmp(direction, "left") == 0) {
                y_end = y_start + block_size - 1;
                x_end = x_start + overlap_size - 1;
            } else if (strcmp(direction, "corner") == 0) {
                y_end = y_start + overlap_size - 1;
                x_end = x_start + overlap_size - 1;
            } else {
                // Invalid slice type
                free(errors);
                return NULL;
            }
//            printf("calc: 3, y=%d, x=%d\n", y, x);

            // Extract block from image
            RGB *block_data = (RGB *) malloc(block_size * block_size * sizeof(RGB));
//            printf("calc: 4, y=%d, x=%d\n", y, x);
            for (int by = y_start; by <= y_end; by++) {
                for (int bx = x_start; bx <= x_end; bx++) {
//                    printf("calc: 4, by=%d, bx=%d\n", by, bx);
                    int idx = (by - y_start) * block_size + (bx - x_start);
//                    printf("calc: 4, by=%d, bx=%d\n", by, bx);
                    block_data[idx] = image->data[by * image_width + bx];
//                    printf("calc: 4, by=%d, bx=%d\n", by, bx);
                }
            }
            Image block = {block_size, block_size, block_data};
//            printf("calc: 5, y=%d, x=%d\n", y, x);

            double error = 0.0;
            for (int yy = 0; yy < y_end - y_start; yy++) {
                for (int xx = 0; xx < x_end - x_start; xx++) {
                    int idx = yy * block_size + xx;
                    RGB pixel_diff = {block_data[idx].r - out_slice->data[idx].r,
                                      block_data[idx].g - out_slice->data[idx].g,
                                      block_data[idx].b - out_slice->data[idx].b};
                    error += pixel_diff.r * pixel_diff.r +
                             pixel_diff.g * pixel_diff.g +
                             pixel_diff.b * pixel_diff.b;
                }
            }
//            printf("calc: 6, y=%d, x=%d\n", y, x);

            errors[y * num_blocks_w + x] = error;

            free(block_data);
        }
    }

    return errors;
}
