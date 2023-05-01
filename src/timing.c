//
// Created by emettaz on 01/05/23.
//

#include "quilting.h"
#include "timing.h"
#include <stdio.h>
#include "tsc_x86.h"

void time_quilt(Image *(*func)(Image *img, int block_size, int out_num_blocks, int overlap_size),
                Image *img, int block_size, int out_num_blocks, int overlap_size) {
    myInt64 timer = start_tsc();
    Image *quilt = func(img, block_size, out_num_blocks, overlap_size);
    printf("Time = %llu\n", stop_tsc(timer));
    store_image(quilt, "output/quilt.jpeg");

}

void multi_time_quilt(Image *(*func)(Image *img, int block_size, int out_num_blocks, int overlap_size),
                      FILE *file, Image *img, int block_size_min, int block_size_max, int block_size_step,
                      int out_num_blocks_min, int out_num_blocks_max, int out_num_blocks_step,
                      int overlap_size_min, int overlap_size_max, int overlap_size_step) {
    int number_of_iter = 5;
    myInt64 acc = 0;
    fprintf(file, "//Block_size, number_of_blocks_in_output_image, overlap_size, number_of_cycles\n");
    for (int i = block_size_min; i < block_size_max; i+= block_size_step) {
        printf("\nBlock size: %d ", i);
        for (int j = out_num_blocks_min; j < out_num_blocks_max; j += out_num_blocks_step) {
            printf("\nNumber of blocks: %d ", j);
            for (int k = overlap_size_min; k < overlap_size_max; k += overlap_size_step) {
                acc = 0;
                for (int l = 0; l < number_of_iter; l++) {
                    myInt64 timer = start_tsc();
                    Image *quilt = func(img, i, j, k);
                    acc += stop_tsc(timer);
                    printf("%d ", quilt->data->b);
                }
                fprintf(file, "%d, %d, %d, %llu,\n", i, j, k, acc / number_of_iter);
            }
        }
    }
}
