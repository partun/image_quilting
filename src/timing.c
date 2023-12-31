//
// Created by emettaz on 01/05/23.
//

#include "timing.h"
#include "baseline/quilting.h"
#include "opt_4/quilting_opt_4.h"
#include "opt_5/quilting_opt_5.h"
#include "opt_6/quilting_opt_6.h"
#include "opt_6a/quilting_opt_6a.h"
#include "opt_6b/quilting_opt_6b.h"
#include "opt_6c/quilting_opt_6c.h"
#include "opt_6d/quilting_opt_6d.h"
#include "opt_8/quilting_opt_8.h"
#include "opt_9/quilting_opt_9.h"
#include "opt_10/quilting_opt_10.h"
#include <stdio.h>
#include "tsc_x86.h"
#include "load_image.h"

double CPU_FREQ_MHz = 2400;

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
    int number_of_iter = 10;
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
                secs = (double) acc / (CPU_FREQ_MHz * 1000000);
                fprintf(file, "%d,%d,%d,%llu,%f\n", i, j, k, acc, secs);
                printf("\nblock: %d output: %d overlap: %d cycles: %llu runtime: %f\n", i, j, k,
                       acc, secs);
            }
        }
    }
}

void multi_time_quilt_rgb(
        ImageRGB *(*func)(ImageRGB *img, int block_size, int out_num_blocks, int overlap_size),
        FILE *file, ImageRGB *img, int block_size_min, int block_size_max, int block_size_step,
        int out_num_blocks_min, int out_num_blocks_max, int out_num_blocks_step,
        int overlap_size_min, int overlap_size_max, int overlap_size_step
) {
    int number_of_iter = 10;
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
                    ImageRGB *quilt = func(img, i, j, k);
                    acc += stop_tsc(timer);
                    free_image_rgb(quilt);
                }
                acc = acc / number_of_iter;
                secs = (double) acc / (CPU_FREQ_MHz * 1000000);
                fprintf(file, "%d,%d,%d,%llu,%f\n", i, j, k, acc, secs);
                printf("\nblock: %d output: %d overlap: %d cycles: %llu runtime: %f\n", i, j, k,
                       acc, secs);
            }
        }
    }
}

void
time_fun(
        Image *img, FILE *file,
        Image *(*func)(Image *img, int block_size, int out_num_blocks, int overlap_size),
        char *revision
) {
    int block_size_min = 16;
    int block_size_max = 129;
    // int block_size_step = *2;

    int out_num_blocks = 12;
    int overlap_size;
    int number_of_iter = 5;
    myInt64 acc = 0;
    double secs = 0;
    myInt64 t;

    for (int i = block_size_min; i < block_size_max; i *= 2) {
        overlap_size = i / 8;
        for (int k = 2 * overlap_size;
             k <= i / 2; k += overlap_size) { // overlap max is the biggest power of two possible
//            int out_num_blocks = 1 + (int) ((400 - i) / (i - k));
//            printf("Block size = %d, overlap size = %d, number of blocks = %d\n", i, k,
//                   out_num_blocks);
            acc = 0;
            for (int l = 0; l < number_of_iter; l++) {
                myInt64 timer = start_tsc();
                Image *quilt = func(img, i, out_num_blocks, k);
                t = stop_tsc(timer);
                acc += t;
                printf("%f\n", (double) t / (CPU_FREQ_MHz * 1000000));
                free_image(quilt);
            }
            acc = acc / number_of_iter;
            secs = (double) acc / (CPU_FREQ_MHz * 1000000);
            fprintf(file, "%s,%d,%d,%d,%llu,%f\n", revision, i, out_num_blocks, k, acc, secs);
            printf("\nblock: %d output: %d overlap: %d cycles: %llu runtime: %f\n", i,
                   out_num_blocks, k,
                   acc, secs);
            fflush(file);
        }

    }
}

void time_funRGB(
        ImageRGB *img, FILE *file,
        ImageRGB *(*func)(
                ImageRGB *img,
                int block_size,
                int out_num_blocks,
                int overlap_size
        ),
        char *revision
) {
    int block_size_min = 16;
    int block_size_max = 129;
    // int block_size_step = *2;

    int out_num_blocks = 12;

    int overlap_size;
    int number_of_iter = 10;
    myInt64 acc = 0;
    double secs = 0;
    myInt64 t;

    for (int i = block_size_min; i < block_size_max; i *= 2) {
        overlap_size = i / 8;
        for (int k = 2 * overlap_size;
             k <= i / 2; k += overlap_size) { // overlap max is the biggest power of two possible
//            int out_num_blocks = 1 + (int) ((400 - i) / (i - k));
//            printf("Block size = %d, overlap size = %d, number of blocks = %d\n", i, k,
//                   out_num_blocks);
            acc = 0;
            for (int l = 0; l < number_of_iter; l++) {
                myInt64 timer = start_tsc();
                ImageRGB *quilt = func(img, i, out_num_blocks, k);
                t = stop_tsc(timer);
                acc += t;
                printf("%f\n", (double) t / (CPU_FREQ_MHz * 100000));
                free_image_rgb(quilt);
            }
            acc = acc / number_of_iter;
            secs = (double) acc / (CPU_FREQ_MHz * 1000000);
            fprintf(file, "%s,%d,%d,%d,%llu,%f\n", revision, i, out_num_blocks, k, acc, secs);
            printf("\nblock: %d output: %d overlap: %d cycles: %llu runtime: %f\n", i,
                   out_num_blocks, k,
                   acc, secs);
            fflush(file);
        }
    }
}

// 1. blue    2. red    3. green     start ~7:14
void time_total() {
    {
        FILE *file;
        file = fopen("timings/measure_red_intel.csv", "w");
        if (file == NULL) {
            printf("Error opening file \n");
            return;
        }

        fprintf(file, "revision,block_size,number_of_blocks_in_output_image,overlap_size,"
                      "number_of_cycles,"
                      "runtime_ms\n");
        char *input_path = "input/radishes.jpg";

        Image *img = read_image(input_path);
        time_fun(img, file, quilting_baseline, "baseline");
        free_image(img);

        img = read_image(input_path);
        time_fun(img, file, quilting_opt_4, "opt_4");
        free_image(img);

        ImageRGB *img_rgb = read_image_rgb(input_path);
        time_funRGB(img_rgb, file, quilting_opt_5, "opt_5");
        free_image_rgb(img_rgb);

        img_rgb = read_image_rgb(input_path);
        time_funRGB(img_rgb, file, quilting_opt_6a, "opt_6a");
        free_image_rgb(img_rgb);

        img_rgb = read_image_rgb(input_path);
        time_funRGB(img_rgb, file, quilting_opt_6b, "opt_6b");
        free_image_rgb(img_rgb);

        img_rgb = read_image_rgb(input_path);
        time_funRGB(img_rgb, file, quilting_opt_6c, "opt_6c");
        free_image_rgb(img_rgb);

        img_rgb = read_image_rgb(input_path);
        time_funRGB(img_rgb, file, quilting_opt_6d, "opt_6d");
        free_image_rgb(img_rgb);

        img_rgb = read_image_rgb(input_path);
        time_funRGB(img_rgb, file, quilting_opt_9, "opt_9");
        free_image_rgb(img_rgb);

        img_rgb = read_image_rgb(input_path);
        time_funRGB(img_rgb, file, quilting_opt_10, "opt_10");
        free_image_rgb(img_rgb);
        fclose(file);
    }
    {
        FILE *file;
        file = fopen("timings/measure_blue_intel.csv", "w");
        if (file == NULL) {
            printf("Error opening file \n");
            return;
        }

        fprintf(file, "revision,block_size,number_of_blocks_in_output_image,overlap_size,"
                      "number_of_cycles,"
                      "runtime_ms\n");
        char *input_path = "input/blue_wood.jpg";

        Image *img = read_image(input_path);
        time_fun(img, file, quilting_baseline, "baseline");
        free_image(img);

        img = read_image(input_path);
        time_fun(img, file, quilting_opt_4, "opt_4");
        free_image(img);


        ImageRGB *img_rgb = read_image_rgb(input_path);
        time_funRGB(img_rgb, file, quilting_opt_5, "opt_5");
        free_image_rgb(img_rgb);

        img_rgb = read_image_rgb(input_path);
        time_funRGB(img_rgb, file, quilting_opt_6a, "opt_6a");
        free_image_rgb(img_rgb);

        img_rgb = read_image_rgb(input_path);
        time_funRGB(img_rgb, file, quilting_opt_6b, "opt_6b");
        free_image_rgb(img_rgb);

        img_rgb = read_image_rgb(input_path);
        time_funRGB(img_rgb, file, quilting_opt_6c, "opt_6c");
        free_image_rgb(img_rgb);

        img_rgb = read_image_rgb(input_path);
        time_funRGB(img_rgb, file, quilting_opt_6d, "opt_6d");
        free_image_rgb(img_rgb);

        img_rgb = read_image_rgb(input_path);
        time_funRGB(img_rgb, file, quilting_opt_9, "opt_9");
        free_image_rgb(img_rgb);

        img_rgb = read_image_rgb(input_path);
        time_funRGB(img_rgb, file, quilting_opt_10, "opt_10");
        free_image_rgb(img_rgb);
        fclose(file);
    }

}





































