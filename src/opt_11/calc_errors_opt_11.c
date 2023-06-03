#include "src/matrix.h"
#include "src/image.h"
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <immintrin.h>

int min_err_opt_11;
double tolerance_11 = 1.1;

ColorOrder get_image_color_order_opt_11(ImageRGB *im) {
    unsigned char *im_data_r = im->r_data;
    unsigned char *im_data_g = im->g_data;
    unsigned char *im_data_b = im->b_data;

    float r_sum = 0;
    float g_sum = 0;
    float b_sum = 0;

    int width = im->width;
    int height = im->height;

    for (int i = 0; i < width * height; ++i) {
        r_sum += (float) im_data_r[i];
        g_sum += (float) im_data_g[i];
        b_sum += (float) im_data_b[i];
    }

    if (r_sum >= g_sum && r_sum >= b_sum) {
        if (g_sum > b_sum) {
            return RGB_ORDER;
        } else {
            return RBG_ORDER;
        }
    } else if (g_sum >= r_sum && g_sum >= b_sum) {
        if (r_sum > b_sum) {
            return GRB_ORDER;
        } else {
            return GBR_ORDER;
        }
    } else {
        if (r_sum > g_sum) {
            return BRG_ORDER;
        } else {
            return BGR_ORDER;
        }
    }
}

void calc_errors_corner_opt_11(
        int *errors,
        ImageRGB *src, ImageRGB *out, ImageCoordinates out_coord, int block_size,
        int overlap
) {
    int src_idx;
    int out_idx;
    int err_idx = 0;

    int max_src_x = src->width - block_size;
    int max_src_y = src->height - block_size;

    int out_x = out_coord.x;
    int out_y = out_coord.y;
    int src_width = src->width;
    int out_width = out->width;

    unsigned char *src_data_r = src->r_data;
    unsigned char *src_data_g = src->g_data;
    unsigned char *src_data_b = src->b_data;
    unsigned char *out_data_r = out->r_data;
    unsigned char *out_data_g = out->g_data;
    unsigned char *out_data_b = out->b_data;

    int diff_0;

    int threshold = (min_err_opt_11 < INT_MAX) ? min_err_opt_11 * tolerance_11 : INT_MAX;
    
    

    for (int candidate_y = 0; candidate_y < max_src_y; ++candidate_y) {
        for (int candidate_x = 0; candidate_x < max_src_x; ++candidate_x) {
            int error = 0;

            // red
            for (int block_y = 0; block_y < overlap; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < block_size; ++block_x) {
                    diff_0 = src_data_r[src_idx] - out_data_r[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }
                if (error > threshold) {
                    break;
                }
            }

            if (error > threshold) {
                errors[err_idx] = INT_MAX;
                err_idx++;
                continue;
            }

            for (int block_y = overlap; block_y < block_size; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < overlap; ++block_x) {
                    diff_0 = src_data_r[src_idx] - out_data_r[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }

            if (error > threshold) {
                errors[err_idx] = INT_MAX;
                err_idx++;
                continue;
            }

            // green
            for (int block_y = 0; block_y < overlap; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < block_size; ++block_x) {
                    diff_0 = src_data_g[src_idx] - out_data_g[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }

            if (error > threshold) {
                errors[err_idx] = INT_MAX;
                err_idx++;
                continue;
            }

            for (int block_y = overlap; block_y < block_size; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < overlap; ++block_x) {
                    diff_0 = src_data_g[src_idx] - out_data_g[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }

            if (error > threshold) {
                errors[err_idx] = INT_MAX;
                err_idx++;
                continue;
            }

            //blue
            for (int block_y = 0; block_y < overlap; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < block_size; ++block_x) {
                    diff_0 = src_data_b[src_idx] - out_data_b[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }

            if (error > threshold) {
                errors[err_idx] = INT_MAX;
                err_idx++;
                continue;
            }

            for (int block_y = overlap; block_y < block_size; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < overlap; ++block_x) {
                    diff_0 = src_data_b[src_idx] - out_data_b[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }


            if (error < min_err_opt_11) {
                min_err_opt_11 = error;
                threshold = (int) (min_err_opt_11 * tolerance_11);
            }

            errors[err_idx] = error;
            err_idx++;
        }
    }
}

void calc_errors_above_opt_11(
        int *errors,
        ImageRGB *src, ImageRGB *out, ImageCoordinates out_coord, int block_size,
        int overlap
) {
    int src_idx;
    int out_idx;
    int err_idx = 0;

    int max_src_x = src->width - block_size;
    int max_src_y = src->height - block_size;

    int out_x = out_coord.x;
    int out_y = out_coord.y;
    int src_width = src->width;
    int out_width = out->width;

    unsigned char *src_data_r = src->r_data;
    unsigned char *src_data_g = src->g_data;
    unsigned char *src_data_b = src->b_data;
    unsigned char *out_data_r = out->r_data;
    unsigned char *out_data_g = out->g_data;
    unsigned char *out_data_b = out->b_data;

    int diff_0;

    int threshold = (min_err_opt_11 < INT_MAX) ? min_err_opt_11 * tolerance_11 : INT_MAX;


    for (int candidate_y = 0; candidate_y < max_src_y; ++candidate_y) {
        for (int candidate_x = 0; candidate_x < max_src_x; ++candidate_x) {
            int error = 0;

            // error on above overlap
            for (int block_y = 0; block_y < overlap; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < block_size; ++block_x) {
                    diff_0 = src_data_r[src_idx] - out_data_r[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }

            if (error > threshold) {
                errors[err_idx] = INT_MAX;
                err_idx++;
                continue;
            }

            for (int block_y = 0; block_y < overlap; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < block_size; ++block_x) {
                    diff_0 = src_data_g[src_idx] - out_data_g[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }

            if (error > threshold) {
                errors[err_idx] = INT_MAX;
                err_idx++;
                continue;
            }

            for (int block_y = 0; block_y < overlap; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < block_size; ++block_x) {
                    diff_0 = src_data_b[src_idx] - out_data_b[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }


            if (error < min_err_opt_11) {
                min_err_opt_11 = error;
                threshold = (int) (min_err_opt_11 * tolerance_11);
            }


            errors[err_idx] = error;
            err_idx++;
        }
    }
}


void calc_errors_left_opt_11(
        int *errors,
        ImageRGB *src, ImageRGB *out, ImageCoordinates out_coord, int block_size,
        int overlap
) {
    int src_idx;
    int out_idx;
    int err_idx = 0;

    int max_src_x = src->width - block_size;
    int max_src_y = src->height - block_size;

    int out_x = out_coord.x;
    int out_y = out_coord.y;
    int src_width = src->width;
    int out_width = out->width;

    unsigned char *src_data_r = src->r_data;
    unsigned char *src_data_g = src->g_data;
    unsigned char *src_data_b = src->b_data;
    unsigned char *out_data_r = out->r_data;
    unsigned char *out_data_g = out->g_data;
    unsigned char *out_data_b = out->b_data;

    int diff_0;

    int threshold = (min_err_opt_11 < INT_MAX) ? min_err_opt_11 * tolerance_11 : INT_MAX;


    for (int candidate_y = 0; candidate_y < max_src_y; ++candidate_y) {
        for (int candidate_x = 0; candidate_x < max_src_x; ++candidate_x) {
            int error = 0;

            // error on left overlap
            for (int block_y = 0; block_y < block_size; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < overlap; ++block_x) {
                    diff_0 = src_data_r[src_idx] - out_data_r[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }

            if (error > threshold) {
                errors[err_idx] = INT_MAX;
                err_idx++;
                continue;
            }

            for (int block_y = 0; block_y < block_size; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < overlap; ++block_x) {
                    diff_0 = src_data_g[src_idx] - out_data_g[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }

            if (error > threshold) {
                errors[err_idx] = INT_MAX;
                err_idx++;
                continue;
            }

            for (int block_y = 0; block_y < block_size; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < overlap; ++block_x) {
                    diff_0 = src_data_b[src_idx] - out_data_b[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }


            if (error < min_err_opt_11) {
                min_err_opt_11 = error;
                threshold = (int) (min_err_opt_11 * tolerance_11);
            }

            errors[err_idx] = error;
            err_idx++;
        }
    }
}

void predict_min_err_above( ImageRGB *src, ImageRGB *out, ImageCoordinates out_coord, int block_size,
        int overlap, ImageCoordinates im_coords_top){
        
    min_err_opt_11 = INT_MAX;      
       
    int src_idx;
    int out_idx;

    int out_x = out_coord.x;
    int out_y = out_coord.y;
    int src_width = src->width;
    int out_width = out->width;

    int candidate_x = im_coords_top.x;
 //   printf("candidate_x = %d\n", candidate_x);
    int candidate_y = im_coords_top.y;
//    printf("candidate_y = %d\n", candidate_y);
    if(candidate_y > src->height - 2*block_size+overlap){
        return;
    }

    candidate_y += block_size - overlap;

    unsigned char *src_data_r = src->r_data;
    unsigned char *src_data_g = src->g_data;
    unsigned char *src_data_b = src->b_data;
    unsigned char *out_data_r = out->r_data;
    unsigned char *out_data_g = out->g_data;
    unsigned char *out_data_b = out->b_data;

    int diff_0;

    int error = 0;
      
    // red
    for (int block_y = 0; block_y < overlap; ++block_y) {
        src_idx = (block_y + candidate_y) * src_width + candidate_x;
        out_idx = (block_y + out_y) * out_width + out_x;
        for (int block_x = 0; block_x < block_size; ++block_x) {
            diff_0 = src_data_r[src_idx] - out_data_r[out_idx];
            diff_0 = diff_0 * diff_0;
            error += diff_0;
            out_idx++;
            src_idx++;
        }
    }

    // green
    for (int block_y = 0; block_y < overlap; ++block_y) {
        src_idx = (block_y + candidate_y) * src_width + candidate_x;
        out_idx = (block_y + out_y) * out_width + out_x;
        for (int block_x = 0; block_x < block_size; ++block_x) {
            diff_0 = src_data_g[src_idx] - out_data_g[out_idx];
            diff_0 = diff_0 * diff_0;
            error += diff_0;
            out_idx++;
            src_idx++;
        }
    }

    //blue
    for (int block_y = 0; block_y < overlap; ++block_y) {
        src_idx = (block_y + candidate_y) * src_width + candidate_x;
        out_idx = (block_y + out_y) * out_width + out_x;
        for (int block_x = 0; block_x < block_size; ++block_x) {
            diff_0 = src_data_b[src_idx] - out_data_b[out_idx];
            diff_0 = diff_0 * diff_0;
            error += diff_0;
            out_idx++;
            src_idx++;
        }
    }
    if (error < min_err_opt_11) {
 //       printf("predicted min error = %d\n\n", error);
        min_err_opt_11 = error;
    }

}

void predict_min_err_left( ImageRGB *src, ImageRGB *out, ImageCoordinates out_coord, int block_size,
        int overlap, ImageCoordinates im_coords_left){
        
    min_err_opt_11 = INT_MAX;      
       
    int src_idx;
    int out_idx;

    int out_x = out_coord.x;
    int out_y = out_coord.y;
    int src_width = src->width;
    int out_width = out->width;

    int candidate_x = im_coords_left.x;
 //   printf("candidate_x = %d\n", candidate_x);
    int candidate_y = im_coords_left.y;
//    printf("candidate_y = %d\n", candidate_y);
    if(candidate_x > src_width - 2*block_size+overlap){
        return;
    }

    candidate_x += block_size - overlap;

    unsigned char *src_data_r = src->r_data;
    unsigned char *src_data_g = src->g_data;
    unsigned char *src_data_b = src->b_data;
    unsigned char *out_data_r = out->r_data;
    unsigned char *out_data_g = out->g_data;
    unsigned char *out_data_b = out->b_data;

    int diff_0;

    int error = 0;
      
    // red
    for (int block_y = 0; block_y < block_size; ++block_y) {
        src_idx = (block_y + candidate_y) * src_width + candidate_x;
        out_idx = (block_y + out_y) * out_width + out_x;
        for (int block_x = 0; block_x < overlap; ++block_x) {
            diff_0 = src_data_r[src_idx] - out_data_r[out_idx];
            diff_0 = diff_0 * diff_0;
            error += diff_0;
            out_idx++;
            src_idx++;
        }
    }

    // green
    for (int block_y = 0; block_y < block_size; ++block_y) {
        src_idx = (block_y + candidate_y) * src_width + candidate_x;
        out_idx = (block_y + out_y) * out_width + out_x;
        for (int block_x = 0; block_x < overlap; ++block_x) {
            diff_0 = src_data_g[src_idx] - out_data_g[out_idx];
            diff_0 = diff_0 * diff_0;
            error += diff_0;
            out_idx++;
            src_idx++;
        }
    }

    //blue
    for (int block_y = 0; block_y < block_size; ++block_y) {
        src_idx = (block_y + candidate_y) * src_width + candidate_x;
        out_idx = (block_y + out_y) * out_width + out_x;
        for (int block_x = 0; block_x < overlap; ++block_x) {
            diff_0 = src_data_b[src_idx] - out_data_b[out_idx];
            diff_0 = diff_0 * diff_0;
            error += diff_0;
            out_idx++;
            src_idx++;
        }
    }
    if (error < min_err_opt_11) {
   //     printf("predicted min error = %d\n\n", error);
        min_err_opt_11 = error;
    }

}

void predict_min_err_corner( ImageRGB *src, ImageRGB *out, ImageCoordinates out_coord, int block_size,
        int overlap, ImageCoordinates im_coords_top, ImageCoordinates im_coords_left){
        
    min_err_opt_11 = INT_MAX;      
       
    int src_idx;
    int out_idx;

    int out_x = out_coord.x;
    int out_y = out_coord.y;
    int src_width = src->width;
    int out_width = out->width;

    int candidate_x; 
    int candidate_y; 
    if(im_coords_top.y > src->height - 2*block_size+overlap){
        if(im_coords_left.x > src_width - 2*block_size+overlap){
            return;
        }else{
            candidate_x = im_coords_left.x+overlap;
            candidate_y = im_coords_left.y;
    }
    }else{
        candidate_x = im_coords_top.x;
        candidate_y = im_coords_top.y + overlap;
    }
//    printf("candidate_y = %d\n", candidate_y);
    candidate_y += block_size-overlap;

    unsigned char *src_data_r = src->r_data;
    unsigned char *src_data_g = src->g_data;
    unsigned char *src_data_b = src->b_data;
    unsigned char *out_data_r = out->r_data;
    unsigned char *out_data_g = out->g_data;
    unsigned char *out_data_b = out->b_data;

    int diff_0;

    int error = 0;
      
    // red
    for (int block_y = 0; block_y < overlap; ++block_y) {
        src_idx = (block_y + candidate_y) * src_width + candidate_x;
        out_idx = (block_y + out_y) * out_width + out_x;
        for (int block_x = 0; block_x < block_size; ++block_x) {
            diff_0 = src_data_r[src_idx] - out_data_r[out_idx];
            diff_0 = diff_0 * diff_0;
            error += diff_0;
            out_idx++;
            src_idx++;
        }
    }

    for (int block_y = overlap; block_y < block_size; ++block_y) {
        src_idx = (block_y + candidate_y) * src_width + candidate_x;
        out_idx = (block_y + out_y) * out_width + out_x;
        for (int block_x = 0; block_x < overlap; ++block_x) {
            diff_0 = src_data_r[src_idx] - out_data_r[out_idx];
            diff_0 = diff_0 * diff_0;
            error += diff_0;
            out_idx++;
            src_idx++;
        }
    }

    // green
    for (int block_y = 0; block_y < overlap; ++block_y) {
        src_idx = (block_y + candidate_y) * src_width + candidate_x;
        out_idx = (block_y + out_y) * out_width + out_x;
        for (int block_x = 0; block_x < block_size; ++block_x) {
            diff_0 = src_data_g[src_idx] - out_data_g[out_idx];
            diff_0 = diff_0 * diff_0;
            error += diff_0;
            out_idx++;
            src_idx++;
        }
    }

    for (int block_y = overlap; block_y < block_size; ++block_y) {
        src_idx = (block_y + candidate_y) * src_width + candidate_x;
        out_idx = (block_y + out_y) * out_width + out_x;
        for (int block_x = 0; block_x < overlap; ++block_x) {
            diff_0 = src_data_g[src_idx] - out_data_g[out_idx];
            diff_0 = diff_0 * diff_0;
            error += diff_0;
            out_idx++;
            src_idx++;
        }
    }

    //blue
    for (int block_y = 0; block_y < overlap; ++block_y) {
        src_idx = (block_y + candidate_y) * src_width + candidate_x;
        out_idx = (block_y + out_y) * out_width + out_x;
        for (int block_x = 0; block_x < block_size; ++block_x) {
            diff_0 = src_data_b[src_idx] - out_data_b[out_idx];
            diff_0 = diff_0 * diff_0;
            error += diff_0;
            out_idx++;
            src_idx++;
        }
    }

    for (int block_y = overlap; block_y < block_size; ++block_y) {
        src_idx = (block_y + candidate_y) * src_width + candidate_x;
        out_idx = (block_y + out_y) * out_width + out_x;
        for (int block_x = 0; block_x < overlap; ++block_x) {
            diff_0 = src_data_b[src_idx] - out_data_b[out_idx];
            diff_0 = diff_0 * diff_0;
            error += diff_0;
            out_idx++;
            src_idx++;
        }
    }

    if (error < min_err_opt_11) {
//        printf("predicted min error = %d\n\n", error);
        min_err_opt_11 = error;
    }

}

//int min_err_opt_11;
/*
 * computes the overlap error for all possible src image blocks
 */

void calc_errors_opt_11(
        int *errors,
        ImageRGB *src, ImageRGB *out, ImageCoordinates out_coord, int block_size,
        int overlap, Direction direction, ImageCoordinates im_coords_top, 
        ImageCoordinates im_coords_left
) {

    min_err_opt_11 = INT_MAX;
    switch (direction) {
        case CORNER:
            predict_min_err_corner(src, out, out_coord, block_size, overlap, 
                                    im_coords_top, im_coords_left);
            break;
        case ABOVE:
            predict_min_err_above(src, out, out_coord, block_size, overlap, im_coords_top);
            break;
        case LEFT:
            predict_min_err_left(src, out, out_coord, block_size, overlap, im_coords_left);
            break;
        default:
            exit(32);
    }
    switch (direction) {
        case CORNER:
            calc_errors_corner_opt_11(errors, src, out, out_coord, block_size, overlap);
            break;
        case ABOVE:
            calc_errors_above_opt_11(errors, src, out, out_coord, block_size, overlap);
            break;
        case LEFT:
            calc_errors_left_opt_11(errors, src, out, out_coord, block_size, overlap);
            break;
        default:
            exit(32);
    }
}

//void calc_errors_opt_11(
//        int *errors,
//        ImageRGB *src, ImageRGB *out, ImageCoordinates out_coord, int block_size,
//        int overlap, Direction direction
//) {
//
//    int src_idx;
//    int out_idx;
//
//    int max_src_x = src->width - block_size;
//    int max_src_y = src->height - block_size;
//
//    ColorV *src_data_r = src->r_data;
//    ColorV *src_data_g = src->g_data;
//    ColorV *src_data_b = src->b_data;
//    ColorV *out_data_r = out->r_data;
//    ColorV *out_data_g = out->g_data;
//    ColorV *out_data_b = out->b_data;
//
//    int r_error, g_error, b_error;
//
//    min_err_opt_11 = INT_MAX;
//
//
//    /* RED */
//    int err_idx = 0;
//    for (int candidate_y = 0; candidate_y < max_src_y; ++candidate_y) {
//        for (int candidate_x = 0; candidate_x < max_src_x; ++candidate_x) {
//
//
//            int error = 0;
//            if (direction == ABOVE) {
//                // error on above overlap
//                for (int block_y = 0; block_y < overlap; ++block_y) {
//                    for (int block_x = 0; block_x < block_size; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        r_error = src_data_r[src_idx] - out_data_r[out_idx];
//                        r_error = r_error * r_error;
//                        error += r_error;
//                    }
//                }
//
//            } else if (direction == LEFT) {
//                // error on left overlap
//                for (int block_y = 0; block_y < block_size; ++block_y) {
//                    for (int block_x = 0; block_x < overlap; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        r_error = src_data_r[src_idx] - out_data_r[out_idx];
//                        r_error = r_error * r_error;
//                        error += r_error;
//                    }
//                }
//            } else if (direction == CORNER) {
//
//                // red
//                for (int block_y = 0; block_y < overlap; ++block_y) {
//                    for (int block_x = 0; block_x < block_size; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        r_error = src_data_r[src_idx] - out_data_r[out_idx];
//                        r_error = r_error * r_error;
//                        error += r_error;
//                    }
//                }
//
//                for (int block_y = overlap; block_y < block_size; ++block_y) {
//                    for (int block_x = 0; block_x < overlap; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        r_error = src_data_r[src_idx] - out_data_r[out_idx];
//                        r_error = r_error * r_error;
//                        error += r_error;
//                    }
//                }
//            }
//
//            errors[err_idx] = error;
//            err_idx++;
//        }
//    }
//
//    /* GREEN */
//    err_idx = 0;
//    for (int candidate_y = 0; candidate_y < max_src_y; ++candidate_y) {
//        for (int candidate_x = 0; candidate_x < max_src_x; ++candidate_x) {
//
//
//            int error = 0;
//            if (direction == ABOVE) {
//                // error on above overlap
//                for (int block_y = 0; block_y < overlap; ++block_y) {
//                    for (int block_x = 0; block_x < block_size; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        g_error = src_data_g[src_idx] - out_data_g[out_idx];
//                        g_error = g_error * g_error;
//                        error += g_error;
//                    }
//                }
//
//            } else if (direction == LEFT) {
//                // error on left overlap
//                for (int block_y = 0; block_y < block_size; ++block_y) {
//                    for (int block_x = 0; block_x < overlap; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        g_error = src_data_g[src_idx] - out_data_g[out_idx];
//                        g_error = g_error * g_error;
//                        error += g_error;
//                    }
//                }
//            } else if (direction == CORNER) {
//                // green
//                for (int block_y = 0; block_y < overlap; ++block_y) {
//                    for (int block_x = 0; block_x < block_size; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        g_error = src_data_g[src_idx] - out_data_g[out_idx];
//                        g_error = g_error * g_error;
//                        error += g_error;
//                    }
//                }
//                for (int block_y = overlap; block_y < block_size; ++block_y) {
//                    for (int block_x = 0; block_x < overlap; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        g_error = src_data_g[src_idx] - out_data_g[out_idx];
//                        g_error = g_error * g_error;
//                        error += g_error;
//                    }
//                }
//            }
//            errors[err_idx] += error;
//            err_idx++;
//        }
//    }
//
//    /* BLUE */
//    err_idx = 0;
//    for (int candidate_y = 0; candidate_y < max_src_y; ++candidate_y) {
//        for (int candidate_x = 0; candidate_x < max_src_x; ++candidate_x) {
//
//
//            int error = 0;
//            if (direction == ABOVE) {
//                // error on above overlap
//                for (int block_y = 0; block_y < overlap; ++block_y) {
//                    for (int block_x = 0; block_x < block_size; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        b_error = src_data_b[src_idx] - out_data_b[out_idx];
//                        b_error = b_error * b_error;
//                        error += b_error;
//                    }
//                }
//
//            } else if (direction == LEFT) {
//                // error on left overlap
//                for (int block_y = 0; block_y < block_size; ++block_y) {
//                    for (int block_x = 0; block_x < overlap; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        b_error = src_data_b[src_idx] - out_data_b[out_idx];
//                        b_error = b_error * b_error;
//                        error += b_error;
//                    }
//                }
//            } else if (direction == CORNER) {
//                //blue
//                for (int block_y = 0; block_y < overlap; ++block_y) {
//                    for (int block_x = 0; block_x < block_size; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        b_error = src_data_b[src_idx] - out_data_b[out_idx];
//                        b_error = b_error * b_error;
//                        error += b_error;
//                    }
//                }
//                for (int block_y = overlap; block_y < block_size; ++block_y) {
//                    for (int block_x = 0; block_x < overlap; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        b_error = src_data_b[src_idx] - out_data_b[out_idx];
//                        b_error = b_error * b_error;
//                        error += b_error;
//                    }
//                }
//            }
//
//
//            errors[err_idx] += error;
//            if (errors[err_idx] < min_err_opt_11) {
//                min_err_opt_11 = errors[err_idx];
//            }
//            err_idx++;
//        }
//    }
//}

/*
 * chooses a random block from the source image with an error within the tolerance
 */
ImageCoordinates find_best_block_opt_11(
        int *errors, ImageRGB *src, int block_size
) {
    int max_src_x = src->width - block_size;
    int max_src_y = src->height - block_size;

    int threshold = (int) (min_err_opt_11 * tolerance_11);
    int candidate_idx = 0;

    for (int i = 0; i < max_src_x * max_src_y; ++i) {
        if (errors[i] <= threshold) {
            errors[candidate_idx] = i;
            candidate_idx++;
        }
    }

    int best_block_idx = errors[rand() % candidate_idx];
    ImageCoordinates best_block_coord = {
            best_block_idx % max_src_x, best_block_idx / max_src_x
    };
    return best_block_coord;
}


