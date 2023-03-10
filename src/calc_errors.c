//
// Created by Oleh Kuzyk on 10.03.23.
//
#include "load_image.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

double* calc_errors(Image *image, Image* out_slice, int block_size, int overlap_size, char* direction) {
    int image_height = image->height;
    int image_width = image->width;

    int num_blocks_h = image_height - block_size;
    int num_blocks_w = image_width - block_size;

    int errors_size = num_blocks_h * num_blocks_w;
    double* errors = malloc(errors_size * sizeof(double));

    for (int y = 0; y < num_blocks_h; y++) {
        for (int x = 0; x < num_blocks_w; x++) {
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

            // Extract block from image
            RGB* block_data = (RGB*) malloc(block_size * block_size * sizeof(RGB));
            for (int by = y_start; by <= y_end; by++) {
                for (int bx = x_start; bx <= x_end; bx++) {
                    int idx = (by - y_start) * block_size + (bx - x_start);
                    block_data[idx] = image->data[by * image_width + bx];
                }
            }
            Image block = {block_size, block_size, block_data};

            double error = 0.0;
            for (int yy = 0; yy < block_size; yy++) {
                for (int xx = 0; xx < block_size; xx++) {
                    int idx = yy * block_size + xx;
                    RGB pixel_diff = {block_data[idx].r - out_slice->data[idx].r,
                                      block_data[idx].g - out_slice->data[idx].g,
                                      block_data[idx].b - out_slice->data[idx].b};
                    error += pixel_diff.r * pixel_diff.r +
                             pixel_diff.g * pixel_diff.g +
                             pixel_diff.b * pixel_diff.b;
                }
            }

            errors[y * num_blocks_w + x] = error;

            free(block_data);
        }
    }

    return errors;
}
