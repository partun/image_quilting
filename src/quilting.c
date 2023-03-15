#include "load_image.h"
#include "calc_errors.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <float.h>
#include <time.h>

#define RSEED 8

void find_indexes(double *errors, int *inds, 
                    int num_blocks_h, int num_blocks_w){
                    
    double min_err = DBL_MAX;
    inds[0]=-1;
    inds[1]=-1;
    //printf("errors: %d, %d\n",num_blocks_h, num_blocks_w);
    for (int y = 0; y < num_blocks_h; y++) {
        for (int x = 0; x < num_blocks_w; x++){
            double err = errors[y*num_blocks_w + x];
    //      printf("%.1f ", err);
            if(err < min_err){
                min_err = err;
                inds[0] = y;
                inds[1] = x;
            }       
        }
    //   printf("\n");
    } 
    //printf("index - y:%d x:%d\n", inds[0], inds[1]);
    if(inds[0] < 0 || inds[1] < 0){
        fprintf(stderr, "error: find index negative: x=%d y=%d", inds[1], inds[0]);
        exit(1);
    }
    return;               
} 

void copy_blocks(RGB *im_data, RGB *out_data, 
                 int im_width, int out_width, 
                 int over_x, int over_y, int bx, int by,
                 int out_si, int out_sj, int im_si, int im_sj){
                 
    for(int i = over_y; i < by; i++){
        for(int j = over_x; j < bx; j++){
            out_data[(out_si+i)*out_width + out_sj+j] = 
                im_data[(im_si+i)*im_width + im_sj+j];
        }
    }  
    return;
}

Image* quilting(Image *image, int block_size, int out_num_blocks, int overlap_size){
    
    int im_height = image->height;
    int im_width = image->width;

    int num_blocks_h = im_height - block_size;
    int num_blocks_w = im_width - block_size;
    
    if(num_blocks_h < 1 || num_blocks_w < 1){
        fprintf(stderr, "error: quilting block size (%dx%d) exceeds one of the image dimensions (%dx%d)\n",
        block_size, block_size, im_height, im_width);
        exit(1);
    }  
       
    double tolerance = 0.3;
    int out_size = out_num_blocks * block_size - (out_num_blocks-1)*overlap_size;
    
    RGB* out_data   = (RGB*) malloc(out_size * out_size * sizeof(RGB));
    RGB* slice_data = (RGB*) malloc(block_size * block_size * sizeof(RGB));
    Image *out_slice = malloc(sizeof(Image));
    
    int seed = RSEED;
    //srand(seed);
    srand(time(NULL));
    
    int *inds = (int*) malloc(2 * sizeof(int));
    
    for(int i = 0; i < out_num_blocks; i++){
        for(int j = 0; j < out_num_blocks; j++){
            
            int start_i = i*block_size - i*overlap_size;
            int start_j = j*block_size - j*overlap_size;
            
            double* errors;
            
            if(i==0 && j==0){
                int rand_block_row = rand() % num_blocks_h;
                int rand_block_col = rand() % num_blocks_w;
            //  printf("init index - y:%d x:%d\n", rand_block_row, rand_block_col);
                copy_blocks(image->data, out_data, im_width, out_size, 
                            0, 0, block_size, block_size,
                            0, 0, rand_block_row, rand_block_col);
                continue;     
            }else if(j==0){
                    copy_blocks(out_data, slice_data, out_size, block_size,
                            0, 0, block_size, overlap_size,
                            0, 0, start_i, 0);
                    out_slice->data = slice_data;
                    out_slice->width = block_size;
                    out_slice->height = block_size; 
                    double *errors = calc_errors(image, out_slice, block_size, overlap_size, "above");
                    find_indexes(errors, inds, num_blocks_h, num_blocks_w);
                    copy_blocks(image->data, out_data, im_width, out_size,
                           0, overlap_size, block_size, block_size,
                           start_i, 0, inds[0], inds[1]);
                    free(errors);                                    
                    continue;
            } else if(i==0){
                    copy_blocks(out_data, slice_data, out_size, block_size,
                            0, 0, overlap_size, block_size,
                            0, 0, 0, start_j);
                    out_slice->data = slice_data;
                    out_slice->width = block_size;
                    out_slice->height = block_size; 
                    double *errors = calc_errors(image, out_slice, block_size, overlap_size, "left");
                    find_indexes(errors, inds, num_blocks_h, num_blocks_w);
                    copy_blocks(image->data, out_data, im_width, out_size,
                            overlap_size, 0, block_size, block_size,
                            0,start_j, inds[0], inds[1]);
                    free(errors);  
                    continue;
            }else{
                copy_blocks(out_data, slice_data, out_size, block_size,
                            0, 0, block_size, overlap_size,
                            0, 0, start_i, 0);
                out_slice->data = slice_data;
                out_slice->width = block_size;
                out_slice->height = block_size; 
                double *errors = calc_errors(image, out_slice, block_size, overlap_size, "above");
                copy_blocks(out_data, slice_data, out_size, block_size,
                        0, 0, overlap_size, block_size,
                        0, 0, 0, start_j);
                out_slice->data = slice_data;
                out_slice->width = block_size;
                out_slice->height = block_size; 
                double *errors_left = calc_errors(image, out_slice, block_size, overlap_size, "left");
                double *errors_corner = calc_errors(image, out_slice, block_size, overlap_size, "corner");
                for (int y = 0; y < num_blocks_h; y++) {
                    for (int x = 0; x < num_blocks_w; x++){
                        int idx = y*num_blocks_w + x;
                        errors[idx] += (errors_left[idx] - errors_corner[idx]);       
                    }
                }
                find_indexes(errors, inds, num_blocks_h, num_blocks_w);
                copy_blocks(image->data, out_data, im_width, out_size,
                        overlap_size, overlap_size, block_size, block_size,
                        start_i, start_j, inds[0], inds[1]);
                free(errors_left);
                free(errors_corner);
                free(errors);
           }
        }
    }    
    
    free(slice_data);
    free(inds);
    
    Image *out = malloc(sizeof(Image));
    out->data = out_data;
    out->width = out_size;
    out->height = out_size;
    
    return out;

}

