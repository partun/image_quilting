//
// Created by Oleh Kuzyk on 10.03.23.
//
#include "load_image.h"
#include "calc_errors.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

double *calc_errors(Image *image, Image *out_slice, int block_size, int overlap_size, Direction direction) {
    int image_height = image->height;
    int image_width = image->width;

    int num_blocks_h = image_height - block_size;
    int num_blocks_w = image_width - block_size;

    int errors_size = num_blocks_h * num_blocks_w;
    double *errors = malloc(errors_size * sizeof(double));

    for (int y = 0; y < num_blocks_h; y++) {
        for (int x = 0; x < num_blocks_w; x++) {
            int y_start = y, x_start = x;
            int y_end, x_end;
            if (direction == ABOVE) {
                y_end = y_start + overlap_size - 1;
                x_end = x_start + block_size - 1;
            } else if (direction == LEFT) {
                y_end = y_start + block_size - 1;
                x_end = x_start + overlap_size - 1;
            } else if (direction == CORNER) {
                y_end = y_start + overlap_size - 1;
                x_end = x_start + overlap_size - 1;
            }

            // Extract block from image
            RGB *block_data = (RGB *) malloc(block_size * block_size * sizeof(RGB));
            for (int by = y_start; by <= y_end; by++) {
                for (int bx = x_start; bx <= x_end; bx++) {
                    int idx = (by - y_start) * block_size + (bx - x_start);
                    block_data[idx] = image->data[by * image_width + bx];
                }
            }
//            Image block = {block_size, block_size, block_data};

            int error = 0;
            for (int yy = 0; yy < y_end - y_start + 1; yy++) {
                for (int xx = 0; xx < x_end - x_start + 1; xx++) {
                    int idx = yy * block_size + xx;
                    int pixel_diff[3] = {block_data[idx].r - out_slice->data[idx].r,
                                         block_data[idx].g - out_slice->data[idx].g,
                                         block_data[idx].b - out_slice->data[idx].b};
                    error += pixel_diff[0] * pixel_diff[0] +
                             pixel_diff[1] * pixel_diff[1] +
                             pixel_diff[2] * pixel_diff[2];
                }
            }
            errors[y * num_blocks_w + x] = error;

            free(block_data);
        }
    }
    return errors;
}