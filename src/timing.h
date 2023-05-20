//
// Created by emettaz on 01/05/23.
//
#ifndef TEAM08_SRC_TIMING_H_
#define TEAM08_SRC_TIMING_H_

#include "image.h"
#include <stdio.h>

void time_quilt(
        Image *(*func)(Image *image, int block_size, int out_num_blocks, int overlap_size),
        Image *img, int block_size, int out_num_blocks, int overlap_size
);

void multi_time_quilt(
        Image *(*func)(Image *img, int block_size, int out_num_blocks, int overlap_size),
        FILE *file, Image *img, int block_size_min, int block_size_max, int block_size_step,
        int out_num_blocks_min, int out_num_blocks_max, int out_num_blocks_step,
        int overlap_size_min, int overlap_size_max, int overlap_size_step
);

void multi_time_quilt_rgb(
        ImageRGB *(*func)(ImageRGB *img, int block_size, int out_num_blocks, int overlap_size),
        FILE *file, ImageRGB *img, int block_size_min, int block_size_max, int block_size_step,
        int out_num_blocks_min, int out_num_blocks_max, int out_num_blocks_step,
        int overlap_size_min, int overlap_size_max, int overlap_size_step
);

#endif //TEAM08_SRC_TIMING_H_
