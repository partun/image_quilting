#include "src/matrix.h"
#include "src/image.h"
#include <stdlib.h>
#include <limits.h>

int min_err_opt_7;

//int min_err_opt_7;
/*
 * computes the overlap error for all possible src image blocks
 */
void calc_errors_opt_7(
        int *errors,
        ImageRGB *src, ImageRGB *out, ImageCoordinates out_coord, int block_size,
        int overlap, Direction direction
) {

    int src_idx;
    int out_idx;
    int src_idx_off;
    int out_idx_off;
    int x_loop_lim;
    int err_idx = 0;

    int max_src_x = src->width - block_size;
    int max_src_y = src->height - block_size;

    int r_error, g_error, b_error;
    int r_error1, g_error1, b_error1;

    min_err_opt_7 = INT_MAX;


    for (int candidate_y = 0; candidate_y < max_src_y; ++candidate_y) {
        for (int candidate_x = 0; candidate_x < max_src_x; ++candidate_x) {


            int error = 0;

            int error0 = 0;
            int error1 = 0;
            int error2 = 0;
            int error3 = 0;
            int error4 = 0;
            int error5 = 0;

            if (direction == LEFT) {
                // error on left overlap
                src_idx = (candidate_y) * src->width + candidate_x;
                out_idx = (out_coord.y) * out->width + out_coord.x;
                src_idx_off = src->width-overlap;
                out_idx_off = out->width-overlap;
                
                x_loop_lim = overlap-1;

                for (int block_y = 0; block_y < block_size; ++block_y) {
                    int block_x = 0;
                    for (; block_x < x_loop_lim; block_x+=2) {
 
                        r_error = src->r_data[src_idx] - out->r_data[out_idx];
                        g_error = src->g_data[src_idx] - out->g_data[out_idx];
                        b_error = src->b_data[src_idx] - out->b_data[out_idx];

                        r_error1 = src->r_data[src_idx+1] - out->r_data[out_idx+1];
                        g_error1 = src->g_data[src_idx+1] - out->g_data[out_idx+1];
                        b_error1 = src->b_data[src_idx+1] - out->b_data[out_idx+1];

                        error0 += r_error * r_error;
                        error1 += g_error * g_error;
                        error2 += b_error * b_error;
                        
                        error3 += r_error1 * r_error1;
                        error4 += g_error1 * g_error1;
                        error5 += b_error1 * b_error1;
                        
                        src_idx+=2;
                        out_idx+=2;
                    }
                    for (; block_x < overlap; block_x++) {
                    
                        r_error = src->r_data[src_idx] - out->r_data[out_idx];
                        g_error = src->g_data[src_idx] - out->g_data[out_idx];
                        b_error = src->b_data[src_idx] - out->b_data[out_idx];

                    
                        error0 += r_error * r_error;
                        error1 += g_error * g_error;
                        error2 += b_error * b_error;
                        
                        src_idx++;
                        out_idx++;
                    
                    }
                    src_idx+=src_idx_off;
                    out_idx+=out_idx_off;
                }
            }
            if (direction == ABOVE || direction == CORNER) {
                // error on above overlap
                
                src_idx = (candidate_y) * src->width + candidate_x;
                out_idx = (out_coord.y) * out->width + out_coord.x;
                src_idx_off = src->width-block_size;
                out_idx_off = out->width-block_size;

                x_loop_lim = block_size-1;

                for (int block_y = 0; block_y < overlap; ++block_y) {
                    int block_x = 0;
                    for (; block_x < x_loop_lim; block_x+=2) {
 
                        r_error = src->r_data[src_idx] - out->r_data[out_idx];
                        g_error = src->g_data[src_idx] - out->g_data[out_idx];
                        b_error = src->b_data[src_idx] - out->b_data[out_idx];

                        r_error1 = src->r_data[src_idx+1] - out->r_data[out_idx+1];
                        g_error1 = src->g_data[src_idx+1] - out->g_data[out_idx+1];
                        b_error1 = src->b_data[src_idx+1] - out->b_data[out_idx+1];

                        error0 += r_error * r_error;
                        error1 += g_error * g_error;
                        error2 += b_error * b_error;
                        
                        error3 += r_error1 * r_error1;
                        error4 += g_error1 * g_error1;
                        error5 += b_error1 * b_error1;
                        
                        src_idx+=2;
                        out_idx+=2;
                    }
                    for (; block_x < block_size; block_x++) {
                    
                        r_error = src->r_data[src_idx] - out->r_data[out_idx];
                        g_error = src->g_data[src_idx] - out->g_data[out_idx];
                        b_error = src->b_data[src_idx] - out->b_data[out_idx];
                    
                        error0 += r_error * r_error;
                        error1 += g_error * g_error;
                        error2 += b_error * b_error;
                        
                        src_idx++;
                        out_idx++;
                    
                    }
                    src_idx+=src_idx_off;
                    out_idx+=out_idx_off;
                }
            }
            if (direction == CORNER) {
                // error left overlap without the section overlapping with the

                src_idx = (candidate_y+overlap) * src->width + candidate_x;
                out_idx = (out_coord.y+overlap) * out->width + out_coord.x;
                src_idx_off = src->width-overlap;
                out_idx_off = out->width-overlap;

                x_loop_lim = overlap-1;

                for (int block_y = overlap; block_y < block_size; ++block_y) {
                    int block_x = 0;
                    for (; block_x < x_loop_lim; block_x+=2) {
 
                        r_error = src->r_data[src_idx] - out->r_data[out_idx];
                        g_error = src->g_data[src_idx] - out->g_data[out_idx];
                        b_error = src->b_data[src_idx] - out->b_data[out_idx];

                        r_error1 = src->r_data[src_idx+1] - out->r_data[out_idx+1];
                        g_error1 = src->g_data[src_idx+1] - out->g_data[out_idx+1];
                        b_error1 = src->b_data[src_idx+1] - out->b_data[out_idx+1];

                        error0 += r_error * r_error;
                        error1 += g_error * g_error;
                        error2 += b_error * b_error;
                        
                        error3 += r_error1 * r_error1;
                        error4 += g_error1 * g_error1;
                        error5 += b_error1 * b_error1;
                        
                        src_idx+=2;
                        out_idx+=2;
                    }
                    for (; block_x < overlap; block_x++) {
                    
                        r_error = src->r_data[src_idx] - out->r_data[out_idx];
                        g_error = src->g_data[src_idx] - out->g_data[out_idx];
                        b_error = src->b_data[src_idx] - out->b_data[out_idx];

                    
                        error0 += r_error * r_error;
                        error1 += g_error * g_error;
                        error2 += b_error * b_error;
                        
                        src_idx++;
                        out_idx++;
                    
                    }
                    src_idx+=src_idx_off;
                    out_idx+=out_idx_off;
                }
            }

            error += error0 + error1 + error2 + error3 + error4 + error5;

            if (error < min_err_opt_7) {
                min_err_opt_7 = error;
            }

            errors[err_idx] = error;
            err_idx++;
        }
    }
}

/*
 * chooses a random block from the source image with an error within the tolerance
 */
ImageCoordinates find_best_block_opt_7(
        const int *errors, ImageRGB *src, int block_size,
        ImageCoordinates *candidate_coords
) {
    double tolerance = 1.1;

    int max_src_x = src->width - block_size;
    int max_src_y = src->height - block_size;
/*
    ImageCoordinates *candidate_coords = (ImageCoordinates *) malloc(
            max_src_x * max_src_y * sizeof(ImageCoordinates));
*/    int candidate_idx = 0;

    int threshold = (int) (min_err_opt_7 * tolerance);

    for (int y = 0; y < max_src_y; y++) {
        for (int x = 0; x < max_src_x; x++) {
            if (errors[y * max_src_x + x] <= threshold) {
                ImageCoordinates coord = {x, y};
                candidate_coords[candidate_idx] = coord;
                candidate_idx++;
            }
        }
    }
    // printf("min error = %d, threshold = %d, candidate_idx = %d\n",
    //     min_err, threshold, candidate_idx);

    ImageCoordinates best_block_coord = candidate_coords[rand() % candidate_idx];
    // free(candidate_coords);
    return best_block_coord;
}


