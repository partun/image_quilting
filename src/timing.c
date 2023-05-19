//
// Created by emettaz on 01/05/23.
//

#include "timing.h"
#include <stdio.h>
#include "tsc_x86.h"
#include "load_image.h"

double CPU_FREQ_MHz = 3600;

void time_quilt(
        Image *(*func)(Image *img, int block_size, int out_num_blocks, int overlap_size),
        Image *img, int block_size, int out_num_blocks, int overlap_size
) {
    myInt64 timer = start_tsc();
    Image *quilt = func(img, block_size, out_num_blocks, overlap_size);
    printf("Time = %llu\n", stop_tsc(timer));
    store_image(quilt, "output/quilt.jpeg");

}

void multi_time_quilt(
        Image *(*func)(Image *img, int block_size, int out_num_blocks, int overlap_size),
        FILE *file, Image *img, int block_size_min, int block_size_max, int block_size_step,
        int out_num_blocks_min, int out_num_blocks_max, int out_num_blocks_step,
        int overlap_size_min, int overlap_size_max, int overlap_size_step
) {
    int number_of_iter = 5;
    myInt64 acc = 0;
    double secs = 0;
    fprintf(file, "block_size,number_of_blocks_in_output_image,overlap_size,number_of_cycles,"
                  "runtime_ms\n");
    for (int i = block_size_min; i < block_size_max; i += block_size_step) {
        printf("\nBlock size: %d ", i);
        for (int j = out_num_blocks_min; j < out_num_blocks_max; j += out_num_blocks_step) {
            printf("\nNumber of blocks: %d ", j);
            for (int k = overlap_size_min; k < overlap_size_max; k += overlap_size_step) {
                acc = 0;
                for (int l = 0; l < number_of_iter; l++) {
                    myInt64 timer = start_tsc();
                    Image *quilt = func(img, i, j, k);
                    acc += stop_tsc(timer);
                    free_image(quilt);
                }
                acc = acc / number_of_iter;
                secs = acc / (CPU_FREQ_MHz * 1000000);
                fprintf(file, "%d,%d,%d,%llu,%f\n", i, j, k, acc, secs);
                printf("block: %d output: %d overlap: %d cycles: %llu runtime: %f\n", i, j, k,
                       acc, secs);
            }
        }
    }
}
